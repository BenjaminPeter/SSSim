/* 
 * File:   Simulator.cpp
 * Author: peterb
 * 
 * Created on June 1, 2012, 10:53 AM
 */

#include "Simulator.h"
#include "Parameters.h"
#include <stdio.h>

#define DEBUG 0

Simulator::Simulator(long seed) {
    this->verbose = false;
    this->ut = new utils(seed);
    this->migrationScheme = 0;
    this->nLineagesStart = 0;
    this->nSamplesStart = 0;
    this->nMigrationEvents = 0;
    this->sampMapStart = boost::unordered_map<int, Sample*>();
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

    for (it = this->sampMapStart.begin(); it != this->sampMapStart.end(); ++it) {
        it->second->deleteAllLineages();
        delete it->second;
        it->second = NULL;
    }

    //delete this->migrationScheme;
    //delete this->expansionEvents;
    delete this->seqSim;
}

/*void Simulator::setHeight(int height){
    this->height=height;
    if (this->migrationScheme>0){
        this->migrationScheme->setHeight(height);
    }
}
void Simulator::setWidth(int width){
    this->width=width;
    if (this->migrationScheme>0){
        cout<<"set migration width to " <<width<<endl;
        this->migrationScheme->setWidth(width);
        cout<<"just set migration width to " <<this->migrationScheme->getWidth()<<endl;
    }
}*/

void Simulator::vPrint(string s) {
    if (this->verbose) {
        cout << s << "\n";
    }
}

void Simulator::addSampleStart(int* pos, int nNewLineages, bool outputLoci, stringstream* sOutputLoci) {
    this->addSampleStart(pos[0], pos[1], nNewLineages,outputLoci,sOutputLoci);
}

void Simulator::addSampleStart(int x, int y, int nNewLineages,bool outputLoci,stringstream* sOutputLoci) {
    int pos = this->migrationScheme->coords2d1d(x, y);
    if (this->migrationScheme->getArrivalTime(pos) == 0) {
        cerr << "Error: sample (" << x << "," << y << ") outside the colonized area" << endl;
        throw 10;
    } else {
    }

    Lineage* l;
    vector<Lineage*>newLineages;
    if (outputLoci){
        (*sOutputLoci) << this->nSamplesStart <<"\t"<< x << "\t"<< y<<endl;
    }
    for (int i = this->nLineagesStart; i<this->nLineagesStart + nNewLineages; i++) {
        l = new LineageTemplate(i, x, y, this->nSamplesStart);
        //printf("%s",l->toString().c_str());
        newLineages.push_back(l);
    }
    Sample* newSample = new Sample(x, y, newLineages, this->ut);
    this->nLineagesStart += nNewLineages;
    this->nSamplesStart++;
    this->sampMapStart.insert(pair<int, Sample*>(pos, newSample));
    this->vPrint("added new Sample");

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
    //cout <<"-------------\ntime:"<<this->timeSinceStart<<endl;
    //cout <<"propCoal:"<<evCoal->toString();
    //cout <<"propMig:"<<evMig->toString();//<<evMig->time<<endl;
    if (this->expansionEvents->size() > 0) {
        pair<double, int>* expansion = &this->expansionEvents->back();
        //cout <<"propExp:"<<expansion->first-this->timeSinceStart<<"/"<<expansion->second<<endl;
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
            //cout <<"recursive shit"<<endl;
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
    Sample* sample;
    int nl, pos;
    double tEvent;
    double rMax = this->coalRejFunction(this->timeSinceStart);

    tEvent = this->ut->nhpp2((void*) this, rMax, &Simulator::wrapper_coalRejFunction, this->timeSinceStart, false);
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
    Sample* sample;
    int nl, pos;
    double rMax = this->migRejFunction(this->timeSinceStart);
    double tEvent;


    tEvent = this->ut->nhpp2((void*) this, rMax, &Simulator::wrapper_migRejFunction, this->timeSinceStart, false);
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
        //cout <<"mRate: "<<(mNorth + mSouth + mEast + mWest) / pSize<<endl;
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
    double randomNumber = this->ut->randomD(lambdaT);

    for (iter = this->sampMap.begin(); iter != this->sampMap.end(); ++iter) {
        pos = iter->first;
        sample = iter->second;
        nl = sample->getNlineages();

        pSize = this->migrationScheme->getPopSize(pos, tEffective);
        mNorth = this->migrationScheme->getMigrationRate(0, pos, tEffective);
        mSouth = this->migrationScheme->getMigrationRate(1, pos, tEffective);
        mEast = this->migrationScheme->getMigrationRate(2, pos, tEffective);
        mWest = this->migrationScheme->getMigrationRate(3, pos, tEffective);
        //printf("[%d/%d]:%f\t%f\t%f\t%f\t\n",this->migrationScheme->coords1d2d(pos)[0],this->migrationScheme->coords1d2d(pos)[1],mNorth,mSouth,mEast,mWest);
        //printf("2:[%d/%d]:%f\t%f\t%f\t%f\t%f\n",this->migrationScheme->coords1d2d(pos)[0],this->migrationScheme->coords1d2d(pos)[1],nl*mNorth/pSize,nl*mSouth/pSize,nl*mEast/pSize,nl*mWest/pSize,randomNumber);
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
    //printf("got:[%d/%d]:%d\n",ev->sample->getX(),ev->sample->getY(),ev->type);
    return ev;
    fprintf(stderr, "Error: invalid migration rate");
    throw 10;
}

Event* Simulator::getNextEvent2() {
    cout << "NYI";
}

/*Event* Simulator::getNextEvent(){
    Event* curEvent = new Event();
    //terminate if only one lineage remains
    if (this->nLineages==1){
        return curEvent;
    }
    boost::unordered_map<int,Sample*>::const_iterator iter;
    Sample* sample;
    double t;
    for (iter = this->sampMap.begin();iter!=this->sampMap.end();++iter){
        sample=iter->second;
        int nl=sample->getNlineages();
        int sx=sample->getX();
        int sy=sample->getY();
        if (nl>1){
            t=this->ut->rexp(nl*(nl-1)/2);
            //printf("tCoal: %f\n",t);
            if (t<curEvent->time){
                delete curEvent;
                curEvent = new Event(1,sample,t);
            }
        }
        if (sy<this->height-1){
            t=ut->rexp(nl*this->mRates[0][sx][sy]);
            //printf("tMigN: %f\n",t);
            if (t<curEvent->time){
                delete curEvent;
                curEvent=new Event(2,sample,t);
            }
        }
        if (sy>0){
            t=ut->rexp(nl*this->mRates[1][sx][sy]);
            //printf("tMigS: %f\n",t);
            if (t<curEvent->time){
                delete curEvent;
                curEvent=new Event(3,sample,t);
            }
        }
        if (sx<this->width-1){
            t=ut->rexp(nl*this->mRates[2][sx][sy]);
            //printf("tMigE: %f\n",t);
            if (t<curEvent->time){
                delete curEvent;
                curEvent=new Event(4,sample,t);
            }
        }
        if (sx>0){
            t=ut->rexp(nl*this->mRates[3][sx][sy]);
            //printf("tMigW: %f\n",t);
            if (t<curEvent->time){
                delete curEvent;
                curEvent=new Event(5,sample,t);
            }
        }            
    }
    return curEvent;
}*/

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
    //cout << "coalescent event \t"<<ev->sample->getX()<<"\t"<<ev->sample->getY()<<"\t " ;
    //cout<< this->nLineages << " \tremaining\t" << this->timeSinceStart<<endl;
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
    //Coords* newCoords = new Coords(newX,newY);
    int newCoords = this->migrationScheme->coords2d1d(newX, newY);
    if (this->sampMap.count(newCoords) == 1) {
        Sample* newSample = this->sampMap[newCoords];
        newSample->addLineage(migLineage);
    } else {
        vector<Lineage*> newLineages;
        newLineages.push_back(migLineage);
        Sample* newSample = new Sample(newX, newY, newLineages, this->ut);
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
        //cout <<"nlineages:"<< sample->getNlineages()<<endl;
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

        //cout <<"neigbouring pop sizes:/mrates";
        //for (int i=0;i<4;i++){
        //    cout <<"\t"<<mRate[i]*pDir[i];
        //}
        //cout <<endl;        
        for (int i = 0; i < 4; i++) {
            rTot += pDir[i] * mRate[i];
        }

        //cout <<"rTot:"<<rTot<<endl;

        int l = 0;

        //propagule pool: send all migrants to a single adjacient deme

        if (this->propagulePoolMigration) {
            int direction;
            randomNumber = this->ut->randomD(rTot);
            for (int i = 0; i < 4; i++) {
                randomNumber -= pDir[i] * mRate[i];
                if (randomNumber < 0) {
                    direction = i;
                    break;
                }
            }
            bool samplePresent = true;
            while (samplePresent && sample->getNlineages() > 0) {
                // cout <<"moving lineage "<<l++;
                //randomNumber = this->ut->randomD(rTot);
                //cout <<"   rng:"<<randomNumber;
                //for (int i = 0; i < 4; i++) {
                //    randomNumber -= pDir[i] * mRate[i];
                //    if (randomNumber < 0) {
                        //cout <<"  direction "<<i+2;
                        //lineage =sample->getRandomLineageForMigration();
                        if (sample->getNlineages() == 1) {
                            samplePresent = false;
                        }
                        this->addMigrationEvent(new Event(direction + 2, sample, 0));
                        //cout<< "  "<< sample->getNlineages() << " lienages left"<<endl;
                        //break;
                //    }
                //}
            }
        } else {
            //migrant pool: send all migrants to random adjacient demes
            bool samplePresent = true;
            while (samplePresent && sample->getNlineages() > 0) {
                // cout <<"moving lineage "<<l++;
                randomNumber = this->ut->randomD(rTot);
                //cout <<"   rng:"<<randomNumber;
                for (int i = 0; i < 4; i++) {
                    randomNumber -= pDir[i] * mRate[i];
                    if (randomNumber < 0) {
                        //cout <<"  direction "<<i+2;
                        //lineage =sample->getRandomLineageForMigration();
                        if (sample->getNlineages() == 1) {
                            samplePresent = false;
                        }
                        this->addMigrationEvent(new Event(i + 2, sample, 0));
                        //cout<< "  "<< sample->getNlineages() << " lienages left"<<endl;
                        break;
                    }
                }
            }
        }

    } else {
        //cout<<"actually, nobody home, so nothing gets done"<<endl;
    }
    //cout <<"-------after--------"<<endl;
    //cout <<this->toString();
    //cout <<"-------eafter--------"<<endl;
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
        //printf("%s",sample->toString().c_str());
        ss << sample->toString();
    }
    return ss.str();
}

/*void Simulator::setupCoalEvents(){
    boost::unordered_map<int,Sample*>::const_iterator iter;
    int n;            rTot=1.;
    int* coords;
    double demeSize;
    for (iter =this->sampMap.begin();iter!=this->sampMap.end();++iter){
        n=iter->second->getNlineages();
        //coords = this->coords1d2d(iter->first);
        demeSize=this->migrationScheme->getPopSize(iter->first,0);
        this->coalEvents.insert(pair<int,double>(iter->first,double(n*(n-1)/2/demeSize)));
    }
}*/

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
        if (DEBUG) cout << this->timeSinceStart << "\t" << ev->toString() << endl;
        if (ev->type == 0) {
            delete ev;
            break;
        }
        evc++;
        //if (this->nLineages==4){
        //    cout <<this->toString();
        //    throw 10;
        // }
        this->addEvent(ev);
    }
    //cout <<"got Gene tree with " <<evc << "events"<<endl;
    if (this->sampMap.size() != 1) {
        cerr << "error. sampMap size is " << this->sampMap.size() << ",should be 1";
        throw 10;
    }
    delete this->expansionEvents;
    boost::unordered_map<int, Sample*>::iterator it = this->sampMap.begin();
    Lineage* l = it->second->getRandomLineageForMigration();
    delete (it->second);
    this->sampMap.erase(it);

    //tidy up
    //this->sampMap.clear();
    return l;
}

void Simulator::copySampStartToSamp() {
    Sample * sample;
    int pos;
    boost::unordered_map<int, Sample*>::const_iterator it;
    this->sampMap.clear();
    for (it = this->sampMapStart.begin(); it != this->sampMapStart.end(); ++it) {
        sample = new Sample(*it->second, this->nSamplesStart);
        pos = it->first;
        this->sampMap.insert(pair<int, Sample*>(pos, sample));
    }
    this->nSamples = this->nSamplesStart;
    this->nLineages = this->nLineagesStart;
}

void Simulator::addSequenceSimulator(SequenceSimulator * ss) {
    this->seqSim = ss;
    this->seqSim->ut = this->ut;
}

SimulationResults* Simulator::doSimulations(Parameters* params) {

    vector<int*> samples = params->samples;
    //vector<SFS*> curSFS;
    int pos = 0, nReplicates = params->nReplicates;
    int* sampleSizes = params->sampleSizes;
    SimulationResults* res = new SimulationResults();
    res->initialize(params, samples.size(), sampleSizes);
    double tmrca = 0;
    int nMigrations = 0;
    SFS* sfs;
    FreqTable* ft = new FreqTable(this->ut, params->samples.size());
    FreqTable* ftShared = new FreqTable(this->ut, params->samples.size());
    this->propagulePoolMigration = params->mPropagulePool;

    Lineage* l;
    for (int r = 0; r < nReplicates; r++) {
        //cout<<r<<nReplicates<<float(r)/nReplicates<<endl;
        utils::printProgressBar(100*r/nReplicates,"computing trees");
        l = this->getNewGeneTree();

        l->addToFreqTable(ft);
        l->addToFreqTable(ftShared,true);
        pos = 0;
        for (int i = 0; i < samples.size() - 1; i++) {
            for (int j = i + 1; j < samples.size(); j++) {
                sfs = this->seqSim->create2DSFS(l, i, j);
                //res->vDeltaH[pos][r] = StatCalculator::getDeltaH(sfs);
                //res->vPsi[pos][r] = StatCalculator::getPsi(sfs);
                //res->vFst[pos][r] = StatCalculator::getFST(sfs);
                *(res->sumSFS[pos]) += *sfs;
                //curSFS.push_back(sfs);
                //res->sfsSingleTrees[pos][r]=sfs;
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
            for (int r = 0; r < nReplicates; r++) {
                //SFS* tmpSFS = new SFS(*res->sumSFS[pos]);
                //cout <<tmpSFS->toString();
                //*(tmpSFS) -= *(curSFS[pos]);
                //res->vPsi[pos][r]=StatCalculator::getPsi(tmpSFS);
                //res->vFst[pos][r]=StatCalculator::getFST(tmpSFS);
                //res->vDeltaH[pos][r]=StatCalculator::getDeltaH(tmpSFS);
            }
            //cout << i << "\t" << j << "\t" << res->fst[pos] << "\t" << res->psi[pos] << "\t" << res->deltaH[pos] << endl;
            pos++;
        }
    }


    //    for (vector<SFS*>::iterator it = sumSFS.begin(); it != sumSFS.end(); ++it) {
    //        delete (*it);



    return res;

}
