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
#include <boost/bind.hpp>

//#include "Coords.h"
#include "utils.h"
#include "MigrationScheme.h"
#include "SEExpansion.h"
#include "SequenceSimulator.h"
#include "Parameters.h"
#include "SimulationResults.h"
#include "TreeSimulator.h"
#include "StatCalculator.h"


#include <stdio.h>

using namespace std;

class Parameters;
class Simulator {
public:
    int nLineages,nSamples;
    boost::unordered_map<int,Sample*> sampMap;
    MigrationScheme * migrationScheme;
    static SequenceSimulator* seqSim;
    static int replicate;
    
    //constructors
    Simulator();
    Simulator(const Simulator& orig);
    virtual ~Simulator();
             
    
    static boost::mutex ftMutex,sfsMutex,ft2Mutex,counterMutex;
    
    
    //running the simulation
    SimulationResults* doSimulations(Parameters* params);
    static void getNewGeneTree(Parameters* params, SimulationResults* res,int id=0);

    
    //set up
    void addSequenceSimulator(SequenceSimulator* ss);       
    void setMigrationScheme(MigrationScheme* ms){
        this->migrationScheme=ms;
        //this->expansionEvents=ms->getExpansionEvents();
        int k=Parameters::kMax;
        if (k>0){
            utils::setupStirlingNumberTableD(k);
            utils::setupStirlingNumberTable(k);
            
        }
        //cout << "stirling number"<<this->ut->getStirlingNumber(5,2)<<endl;
    }   
    bool migrationSchemeReady(){
        return this->migrationScheme->isInitialized();
    }

    static double avgNMigrationEvents;
    


private:       
    //writing coal output file
    static boost::mutex oCoalMutex;
    static ofstream fCoal;

};

#endif	/* SIMULATOR_H */

