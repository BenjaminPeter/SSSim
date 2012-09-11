/* 
 * File:   parseArgs.h
 * Author: peterb
 *
 * Created on August 13, 2012, 2:24 PM
 */

#ifndef PARSEARGS_H
#define	PARSEARGS_H
#include <string>
#include <vector>
#include <cstdlib>
#include "Simulator.h"
#include "MigrationScheme.h"
#include "IsolationByDistance.h"
#include "IsolationByDistanceExpansion.h"
#include "SEExpansion.h"
#include "SEExpansionDiffK.h"
class Simulator;
class Parameters {
public:
    Parameters(int argc, char* argv[]);
    Parameters();
    Parameters(const Parameters& orig);
    virtual ~Parameters();

    MigrationScheme* ms; //defines how migration is handled
    SequenceSimulator* ss; //simulates a sequence from a tree
    Simulator* sim; //the simulator class, that does the simulating
    std::vector<int*> samples; //each entry has three ints, being the 
    bool calcDeltaH, calcFST, calcPsi;
    //(x,y) coords and the number of lineages samples
    int nReplicates;//how many trees should be sampled?    
    int* sampleSizes; //calculated from samples, just the sample sizes.
    string outputPrefix;
private:

};

#endif	/* PARSEARGS_H */
