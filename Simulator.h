/* 
 * File:   Simulator.h
 * Author: peterb
 *
 * Created on June 1, 2012, 10:53 AM
 */

#ifndef SIMULATOR_H
#define	SIMULATOR_H
#include <boost/unordered_map.hpp>
#include <string>
#include <vector>
#include <sstream>
#include <math.h>
#include "SFS.h"
//#include "Coords.h"
#include "Sample.h"
#include "Event.h"
#include "Lineage.h"
#include "InternalLineage.h"
#include "TerminalLineage.h"
#include "LineageTemplate.h"
#include "utils.h"
#include "MigrationScheme.h"
#include "SequenceSimulator.h"
#include "Parameters.h"
#include "StatCalculator.h"
#include "SimulationResults.h"
#include <stdio.h>

using namespace std;

class Parameters;
class SimulationResults;
class Simulator {
public:
    int nLineages,nSamples;
    boost::unordered_map<int,Sample*> sampMap;
    MigrationScheme * migrationScheme;
    utils* ut;
    SequenceSimulator* seqSim;
    
    //constructors
    Simulator(long seed=0);
    Simulator(const Simulator& orig);
    virtual ~Simulator();
             
    
    //samples
    void removeSample(int x, int y);
    
    
    
    

    
    
    //generating time for next event
    Event* getNextCoalEvent();
    Event* getNextMigEvent();
    Event* getNextCoalMigEvent();
    Event* getNextEvent3();
    //Event* getNextEvent2();
    //Event* getNextEvent();
    
    
    //adding events
    void addEvent(Event* ev);
    void addCoalEvent(Event* ev);
    void addMigrationEvent(Event* ev);
    void addExpansionEvent(pair<double,int>* ev);
    void terminate();
    
    
    
    //running the simulation
    SimulationResults* doSimulations(Parameters* params);
    Lineage* getNewGeneTree();

    
    //set up
    void addSequenceSimulator(SequenceSimulator* ss);       
    void setMigrationScheme(MigrationScheme* ms){
        this->migrationScheme=ms;
        //this->expansionEvents=ms->getExpansionEvents();
        int k=this->migrationScheme->getExpansionK();
        if (k>0){
            this->ut->setupStirlingNumberTableD(k);
            this->ut->setupStirlingNumberTable(k);
            
        }
        //cout << "stirling number"<<this->ut->getStirlingNumber(5,2)<<endl;
    }   
    bool migrationSchemeReady(){
        return this->migrationScheme->isInitialized();
    }
    
    
    string toString();

private:       
    double timeSinceLastCoalEvent;
    double timeSinceStart;
    int nMigrationEvents;
    bool propagulePoolMigration;
    
    
    vector<pair<double,int> >* expansionEvents;

    
    //rejection sampling for nhpp
    double coalRejFunction(double t);
    double migRejFunction(double t);
    //double coalMigRejFunction(double t);
    
    //what exactly happens?
    Event* whichCoalEvent(double t);
    Event* whichMigEvent(double t);
    //Event* whichCoalMigEvent(double t);
    static double wrapper_coalRejFunction(void* myself_ptr,double t){
        Simulator* myself=(Simulator*)myself_ptr;
        return myself->coalRejFunction(t);
    }
    static double wrapper_migRejFunction(void* myself_ptr,double t){
        Simulator* myself=(Simulator*)myself_ptr;
        return myself->migRejFunction(t);
    }
    
    /*static double wrapper_coalMigRejFunction(void* myself_ptr,double t){
        Simulator* myself=(Simulator*)myself_ptr;
        return myself->coalMigRejFunction(t);
    }**/
    void copySampStartToSamp();

};

#endif	/* SIMULATOR_H */

