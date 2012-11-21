/* 
 * File:   TreeSimulator.cpp
 * Author: peterb
 * 
 * Created on November 6, 2012, 5:21 PM
 */

#include "TreeSimulator.h"

TreeSimulator::TreeSimulator(Parameters* params) {
    this->params = params;
    this->nMigrationEvents = 0;
}

TreeSimulator::TreeSimulator(const TreeSimulator& orig) {
}

TreeSimulator::~TreeSimulator() {
}

void TreeSimulator::copySampStartToSamp() {
    Sample * sample;
    Coords pos;
    SampleList::const_iterator it;
    this->sampMap.clear();
    for (it = Parameters::sampMapStart.begin(); it != Parameters::sampMapStart.end(); ++it) {
        sample = new Sample(*it->second, Parameters::nSamplesStart);
        pos = it->first;
        this->sampMap.insert(pair<Coords, Sample*>(pos, sample));
    }
    this->nSamples = Parameters::nSamplesStart;
    this->nLineages = Parameters::nLineagesStart;
}

LCV TreeSimulator::run(int id) {

    this->copySampStartToSamp();
    this->timeSinceLastCoalEvent = 0;
    this->timeSinceStart = 0;
    this->nMigrationEvents = 0;
    this->expansionEvents = params->ms->getExpansionEvents();


    Event* ev;
    int evc = 0;
    while (true) {
        ev = this->getNextEvent();


        if (Parameters::verbose > 999) cout << "[" << this->timeSinceStart << "]:" << ev->toString() << endl;
        if (ev->type == 0) {
            delete ev;
            break;
        }
        evc++;

        this->addEvent(ev);
    }
    if (this->sampMap.size() != 1) {
        cout << this->toString() << endl;
        cerr << "error. sampMap size is " << this->sampMap.size() << ",should be 1";
        throw 10;
    }
    delete this->expansionEvents;
    boost::unordered_map<Coords, Sample*>::iterator it = this->sampMap.begin();
    Lineage* l = it->second->getRandomLineageForMigration();
    delete (it->second);
    this->sampMap.erase(it);

    LCV lcv;
    //lcv.c = NULL;
    lcv.l = l;
    if (Parameters::outputCoal) {
        lcv.c = this->coalEvents;
    }
    return lcv;
}

Event* TreeSimulator::getNextEvent() {
    if (this->nLineages == 1) {
        return new Event();
    }
    Event* evCoal = this->getNextCoalEvent();
    Event* evMig = this->getNextMigEvent();

    if (Parameters::verbose > 999) {
        cout << "-------------\ntime:" << this->timeSinceStart << endl;
        cout << "propCoal:" << evCoal->toString();
        cout << "propMig:" << evMig->toString(); //<<evMig->time<<endl;
    }
    if (Parameters::verbose > 1999) {
        cout << this->toString() << endl;
    }

    if (this->expansionEvents->size() > 0) {
        ExpansionEvent* expansion = &this->expansionEvents->back();

        if (Parameters::verbose > 999)
            cout << "propExp:" << expansion->second - this->timeSinceStart << "/" << expansion->first << endl;

        if (expansion->second < this->timeSinceStart + evCoal->time && expansion->second < this->timeSinceStart + evMig->time) {
            this->addExpansionEvent(expansion);
            this->expansionEvents->pop_back();
            while (this->expansionEvents->size() > 0) {
                expansion = &this->expansionEvents->back();
                if (expansion->second - this->timeSinceStart > 0.0001) {
                    break;
                }
                this->addExpansionEvent(expansion);
                this->expansionEvents->pop_back();
            }
            //recursive call for next event after expansion
            delete evCoal;
            delete evMig;
            return this->getNextEvent();
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

void TreeSimulator::addEvent(Event* ev) {
    if (ev->type == 1) this->addCoalEvent(ev);
    else if (ev->type > 1 && ev->type < 6) this->addMigrationEvent(ev);
    else if (ev->type == 0) this->terminate();
    else cout << "ERROR: INVALID EVENT" << ev->toString() << endl;
}

Event* TreeSimulator::getNextCoalEvent() {
    double tEvent;
    double rMax = this->coalRejFunction(this->timeSinceStart);
    if (Parameters::verbose > 1999)
        cout << "getNextCoalEvent::rMax:" << rMax << endl;

    tEvent = utils::nhpp2((void*) this, rMax, &TreeSimulator::wrapper_coalRejFunction, this->timeSinceStart, false);
    if (tEvent == -1.0) {
        tEvent = 0;
    }
    if (Parameters::verbose > 1999)
        cout << "getNextCoalEvent::tEvent:" << tEvent << endl;
    return this->whichCoalEvent(tEvent);
}

double TreeSimulator::coalRejFunction(const double t) {
    SampleList::const_iterator iter;
    Sample* sample;
    int nl;
    Coords pos;
    double pSize, lambdaT = 0.0;

    for (iter = this->sampMap.begin(); iter != this->sampMap.end(); ++iter) {
        pos = iter->first;
        sample = iter->second;
        nl = sample->getNlineages();
        pSize = this->params->ms->getPopSize(pos, t);
        if (Parameters::verbose > 2999) {
            cout << "coalRejFunction:: [" << pos << "\t" << nl << "\t" << pSize << "]" << endl;
        }

        if (pSize != 0)
            lambdaT += nl * (nl - 1.) / 2. / pSize;
    }
    return lambdaT;
}

Event* TreeSimulator::whichCoalEvent(double t) {
    SampleList::const_iterator iter;
    Sample*sample;
    int nl;
    Coords pos;
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
        pSize = this->params->ms->getPopSize(pos, tEffective);
        if (pSize != 0)
            lambdaT += nl * (nl - 1.) / 2. / pSize;
    }
    if (lambdaT < 0.000000001) return new Event(0, NULL, 100000000000);
    double randomNumber = utils::randomD(lambdaT);

    for (iter = this->sampMap.begin(); iter != this->sampMap.end(); ++iter) {
        pos = iter->first;
        sample = iter->second;
        nl = sample->getNlineages();
        pSize = this->params->ms->getPopSize(pos, tEffective);
        if (pSize != 0)
            randomNumber -= nl * (nl - 1.) / 2. / pSize;
        if (randomNumber <= 0) {//the event!
            return new Event(1, sample, tEffective - this->timeSinceStart);
        }

    }
    fprintf(stdout, "Error: invalid coalescent event\n");
    throw 10;
}

Event* TreeSimulator::getNextMigEvent() {
    double rMax = this->migRejFunction(this->timeSinceStart);
    double tEvent;

    tEvent = utils::nhpp2((void*) this, rMax, &TreeSimulator::wrapper_migRejFunction, this->timeSinceStart, false);
    return this->whichMigEvent(tEvent);
}

double TreeSimulator::migRejFunction(const double t) {
    SampleList::const_iterator iter;
    Sample*sample;
    int nl;
    Coords pos;
    double pSize, lambdaT = 0.0;
    double mNorth, mSouth, mEast, mWest;


    for (iter = this->sampMap.begin(); iter != this->sampMap.end(); ++iter) {
        pos = iter->first;
        sample = iter->second;
        nl = sample->getNlineages();
        mNorth = this->params->ms->getMigrationRate(0, pos, t);
        mSouth = this->params->ms->getMigrationRate(1, pos, t);
        mEast = this->params->ms->getMigrationRate(2, pos, t);
        mWest = this->params->ms->getMigrationRate(3, pos, t);
        pSize = this->params->ms->getPopSize(pos, t);
        if (pSize > 0)
            lambdaT += nl * (mNorth + mSouth + mEast + mWest) / pSize;
    }
    return lambdaT;
}

Event* TreeSimulator::whichMigEvent(double t) {
    SampleList::const_iterator iter;
    Sample*sample;
    int nl;
    Coords pos;
    double lambdaT = 0.0;
    double mNorth, mSouth, mEast, mWest, pSize;
    Event* ev;
    double tEffective = t + this->timeSinceStart;

    //this is stupid, cause I am doing the same thing essentially twice
    for (iter = this->sampMap.begin(); iter != this->sampMap.end(); ++iter) {
        pos = iter->first;
        sample = iter->second;
        nl = sample->getNlineages();
        mNorth = this->params->ms->getMigrationRate(0, pos, tEffective);
        mSouth = this->params->ms->getMigrationRate(1, pos, tEffective);
        mEast = this->params->ms->getMigrationRate(2, pos, tEffective);
        mWest = this->params->ms->getMigrationRate(3, pos, tEffective);
        pSize = this->params->ms->getPopSize(pos, tEffective);
        if (pSize > 0)
            lambdaT += nl * (mNorth + mSouth + mEast + mWest) / pSize;
    }
    if (lambdaT < 0.000000001) return new Event(0, NULL, 100000000000);
    double randomNumber = utils::randomD(lambdaT);

    for (iter = this->sampMap.begin(); iter != this->sampMap.end(); ++iter) {
        pos = iter->first;
        sample = iter->second;
        nl = sample->getNlineages();

        pSize = this->params->ms->getPopSize(pos, tEffective);
        mNorth = this->params->ms->getMigrationRate(0, pos, tEffective);
        mSouth = this->params->ms->getMigrationRate(1, pos, tEffective);
        mEast = this->params->ms->getMigrationRate(2, pos, tEffective);
        mWest = this->params->ms->getMigrationRate(3, pos, tEffective);
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

void TreeSimulator::terminate() {
    cerr << "terminate NYI" << endl;
}

void TreeSimulator::addMigrationEvent(Event* ev) {
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
    Coords newCoords = Coords(newX, newY);
    if (this->sampMap.count(newCoords) == 1) {
        Sample* newSample = this->sampMap[newCoords];
        newSample->addLineage(migLineage);
    } else {
        vector<Lineage*> newLineages;
        newLineages.push_back(migLineage);
        Sample* newSample = new Sample(newX, newY, newLineages);
        this->sampMap.insert(pair<Coords, Sample*>(newCoords, newSample));
        this->nSamples += 1;
    }
    this->timeSinceLastCoalEvent += ev->time;

    if (curSample->getNlineages() == 0) {
        this->removeSample(ev->sample->getX(), ev->sample->getY());
    }
    delete ev;
}

void TreeSimulator::removeSample(int x, int y) {
    Coords pos = Coords(x, y);
    //int pos = this->params->ms->coords2d1d(x, y);
    Sample* a = this->sampMap[pos];
    delete a;
    this->sampMap.erase(pos);
    this->nSamples -= 1;
}

void TreeSimulator::addCoalEvent(Event* ev) {
    this->timeSinceStart += ev->time;
    Sample * curSample = ev->sample;
    int nChildren = curSample->merge2randomLineages(this->timeSinceStart);

    this->nLineages--;
    this->timeSinceLastCoalEvent = 0;
    if (Parameters::outputCoal) {
        coalEvent ce;
        ce.t = ev->time + this->timeSinceStart;
        ce.x = curSample->getX();
        ce.y = curSample->getY();
        ce.nDescendants = nChildren;

        this->coalEvents.push_back(ce);
    }
    delete ev;

}

void TreeSimulator::addExpansionEvent(ExpansionEvent* ev) {

    Coords pos = ev->first;
    //int* arr = this->params->ms->coords1d2d(pos);
    int x = pos.first;
    int y = pos.second;
    //delete[] arr;
    if (Parameters::verbose > 499) {
        cout << "[" << ev->second << "]:Expansion in deme (" << x << "," << y << ")" << endl;
    }
    if (Parameters::outputCoal) {
        coalEvent ce;
        ce.t = ev->second;
        ce.x = x;
        ce.y = y;
        ce.nDescendants = -1;

        this->coalEvents.push_back(ce);
    }

    int k = this->params->ms->getExpansionK(pos);
    double pDir[4], mRate[4], rTot = 0, randomNumber;
    Lineage * lineage;


    this->timeSinceStart = ev->second;
    if (this->sampMap.count(ev->first) == 1) {
        Sample * sample = this->sampMap[ev->first];

        //if there is a founding propagule size set, check if we need to perform
        // additional coalescent events

        if (k > 0 && sample->getNlineages() > 1) {
            int nAncLineages = 1;
            double logPNAncLineages; //pNAncLineages, 
            randomNumber = utils::randomD(1);

            //if there are more lineages than k, reduce to k...
            if (sample->getNlineages() > k) {
                int nCoal = sample->getNlineages() - k;
                for (int i = 0; i < nCoal; i++) {
                    this->addCoalEvent(new Event(1, sample, 0));
                }
            } else {
                while (nAncLineages <= k) {
                    logPNAncLineages = log(utils::getStirlingNumberD(sample->getNlineages(),
                            nAncLineages));
                    //pNAncLineages = this->ut->getStirlingNumber(sample->getNlineages(),
                    logPNAncLineages += utils::logFallingFactorial(k, nAncLineages);
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
        int h = this->params->ms->getHeight();
        //get population sizes/migration rates of surrounding demes:
        pDir[0] = this->params->ms->getPopSize(Coords(pos.first, pos.second + 1), timeSinceStart);
        pDir[1] = this->params->ms->getPopSize(Coords(pos.first, pos.second - 1), timeSinceStart);
        pDir[2] = this->params->ms->getPopSize(Coords(pos.first + 1, pos.second), timeSinceStart);
        pDir[3] = this->params->ms->getPopSize(Coords(pos.first - 1, pos.second), timeSinceStart);


        mRate[0] = this->params->ms->getMigrationRate(0, pos, timeSinceStart);
        mRate[1] = this->params->ms->getMigrationRate(1, pos, timeSinceStart);
        mRate[2] = this->params->ms->getMigrationRate(2, pos, timeSinceStart);
        mRate[3] = this->params->ms->getMigrationRate(3, pos, timeSinceStart);

        for (int i = 0; i < 4; i++) {
            rTot += pDir[i] * mRate[i];
        }

        int l = 0;

        //propagule pool: send all migrants to a single adjacient deme

        if (Parameters::mPropagulePool) {
            int direction;
            randomNumber = utils::randomD(rTot);
            for (int i = 0; i < 4; i++) {
                randomNumber -= pDir[i] * mRate[i];
                if (randomNumber < 0) {
                    if (Parameters::verbose > 499) {
                        cout << "Deme(" << x << "," << y << "); ppMigration: direction" << i << endl;
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
                randomNumber = utils::randomD(rTot);
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

string TreeSimulator::toString() {
    char buffer[100];
    stringstream ss;
    sprintf(buffer, "SSSimulator on a (%d x %d) grid\n",
            this->params->ms->getWidth()
            , this->params->ms->getHeight());
    ss << buffer;
    sprintf(buffer, "total time is %f. (time since last coal is %f\n", this->timeSinceStart, this->timeSinceLastCoalEvent);
    ss << buffer;
    sprintf(buffer, "contains %d lineages in %d samples: \n", this->nLineages, this->nSamples);
    ss << buffer;
    SampleList::const_iterator iter;
    Sample* sample;
    for (iter = this->sampMap.begin(); iter != this->sampMap.end(); ++iter) {
        sample = iter->second;
        ss << sample->toString();
    }
    return ss.str();
}
