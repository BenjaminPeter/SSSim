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


using namespace std;

class Parameters;
class SimulationResults;
class Simulator {
public:
    int nLineages,nSamples,nLineagesStart,nSamplesStart;
    boost::unordered_map<int,Sample*> sampMap, sampMapStart;
    bool verbose;
    MigrationScheme * migrationScheme;
    utils* ut;
    
    Simulator(long seed=0);
    Simulator(const Simulator& orig);
    virtual ~Simulator();
    void vPrint(string s);
    //void setWidth(int width);
    //void setHeight(int height);
    void addSampleStart(int * pos,int nNewLineages,bool outputLoci=false, stringstream* ssOutputLoci=NULL);
    void addSampleStart(int x, int y, int nNewLineages,
            bool outputLoci=false, stringstream* ssOutputLoci=NULL);
    void removeSample(int x, int y);
    void addSequenceSimulator(SequenceSimulator* ss);
    
    Event* getNextCoalEvent();
    Event* getNextMigEvent();
    Event* getNextCoalMigEvent();
    Event* getNextEvent3();
    //Event* getNextEvent4();
    Event* getNextEvent2();
    Event* getNextEvent();
    
    void addEvent(Event* ev);
    void addCoalEvent(Event* ev);
    void addMigrationEvent(Event* ev);
    void addExpansionEvent(pair<double,int>* ev);
    void terminate();
    SimulationResults* doSimulations(Parameters* params);
    Lineage* getNewGeneTree();

    string toString();
    
        
    void setMigrationScheme(MigrationScheme* ms){
        this->migrationScheme=ms;
        //this->expansionEvents=ms->getExpansionEvents();
        int k=this->migrationScheme->getExpansionK();
        if (k>0){
            this->ut->setupStirlingNumberTableD(k);
            this->ut->setupStirlingNumberTable(k);
            
        }
        //cout << "stirling number"<<this->ut->getStirlingNumber(5,2)<<endl;
    };
    
    bool migrationSchemeReady(){
        return this->migrationScheme->isInitialized();
    }
    
    
    SequenceSimulator* seqSim;
    
    //int carCap;
private:       
    //boost::unordered_map<int,double> coalEvents;
    //boost::unordered_map<pair<int,int>,double> migEvents;
    //double totRateCoalEvents,totRateMigEvents;

    //double*** mRates;
    //int width,height;
    double timeSinceLastCoalEvent;
    double timeSinceStart;
    int nMigrationEvents;
    
    vector<pair<double,int> >* expansionEvents;
    
    double coalRejFunction(double t);
    double migRejFunction(double t);
    //double coalMigRejFunction(double t);
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
    bool propagulePoolMigration;
};

#endif	/* SIMULATOR_H */

