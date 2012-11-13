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
#include <iostream>
#include <fstream>
#include <boost/unordered_map.hpp>
#include <boost/thread.hpp>
#include "Sample.h"
#include "MigrationScheme.h"
#include "IsolationByDistance.h"
#include "IsolationByDistanceExpansion.h"
#include "SEExpansion.h"
#include "SEExpansionDiffK.h"
#include "SequenceSimulator.h"
#include "Lineage.h"
#include "LineageTemplate.h"
#include "Barrier.h"
#include "IsolationByDistanceBarrier.h"
#include "SEExpansionBarrier.h"

typedef boost::unordered_map<Coords, Sample*> SampleList;

class SequenceSimulator;
class Parameters {
public:
    Parameters(int argc, char* argv[]);
    Parameters();
    Parameters(const Parameters& orig);
    virtual ~Parameters();
    static SampleList sampMapStart;
    

    MigrationScheme* ms; //defines how migration is handled
    SequenceSimulator* ss; //simulates a sequence from a tree
    //Simulator* sim; //the simulator class, that does the simulating
    std::vector<int*> samples; //each entry has three ints, being the 
    std::vector<Barrier*> barriers; // barriers in the habitat
    bool calcDeltaH, calcFST, calcPsi;
    //(x,y) coords and the number of lineages samples
    int nReplicates;//how many trees should be sampled?    
    static int* sampleSizes; //calculated from samples, just the sample sizes.
    string outputPrefix;
    bool outputSFS,outputFT,outputSNP,outputSNPShared,outputStats,outputStatsJK;
    bool outputTree,outputCommand;
    bool outputSNPStats, outputSNPSharedStats;
    int bootstrapSNPStats, bootstrapSNPSharedStats;
    static int verbose; //verbosity level:
    /*for now
     * 0: no output
     * 1: very basic
     * 1000: highly messy low level debugging
     
     */
    static bool mPropagulePool;
    bool outputLoci;
    int nSNP; //how many SNP should be sampled
    
    void addSampleStart(int * pos,int nNewLineages,bool outputLoci=false, stringstream* ssOutputLoci=NULL);
    void addSampleStart(int x, int y, int nNewLineages,
            bool outputLoci=false, stringstream* ssOutputLoci=NULL);
    static int nLineagesStart;
    static int nSamplesStart;
    static int nThreads;
    void printHelp();
    static int seed;
    static double theta;
    static bool memoryEfficient;
private:

};


#endif	/* PARSEARGS_H */

