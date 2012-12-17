/* 
 * File:   IslandSimulator.h
 * Author: peterb
 * a simple island model simulator
 * Created on December 12, 2012, 4:53 PM
 */

#ifndef ISLANDSIMULATOR_H
#define	ISLANDSIMULATOR_H
#include "Sample.h"
#include "TreeSimulator.h"
#include "Event.h"

using namespace std;

class IslandSimulator : public TreeSimulator {
public:
    IslandSimulator();
    IslandSimulator(TreeSimulator* ts);

    IslandSimulator(const IslandSimulator& orig);
    virtual ~IslandSimulator();
    double mRate;
    vector<int> popSizes;
    vector<Sample*> samples;
    LCV runIsland();
    
private:
    //next event
    virtual Event* getNextEvent();
    virtual Event* getNextCoalEvent();
    virtual Event* getNextMigEvent();

    //adding events
    virtual void addEvent(Event* ev);
    virtual void addCoalEvent(Event* ev);
    virtual void addMigrationEvent(Event* ev);

    virtual double coalRejFunction(const double t);
    virtual double migRejFunction(const double t);

    virtual Event* whichCoalEvent(double t);
    virtual Event* whichMigEvent(double t);
    
};

#endif	/* ISLANDSIMULATOR_H */

