/* 
 * File:   TreeSimulator.h
 * Author: peterb
 *
 * Created on November 6, 2012, 5:21 PM
 */

#ifndef TREESIMULATOR_H
#define	TREESIMULATOR_H

#include "boost/unordered_map.hpp"
#include "Parameters.h"
#include "SimulationResults.h"
#include "Lineage.h"
#include "Event.h"

class IslandSimulator;

struct coalEvent {
    double t;
    int x;
    int y;
    int nDescendants;
};

struct LCV {
    Lineage* l;
    vector<coalEvent> c;
};

class TreeSimulator {
public:
    TreeSimulator();
    TreeSimulator(Parameters* params);
    TreeSimulator(const TreeSimulator& orig);
    virtual ~TreeSimulator();
    LCV run(int id = 0);
    string toString();

    vector<Sample*> finalSamples; //final samples, assumed in island model
    double timeSinceLastCoalEvent, timeSinceStart;
    vector<coalEvent> coalEvents;
    int nMigrationEvents;


protected:
    Parameters* params;
    void copySampStartToSamp();
    SampleList sampMap;
    int nSamples, nLineages;
    vector<ExpansionEvent>* expansionEvents;

    //next event
    virtual Event* getNextEvent();
    virtual Event* getNextCoalEvent();
    virtual Event* getNextMigEvent();


    virtual void removeSample(int x, int y);
    //adding events
    virtual void addEvent(Event* ev);
    virtual void addExpansionEvent(ExpansionEvent* ev);
    virtual void addCoalEvent(Event* ev);
    virtual void addMigrationEvent(Event* ev);
    virtual void terminate();

    virtual double coalRejFunction(const double t);
    virtual double migRejFunction(const double t);

    virtual Event* whichCoalEvent(double t);
    virtual Event* whichMigEvent(double t);

    static double wrapper_coalRejFunction(void* myself_ptr, double t) {
        TreeSimulator* myself = (TreeSimulator*) myself_ptr;
        return myself->coalRejFunction(t);
    }

    static double wrapper_migRejFunction(void* myself_ptr, double t) {
        TreeSimulator* myself = (TreeSimulator*) myself_ptr;
        return myself->migRejFunction(t);
    }
};

#endif	/* TREESIMULATOR_H */

