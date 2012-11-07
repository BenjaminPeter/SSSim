/* 
 * File:   TreeSimulator.h
 * Author: peterb
 *
 * Created on November 6, 2012, 5:21 PM
 */

#ifndef TREESIMULATOR_H
#define	TREESIMULATOR_H

#include "Parameters.h"
#include "SimulationResults.h"
#include "Lineage.h"
#include "boost/unordered_map.hpp"
#include "Event.h"

class Parameters;
class TreeSimulator {
public:
    TreeSimulator(Parameters* params);
    TreeSimulator(const TreeSimulator& orig);
    virtual ~TreeSimulator();
    Lineage* run();
    string toString();
    
    
private:
    Parameters* params;
    void copySampStartToSamp();
    boost::unordered_map<int, Sample*> sampMap;
    int nSamples, nLineages;
    double timeSinceLastCoalEvent, timeSinceStart;
    int nMigrationEvents;
    vector<pair<double, int> >* expansionEvents;

    //next event
    Event* getNextEvent();
    void addEvent(Event* ev);
    Event* getNextCoalEvent();
    Event* getNextMigEvent();

    
    void removeSample(int x, int y);
    //adding events
    void addExpansionEvent(pair<double, int>* ev);
    void addCoalEvent(Event* ev);
    void addMigrationEvent(Event* ev);
    void terminate();

    double coalRejFunction(const double t);
    double migRejFunction(const double t);

    Event* whichCoalEvent(double t);
    Event* whichMigEvent(double t);

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
