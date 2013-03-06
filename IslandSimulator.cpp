/* 
 * File:   IslandSimulator.cpp
 * Author: peterb
 * 
 * Created on December 12, 2012, 4:53 PM
 */

#include "IslandSimulator.h"

IslandSimulator::IslandSimulator() {
}


//this is the default way this should be initialized

IslandSimulator::IslandSimulator(TreeSimulator* ts) {
    if (Parameters::verbose > 299) {
        cout << "starting island model ";
    }
    this->samples = ts->finalSamples;
    if (Parameters::verbose > 299) {
        cout << "with " << this->samples.size() << " islands ";
    }
    this->timeSinceStart = ts->timeSinceStart;
    this->timeSinceLastCoalEvent = ts->timeSinceLastCoalEvent;
    this->coalEvents = ts->coalEvents;
    this->nMigrationEvents = ts->nMigrationEvents;

    this->nLineages = 0;
    for (int i = 0; i<this->samples.size(); ++i) {
        this->nLineages += this->samples[i]->getNlineages();
        this->popSizes.push_back(1);
    }
    this->mRate =100;
        if (Parameters::verbose > 299) {
        cout << "and " << this->nLineages << " lineages" << endl;
    }
}

IslandSimulator::IslandSimulator(const IslandSimulator& orig) {
}

IslandSimulator::~IslandSimulator() {
}


//let samp.x be the deme and samp.y be -1

void IslandSimulator::addEvent(Event* ev) {
    if (ev->type == 1) this->addCoalEvent(ev);
    else if (ev->type > 1) this->addMigrationEvent(ev);
    else if (ev->type == 0) this->terminate();
}

void IslandSimulator::addCoalEvent(Event* ev) {
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

void IslandSimulator::addMigrationEvent(Event* ev) {
    //cout <<"mig"<< ev->toString()<<endl;
    this->nMigrationEvents++;
    this->timeSinceStart += ev->time;
    Sample * curSample = ev->sample;
    Sample * targetSample = this->samples[ev->type - 2];
    Lineage * migLineage = curSample->getRandomLineageForMigration();
    targetSample->addLineage(migLineage);
    this->timeSinceLastCoalEvent += ev->time;
    delete ev;
}

double IslandSimulator::coalRejFunction(const double t) {
    int nl;
    double pSize, lambdaT = 0.0;

    for (int i = 0; i<this->samples.size(); ++i) {
        nl = this->samples[i]->getNlineages();
        pSize = this->popSizes[i];

        if (pSize != 0)
            lambdaT += nl * (nl - 1.) / 2. / pSize;
    }
    return lambdaT;
}

double IslandSimulator::migRejFunction(const double t) {

    Sample* sample;
    int nl;
    double pSize, lambdaT = 0.0;

    for (int i = 0; i<this->samples.size(); ++i) {
        nl = this->samples[i]->getNlineages();
        pSize = this->popSizes[i];
        lambdaT += nl * (this->samples.size() - 1) / pSize;
    }
    return lambdaT;
}

Event* IslandSimulator::whichCoalEvent(double t) {

    Sample* sample;
    int nl;
    double lambdaT = 0.0;
    double pSize, tEffective = t + this->timeSinceStart;

    if (t > 100000000000) {
        return new Event();
    }
    //this is stupid, cause I am doing the same thing essentially twice
    for (int i = 0; i<this->samples.size(); ++i) {
        sample = this->samples[i];
        nl = sample->getNlineages();
        pSize = this->popSizes[i];
        lambdaT += nl * (nl - 1.) / 2. / pSize;
    }
    if (lambdaT < 0.000000001) return new Event(0, NULL, 100000000000);
    double randomNumber = utils::randomD(lambdaT);

    for (int i = 0; i<this->samples.size(); ++i) {
        sample = this->samples[i];
        nl = sample->getNlineages();
        pSize = this->popSizes[i];
        if (pSize != 0)
            randomNumber -= nl * (nl - 1.) / 2. / pSize;
        if (randomNumber <= 0) {//the event!
            return new Event(1, sample, tEffective - this->timeSinceStart);
        }

    }
    fprintf(stdout, "Error: invalid coalescent event\n");
    throw 10;
}

Event* IslandSimulator::whichMigEvent(double t) {
    Sample* sample;
    int nl;
    double lambdaT = 0.0;
    double pSize, tEffective = t + this->timeSinceStart;

    //this is stupid, cause I am doing the same thing essentially twice
    for (int i = 0; i<this->samples.size(); ++i) {
        sample = this->samples[i];
        nl = sample->getNlineages();
        pSize = this->popSizes[i];
        if (pSize > 0)
            lambdaT += nl * ((this->samples.size() - 1) * this->mRate) / pSize;
    }
    if (lambdaT < 0.000000001) return new Event(0, NULL, 100000000000);
    double randomNumber = utils::randomD(lambdaT);

    for (int i = 0; i<this->samples.size(); ++i) {
        sample = this->samples[i];
        nl = sample->getNlineages();
        pSize = this->popSizes[i];

        for (int j = 0; j<this->samples.size(); ++j) {
            if (i == j)
                continue;
            randomNumber -= nl * this->mRate / pSize;
            if (randomNumber <= 0) { //the event!
                return new Event(j + 2, sample, t);
            }
        }
    }

    fprintf(stderr, "Error: invalid migration rate");
    throw 10;
}

Event* IslandSimulator::getNextEvent() {
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


    if (evCoal->time > evMig->time) {
        delete evCoal;
        return evMig;
    } else {
        delete evMig;
        return evCoal;
    }
}

Event* IslandSimulator::getNextCoalEvent() {
    double tEvent;
    double rMax = this->coalRejFunction(this->timeSinceStart);
    if (Parameters::verbose > 1999)
        cout << "getNextCoalEvent::rMax:" << rMax << endl;

    tEvent = utils::nhpp2((void*) this, rMax, &IslandSimulator::wrapper_coalRejFunction, this->timeSinceStart, false);
    if (tEvent == -1.0) {
        return new Event(0, 0, 1e15);
    }
    if (Parameters::verbose > 1999)
        cout << "getNextCoalEvent::tEvent:" << tEvent << endl;
    return this->whichCoalEvent(tEvent);
}

Event* IslandSimulator::getNextMigEvent() {
    double rMax = this->migRejFunction(this->timeSinceStart);
    double tEvent;

    tEvent = utils::nhpp2((void*) this, rMax, &IslandSimulator::wrapper_migRejFunction, this->timeSinceStart, false);
    if (tEvent == -1.0) {
        return new Event(0, 0, 1e15);
    }
    return this->whichMigEvent(tEvent);
}

LCV IslandSimulator::runIsland() {

    Event* ev;
    int evc = 0;
    while (this->nLineages>1) {
        //cout <<"did event"<<this->nLineages<<endl;
        ev = this->getNextEvent();


        if (Parameters::verbose > 499) cout << "IM:[" << this->timeSinceStart << "]:" << ev->toString() << endl;
        if (ev->type == 0) {
            delete ev;
            break;
        }
        evc++;

        this->addEvent(ev);
    }


    if (this->nLineages != 1) {
        cout << this->toString() << endl;
        cerr << "error. sampMap size is " << this->sampMap.size() << ",should be 1";
        throw 10;
    }



    Lineage* l;
    for (int i = 0; i<this->samples.size(); ++i) {
        if (this->samples[i]->getNlineages() > 0) {
            l = this->samples[i]->getRandomLineageForMigration();
            break;
        }
    }

    LCV lcv;
    //lcv.c = NULL;
    lcv.l = l;
    if (Parameters::outputCoal) {
        lcv.c = this->coalEvents;
    }
    return lcv;
}
