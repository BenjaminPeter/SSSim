/* 
 * File:   Simulator.cpp
 * Author: peterb
 * 
 * Created on June 1, 2012, 10:53 AM
 */

#include "Simulator.h"




Simulator::Simulator(long seed) {
    this->ut = new utils(seed);
    this->migrationScheme = 0;
    this->nMigrationEvents = 0;
    this->propagulePoolMigration = false;
}

Simulator::Simulator(const Simulator& orig) {
}

Simulator::~Simulator() {
    delete this->ut;

    //delete all lineages in all samples, then delete all samples
    boost::unordered_map<int, Sample*>::iterator it;

    for (it = this->sampMap.begin(); it != this->sampMap.end(); ++it) {
        it->second->deleteAllLineages();
        delete it->second;
        it->second = NULL;
    }


    delete this->seqSim;
}




void Simulator::removeSample(int x, int y) {
    int pos = this->migrationScheme->coords2d1d(x, y);
    Sample* a = this->sampMap[pos];
    delete a;
    this->sampMap.erase(pos);
    this->nSamples -= 1;
}

Event* Simulator::getNextEvent3() {
    if (this->nLineages == 1) {
        return new Event();
    }
    Event* evCoal = this->getNextCoalEvent();
    Event* evMig = this->getNextMigEvent();
    
    if(Parameters::verbose>999){
        cout <<"-------------\ntime:"<<this->timeSinceStart<<endl;
        cout <<"propCoal:"<<evCoal->toString();
        cout <<"propMig:"<<evMig->toString();//<<evMig->time<<endl;
    }
    if (this->expansionEvents->size() > 0) {
        pair<double, int>* expansion = &this->expansionEvents->back();
        
        if(Parameters::verbose>999)
            cout <<"propExp:"<<expansion->first-this->timeSinceStart<<"/"<<expansion->second<<endl;
        
        if (expansion->first < this->timeSinceStart + evCoal->time && expansion->first < this->timeSinceStart + evMig->time) {
            this->addExpansionEvent(expansion);
            this->expansionEvents->pop_back();
            while (this->expansionEvents->size() > 0) {
                expansion = &this->expansionEvents->back();
                if (expansion->first - this->timeSinceStart > 0.0001) {
                    break;
                }
                this->addExpansionEvent(expansion);
                this->expansionEvents->pop_back();
            }
            //recursive call for next event after expansion
            delete evCoal;
            delete evMig;
            return this->getNextEvent3();
            //break;
        }
    }
    if (evCoal->time > evMig->time) {
        delete evCoal;
        return evMig;
    } else {
        delete evMig;
        return evCoal;
    }
}

Event* Simulator::getNextCoalEvent() {
    double tEvent;
    double rMax = this->coalRejFunction(this->timeSinceStart);
    if(Parameters::verbose>1999)
        cout <<"getNextCoalEvent::rMax:" <<rMax<<endl;

    tEvent = this->ut->nhpp2((void*) this, rMax, &Simulator::wrapper_coalRejFunction, this->timeSinceStart, false);
    if(Parameters::verbose>1999)
        cout <<"getNextCoalEvent::tEvent:" <<tEvent  <<endl;
    return this->whichCoalEvent(tEvent);
}

double Simulator::coalRejFunction(const double t) {
    boost::unordered_map<int, Sample*>::const_iterator iter;
    Sample*sample;
    int nl, pos;
    double pSize, lambdaT = 0.0;

    for (iter = this->sampMap.begin(); iter != this->sampMap.end(); ++iter) {
        pos = iter->first;
        sample = iter->second;
        nl = sample->getNlineages();
        pSize = this->migrationScheme->getPopSize(pos, t);
        if(Parameters::verbose>1999){
            cout <<"coalRejFunction:: ["<<pos<<"\t"<<nl<<"\t"<<pSize<<"]"<<endl;        
        }
        
        if (pSize != 0)
            lambdaT += nl * (nl - 1.) / 2. / pSize;
    }
    return lambdaT;
}

Event* Simulator::whichCoalEvent(double t) {
    boost::unordered_map<int, Sample*>::const_iterator iter;
    Sample*sample;
    int nl, pos;
    double lambdaT = 0.0;
    double pSize, tEffective = t + this->timeSinceStart;

    if (t > 100000000000) {
        return new Event();
    }
    //this is stupid, cause I am doing the same thing essentially twice
    for (iter = this->sampMap.begin(); iter != this->sampMap.end(); ++iter) {
        pos = iter->first;
        sample = iter->second;
        nl = sample->getNlineages();
        pSize = this->migrationScheme->getPopSize(pos, tEffective);
        if (pSize != 0)
            lambdaT += nl * (nl - 1.) / 2. / pSize;
    }
    if (lambdaT < 0.000000001) return new Event(0, NULL, 100000000000);
    double randomNumber = this->ut->randomD(lambdaT);

    for (iter = this->sampMap.begin(); iter != this->sampMap.end(); ++iter) {
        pos = iter->first;
        sample = iter->second;
        nl = sample->getNlineages();
        pSize = this->migrationScheme->getPopSize(pos, tEffective);
        if (pSize != 0)
            randomNumber -= nl * (nl - 1.) / 2. / pSize;
        if (randomNumber <= 0) {//the event!
            return new Event(1, sample, tEffective - this->timeSinceStart);
        }
    }
    fprintf(stdout, "Error: invalid coalescent event\n");
    throw 10;
}

Event* Simulator::getNextMigEvent() {
    double rMax = this->migRejFunction(this->timeSinceStart);
    double tEvent;

    tEvent = utils::nhpp2((void*) this, rMax, &Simulator::wrapper_migRejFunction, this->timeSinceStart, false);
    return this->whichMigEvent(tEvent);
}

double Simulator::migRejFunction(const double t) {
    boost::unordered_map<int, Sample*>::const_iterator iter;
    Sample*sample;
    int nl, pos;
    double pSize, lambdaT = 0.0;
    double mNorth, mSouth, mEast, mWest;


    for (iter = this->sampMap.begin(); iter != this->sampMap.end(); ++iter) {
        pos = iter->first;
        sample = iter->second;
        nl = sample->getNlineages();
        mNorth = this->migrationScheme->getMigrationRate(0, pos, t);
        mSouth = this->migrationScheme->getMigrationRate(1, pos, t);
        mEast = this->migrationScheme->getMigrationRate(2, pos, t);
        mWest = this->migrationScheme->getMigrationRate(3, pos, t);
        pSize = this->migrationScheme->getPopSize(pos, t);
        if (pSize > 0)
            lambdaT += nl * (mNorth + mSouth + mEast + mWest) / pSize;
    }
    return lambdaT;
}

Event* Simulator::whichMigEvent(double t) {
    boost::unordered_map<int, Sample*>::const_iterator iter;
    Sample*sample;
    int nl, pos;
    double lambdaT = 0.0;
    double mNorth, mSouth, mEast, mWest, pSize;
    Event* ev;
    double tEffective = t + this->timeSinceStart;

    //this is stupid, cause I am doing the same thing essentially twice
    for (iter = this->sampMap.begin(); iter != this->sampMap.end(); ++iter) {
        pos = iter->first;
        sample = iter->second;
        nl = sample->getNlineages();
        mNorth = this->migrationScheme->getMigrationRate(0, pos, tEffective);
        mSouth = this->migrationScheme->getMigrationRate(1, pos, tEffective);
        mEast = this->migrationScheme->getMigrationRate(2, pos, tEffective);
        mWest = this->migrationScheme->getMigrationRate(3, pos, tEffective);
        pSize = this->migrationScheme->getPopSize(pos, tEffective);
        if (pSize > 0)
            lambdaT += nl * (mNorth + mSouth + mEast + mWest) / pSize;
    }
    if (lambdaT < 0.000000001) return new Event(0, NULL, 100000000000);
    double randomNumber = utils::randomD(lambdaT);

    for (iter = this->sampMap.begin(); iter != this->sampMap.end(); ++iter) {
        pos = iter->first;
        sample = iter->second;
        nl = sample->getNlineages();

        pSize = this->migrationScheme->getPopSize(pos, tEffective);
        mNorth = this->migrationScheme->getMigrationRate(0, pos, tEffective);
        mSouth = this->migrationScheme->getMigrationRate(1, pos, tEffective);
        mEast = this->migrationScheme->getMigrationRate(2, pos, tEffective);
        mWest = this->migrationScheme->getMigrationRate(3, pos, tEffective);
        if (pSize > 0) {
            randomNumber -= nl * mNorth / pSize;
            if (randomNumber <= 0) {//the event!
                ev = new Event(2, sample, tEffective - this->timeSinceStart);
                break;
            }
            randomNumber -= nl * mSouth / pSize;
            if (randomNumber <= 0) {//the event!
                ev = new Event(3, sample, tEffective - this->timeSinceStart);
                break;
            }
            randomNumber -= nl * mEast / pSize;
            if (randomNumber <= 0) {//the event!
                ev = new Event(4, sample, tEffective - this->timeSinceStart);
                break;
            }
            randomNumber -= nl * mWest / pSize;
            if (randomNumber <= 0) {//the event!
                ev = new Event(5, sample, tEffective - this->timeSinceStart);
                break;
            }
        }
    }
    return ev;
    fprintf(stderr, "Error: invalid migration rate");
    throw 10;
}



void Simulator::addEvent(Event* ev) {
    if (ev->type == 1) this->addCoalEvent(ev);
    else if (ev->type > 1 && ev->type < 6) this->addMigrationEvent(ev);
    else if (ev->type == 0) this->terminate();
    else cout << "ERROR: INVALID EVENT" << ev->toString() << endl;
}

void Simulator::addCoalEvent(Event* ev) {
    this->timeSinceStart += ev->time;
    Sample * curSample = ev->sample;
    curSample->merge2randomLineages(this->timeSinceStart);

    this->nLineages--;
    this->timeSinceLastCoalEvent = 0;
    delete ev;
}

void Simulator::addMigrationEvent(Event* ev) {
    //cout <<"mig"<< ev->toString()<<endl;
    int newX, newY;
    this->nMigrationEvents++;
    this->timeSinceStart += ev->time;
    Sample * curSample = ev->sample;
    switch (ev->type) {
        case 2:
            newX = curSample->getX();
            newY = curSample->getY() + 1;
            break;
        case 3:
            newX = curSample->getX();
            newY = curSample->getY() - 1;
            break;
        case 4:
            newX = curSample->getX() + 1;
            newY = curSample->getY();
            break;
        case 5:
            newX = curSample->getX() - 1;
            newY = curSample->getY();
            break;
    }
    Lineage * migLineage = curSample->getRandomLineageForMigration();
    int newCoords = this->migrationScheme->coords2d1d(newX, newY);
    if (this->sampMap.count(newCoords) == 1) {
        Sample* newSample = this->sampMap[newCoords];
        newSample->addLineage(migLineage);
    } else {
        vector<Lineage*> newLineages;
        newLineages.push_back(migLineage);
        Sample* newSample = new Sample(newX, newY, newLineages);
        this->sampMap.insert(pair<int, Sample*>(newCoords, newSample));
        this->nSamples += 1;
    }
    this->timeSinceLastCoalEvent += ev->time;

    if (curSample->getNlineages() == 0) {
        this->removeSample(ev->sample->getX(), ev->sample->getY());
    }
    delete ev;
}

void Simulator::addExpansionEvent(pair<double, int>* ev) {

    int pos = ev->second;
    int* arr = this->migrationScheme->coords1d2d(pos);
    int x = arr[0];
    int y = arr[1];
    delete[] arr;
        if(Parameters::verbose>499){
        cout <<"["<<ev->first<<"]:Expansion in deme ("<<x<<","<<y<<")"<<endl;
    }

    int k = this->migrationScheme->getExpansionK(x, y);
    double pDir[4], mRate[4], rTot = 0, randomNumber;
    Lineage * lineage;


    this->timeSinceStart = ev->first;
    if (this->sampMap.count(ev->second) == 1) {
        Sample * sample = this->sampMap[ev->second];

        //if there is a founding propagule size set, check if we need to perform
        // additional coalescent events

        if (k > 0 && sample->getNlineages() > 1) {
            int nAncLineages = 1;
            double pNAncLineages, logPNAncLineages;
            randomNumber = this->ut->randomD(1);

            //if there are more lineages than k, reduce to k...
            if (sample->getNlineages() > k) {
                int nCoal = sample->getNlineages() - k;
                for (int i = 0; i < nCoal; i++) {
                    this->addCoalEvent(new Event(1, sample, 0));
                }
            } else {
                while (nAncLineages <= k) {
                    logPNAncLineages = log(this->ut->getStirlingNumberD(sample->getNlineages(),
                            nAncLineages));
                    //pNAncLineages = this->ut->getStirlingNumber(sample->getNlineages(),
                    logPNAncLineages += this->ut->logFallingFactorial(k, nAncLineages);
                    //pNAncLineages *= this->ut->fallingFactorial(k, nAncLineages);
                    logPNAncLineages -= log(k) * sample->getNlineages();
                    //pNAncLineages /= pow(k, sample->getNlineages());
                    //randomNumber -= pNAncLineages;
                    float f = exp(logPNAncLineages);
                    if (f == f) //check for nan
                        randomNumber -= f;
                    if (randomNumber < 0) {
                        int nCoal = sample->getNlineages() - nAncLineages;
                        for (int i = 0; i < nCoal; i++) {
                            this->addCoalEvent(new Event(1, sample, 0));
                        }
                        break;
                    }
                    nAncLineages++;
                }
            }
        }
        int h = this->migrationScheme->getHeight();
        //get population sizes/migration rates of surrounding demes:
        pDir[0] = this->migrationScheme->getPopSize(pos + 1, ev->first);
        pDir[1] = this->migrationScheme->getPopSize(pos - 1, ev->first);
        pDir[2] = this->migrationScheme->getPopSize(pos + h, ev->first);
        pDir[3] = this->migrationScheme->getPopSize(pos - h, ev->first);


        mRate[0] = this->migrationScheme->getMigrationRate(0, pos, ev->first);
        mRate[1] = this->migrationScheme->getMigrationRate(1, pos, ev->first);
        mRate[2] = this->migrationScheme->getMigrationRate(2, pos, ev->first);
        mRate[3] = this->migrationScheme->getMigrationRate(3, pos, ev->first);

        for (int i = 0; i < 4; i++) {
            rTot += pDir[i] * mRate[i];
        }

        int l = 0;

        //propagule pool: send all migrants to a single adjacient deme

        if (this->propagulePoolMigration) {
            int direction;
            randomNumber = this->ut->randomD(rTot);
            for (int i = 0; i < 4; i++) {
                randomNumber -= pDir[i] * mRate[i];
                if (randomNumber < 0) {
                    if(Parameters::verbose>999){
                        cout <<"Deme("<<x<<","<<y<<"); ppMigration: direction"<<i<<endl;
                    }
                    direction = i;
                    break;
                }
            }
            bool samplePresent = true;
            while (samplePresent && sample->getNlineages() > 0) {
                if (sample->getNlineages() == 1) {
                    samplePresent = false;
                }
                this->addMigrationEvent(new Event(direction + 2, sample, 0));
            }
        } else {
            //migrant pool: send all migrants to random adjacient demes
            bool samplePresent = true;
            while (samplePresent && sample->getNlineages() > 0) {
                randomNumber = this->ut->randomD(rTot);
                for (int i = 0; i < 4; i++) {
                    randomNumber -= pDir[i] * mRate[i];
                    if (randomNumber < 0) {
                        if (sample->getNlineages() == 1) {
                            samplePresent = false;
                        }
                        this->addMigrationEvent(new Event(i + 2, sample, 0));
                        break;
                    }
                }
            }
        }

    } else {
        //cout<<"actually, nobody home, so nothing gets done"<<endl;
    }
}

void Simulator::terminate() {
    cerr << "terminate NYI" << endl;
}

string Simulator::toString() {
    char buffer[100];
    stringstream ss;
    sprintf(buffer, "SSSimulator on a (%d x %d) grid\n",
            this->migrationScheme->getWidth()
            , this->migrationScheme->getHeight());
    ss << buffer;
    sprintf(buffer, "total time is %f. (time since last coal is %f\n", this->timeSinceStart, this->timeSinceLastCoalEvent);
    ss << buffer;
    sprintf(buffer, "contains %d lineages in %d samples: \n", this->nLineages, this->nSamples);
    ss << buffer;
    boost::unordered_map<int, Sample*>::const_iterator iter;
    Sample* sample;
    for (iter = this->sampMap.begin(); iter != this->sampMap.end(); ++iter) {
        sample = iter->second;
        ss << sample->toString();
    }
    return ss.str();
}

Lineage* Simulator::getNewGeneTree() {
    this->copySampStartToSamp();
    this->timeSinceLastCoalEvent = 0;
    this->timeSinceStart = 0;
    this->nMigrationEvents = 0;
    this->expansionEvents = this->migrationScheme->getExpansionEvents();
    Event *ev;
    int evc = 0;
    while (true) {
        ev = this->getNextEvent3();
        if (Parameters::verbose>999) cout << "["<<this->timeSinceStart<<"]:" << ev->toString() << endl;
        if (ev->type == 0) {
            delete ev;
            break;
        }
        evc++;

        this->addEvent(ev);
    }
    if (this->sampMap.size() != 1) {
        cerr << "error. sampMap size is " << this->sampMap.size() << ",should be 1";
        throw 10;
    }
    delete this->expansionEvents;
    boost::unordered_map<int, Sample*>::iterator it = this->sampMap.begin();
    Lineage* l = it->second->getRandomLineageForMigration();
    delete (it->second);
    this->sampMap.erase(it);

    return l;
}

void Simulator::copySampStartToSamp() {
    Sample * sample;
    int pos;
    boost::unordered_map<int, Sample*>::const_iterator it;
    this->sampMap.clear();
    for (it = Parameters::sampMapStart.begin(); it != Parameters::sampMapStart.end(); ++it) {
        sample = new Sample(*it->second, Parameters::nSamplesStart);
        pos = it->first;
        this->sampMap.insert(pair<int, Sample*>(pos, sample));
    }
    this->nSamples = Parameters::nSamplesStart;
    this->nLineages = Parameters::nLineagesStart;
}

void Simulator::addSequenceSimulator(SequenceSimulator * ss) {
    this->seqSim = ss;
    this->seqSim->ut = this->ut;
}

SimulationResults* Simulator::doSimulations(Parameters* params) {

    vector<int*> samples = params->samples;
    int pos = 0, nReplicates = params->nReplicates;
    int* sampleSizes = params->sampleSizes;
    SimulationResults* res = new SimulationResults();
    res->initialize(params, samples.size(), sampleSizes);
    double tmrca = 0;
    int nMigrations = 0;
    SFS* sfs;
    FreqTable* ft = new FreqTable(params->samples.size());
    FreqTable* ftShared = new FreqTable(params->samples.size());
    this->propagulePoolMigration = params->mPropagulePool;

    Lineage* l;
    ofstream f;

    if(params->outputTree){
        char s[100];
        sprintf(s, "%s.tree", params->outputPrefix.c_str());
        f.open(s, ios::out);   
    }
    
    for (int r = 0; r < nReplicates; r++) {
        utils::printProgressBar(100*r/nReplicates,"computing trees");
        l = this->getNewGeneTree();
      
        
        if(params->outputTree)
            f << l->toString()<<endl;
        

        l->addToFreqTable(ft);
        l->addToFreqTable(ftShared,true);
        
        pos = 0;
        for (int i = 0; i < samples.size() - 1; i++) {
            for (int j = i + 1; j < samples.size(); j++) {
                sfs = this->seqSim->create2DSFS(l, i, j);

                *(res->sumSFS[pos]) += *sfs;
                delete sfs;
                pos++;
            }
        }
        for (int i = 0; i < samples.size(); i++) {
            sfs = this->seqSim->create1DSFS(l, i);
            *(res->sumSFS1d[i]) += *sfs;
        }
        tmrca += this->timeSinceStart;
        nMigrations += this->nMigrationEvents;
        delete l;
    }
    if(params->outputTree)
        f.close();
    cout <<"done!"<<endl;

    tmrca /= nReplicates;
    cout << "TMRCA:\t" << tmrca << "\tmEvents\t" << nMigrations << endl;

    res->ft = ft;
    res->ftShared = ftShared;

    pos = 0;
    for (int i = 0; i < samples.size() - 1; i++) {
        for (int j = i + 1; j < samples.size(); j++) {
            res->fst[pos] = StatCalculator::getFST(res->sumSFS[pos]);
            res->psi[pos] = StatCalculator::getPsi(res->sumSFS[pos]);
            res->deltaH[pos] = StatCalculator::getDeltaH(res->sumSFS[pos]);
            pos++;
        }
    }

    return res;

}
