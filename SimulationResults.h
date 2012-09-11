/* 
 * File:   SimulationResults.h
 * Author: peterb
 *
 * Created on August 13, 2012, 3:36 PM
 */

#ifndef SIMULATIONRESULTS_H
#define	SIMULATIONRESULTS_H
#include "SFS.h"
#include "FreqTable.h"
#include "Parameters.h"
#include <vector>
#include "math.h"
class Parameters;
using namespace std;

class SimulationResults {
public:
    SimulationResults();
    SimulationResults(const SimulationResults& orig);
    virtual ~SimulationResults();
    void initialize(Parameters* params, int sampleSize, int* sampleSizes);
    vector<SFS*> sumSFS, sumSFS1d;
    vector<SFS**> sfsSingleTrees;
    FreqTable* ft;
    double* fst,*psi,*deltaH;
    double** vFst,**vPsi,**vDeltaH;
    int nSamples,nReplicates;
    void printStats();
    void printStatsWithJK();
    double* jackknifeFST();
    double* jackknifePsi();
    double* jackknifeDeltaH();
    
    static double doJackknife(int n, double* v);
private:

};

#endif	/* SIMULATIONRESULTS_H */

