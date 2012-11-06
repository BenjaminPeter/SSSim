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
#include <sstream>
#include <string>
#include "math.h"
class Parameters;
using namespace std;

class SimulationResults {
public:
    SimulationResults();
    SimulationResults(const SimulationResults& orig);
    virtual ~SimulationResults();
    void initialize(Parameters* params, int sampleSize, int* sampleSizes, bool singleTrees=false);
    vector<SFS*> sumSFS, sumSFS1d;
    
    //vector<SFS**> sfsSingleTrees;
    FreqTable* ft,*ftShared;
    double* fst,*psi,*deltaH;
    double** vFst,**vPsi,**vDeltaH;
    int nSamples,nReplicates,nSNP;
    string printStats();
    string printStatsWithJK();
    double* jackknifeFST();
    double* jackknifePsi();
    double* jackknifeDeltaH();
    SNPTable* simulateSNPTheta(bool shared=false);
    SNPTable* simulateSNPN(bool shared=false);
    static double doJackknife(int n, double* v);
private:
    double theta;

};

#endif	/* SIMULATIONRESULTS_H */

