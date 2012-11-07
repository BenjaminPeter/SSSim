/* 
 * File:   Simulator.h
 * Author: peterb
 *
 * Created on June 1, 2012, 10:53 AM
 */

#ifndef SIMULATOR_H
#define	SIMULATOR_H
#include <boost/unordered_map.hpp>
#include <boost/thread.hpp>
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
#include "TreeSimulator.h"
#include <boost/bind.hpp>

#include <stdio.h>

using namespace std;

class Parameters;
class SimulationResults;
class TreeSimulator;
class Simulator {
public:
    int nLineages,nSamples;
    boost::unordered_map<int,Sample*> sampMap;
    MigrationScheme * migrationScheme;
    utils* ut;
    static SequenceSimulator* seqSim;
    static int replicate;
    
    //constructors
    Simulator(long seed=0);
    Simulator(const Simulator& orig);
    virtual ~Simulator();
             
    
    static boost::mutex ftMutex,sfsMutex,ft2Mutex;
    
    //running the simulation
    SimulationResults* doSimulations(Parameters* params);
    static void getNewGeneTree(Parameters* params, SimulationResults* res,int id=0);

    
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
    


private:       

};

#endif	/* SIMULATOR_H */

