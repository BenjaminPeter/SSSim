/* 
 * File:   SimulationResults.cpp
 * Author: peterb
 * 
 * Created on August 13, 2012, 3:36 PM
 */

#include "SimulationResults.h"
using namespace std;

SimulationResults::SimulationResults() {
}

SimulationResults::SimulationResults(const SimulationResults& orig) {
}

SimulationResults::~SimulationResults() {
}

void SimulationResults::initialize(Parameters* params, int nSamples, int* sampleSizes, bool singleTrees) {
    int pos = 0;
    this->nSamples = nSamples;
    this->theta = params->ss->getTheta();
    this->nSNP=params->nSNP;
    //initialize
    for (int i = 0; i < nSamples - 1; i++) {
        for (int j = i + 1; j < nSamples; j++) {
            this->sumSFS.push_back(new SFS(sampleSizes[i], sampleSizes[j]));
//            if (singleTrees) {
//                this->sfsSingleTrees.push_back(new SFS*[params->nReplicates]);
//                for (int r = 0; r < params->nReplicates; r++) {
//                    this->sfsSingleTrees.back()[r] = new SFS(sampleSizes[i], sampleSizes[j]);
//                }
//            }
            pos++;
        }
    }
    for (int i = 0; i < nSamples; i++) {
        this->sumSFS1d.push_back(new SFS(sampleSizes[i]));
    }

    this->fst = new double[nSamples * (nSamples - 1) / 2];
    this->psi = new double[nSamples * (nSamples - 1) / 2];
    this->deltaH = new double[nSamples * (nSamples - 1) / 2];

    this->vFst = new double*[nSamples * (nSamples - 1) / 2];
    this->vPsi = new double*[nSamples * (nSamples - 1) / 2];
    this->vDeltaH = new double*[nSamples * (nSamples - 1) / 2];
    for (int i = 0; i < nSamples * (nSamples - 1) / 2; i++) {
        this->vFst[i] = new double[params->nReplicates];
        this->vPsi[i] = new double[params->nReplicates];
        this->vDeltaH[i] = new double[params->nReplicates];
        for (int j = 0; j < params->nReplicates; j++) {
            this->vFst[i][j] = 0;
            this->vPsi[i][j] = 0;
            this->vDeltaH[i][j] = 0;
        }
    }
    this->nReplicates = params->nReplicates;


}

string SimulationResults::printStats() {
    stringstream f;
    int pos = 0;
    for (int i = 0; i < nSamples - 1; i++) {
        for (int j = i + 1; j < nSamples; j++) {
            f << i << "\t" << j << "\t" << this->fst[pos] << "\t" << this->psi[pos] << "\t" << this->deltaH[pos] << endl;
            pos++;
        }
    }
    return f.str();

}

string SimulationResults::printStatsWithJK() {
    stringstream f;
    int pos = 0;
    double* bFST, *bPsi, *bDeltaH;
    bFST = this->jackknifeFST();
    bPsi = this->jackknifePsi();
    bDeltaH = this->jackknifeDeltaH();

    for (int i = 0; i < nSamples - 1; i++) {
        for (int j = i + 1; j < nSamples; j++) {
            f << i << "\t" << j << "\t";
            f << this->fst[pos] << "\t" << bFST[pos] << "\t";
            f << this->psi[pos] << "\t" << bPsi[pos] << "\t";
            f << this->deltaH[pos] << "\t" << bDeltaH[pos] << "\n";
            pos++;
        }
    }
    return f.str();

}

double SimulationResults::doJackknife(int n, double* v) {
    if (n <= 1) return 0.0;
    double theta_dot = 0.0;
    double se_jack = 0.0;

    for (int i = 0; i < n; i++) {
        //cout <<v[i] <<endl;
        theta_dot += v[i];
    }
    theta_dot /= ((float) n);
    //cout  << "td::"<<theta_dot <<endl;
    for (int i = 0; i < n; i++) {
        //cout <<v[i]<<"\t"<<theta_dot<<"\t"<<(theta_dot-v[i])*(theta_dot-v[i])<<endl;
        se_jack += ((theta_dot - v[i])*(theta_dot - v[i]));
    }
    se_jack *= (n - 1.0) / ((float) n);
    se_jack = sqrt(se_jack);
    //cout <<"-------\t"<<se_jack<<endl;
    return se_jack;

}

double* SimulationResults::jackknifeFST() {
    int nPairs = this->nSamples * (this->nSamples - 1) / 2;
    double* jk = new double[nPairs];
    for (int i = 0; i < nPairs; i++) {
        jk[i] = this->doJackknife(this->nReplicates, this->vFst[i]);
    }
    return jk;
}

double* SimulationResults::jackknifePsi() {
    int nPairs = this->nSamples * (this->nSamples - 1) / 2;

    double* jk = new double[nPairs];
    for (int i = 0; i < nPairs; i++) {
        jk[i] = this->doJackknife(this->nReplicates, this->vPsi[i]);
    }
    return jk;
}

double* SimulationResults::jackknifeDeltaH() {
    int nPairs = this->nSamples * (this->nSamples - 1) / 2;

    double* jk = new double[nPairs];
    for (int i = 0; i < nPairs; i++) {
        jk[i] = this->doJackknife(this->nReplicates, this->vDeltaH[i]);
    }
    return jk;
}

/*
 * this function simulates a set of SNP according to theta and the total tree 
 * length from the frequency table
 */
vector<vector<int>* >* SimulationResults::simulateSNPTheta(bool shared) {
    if (theta<0){
        cerr<< "theta not set" <<endl;
        throw 10;
    }
    if (shared){
        return this->ftShared->drawSNP(this->theta * this->ftShared->getTTot());
    }else{
        return this->ft->drawSNP(this->theta * this->ft->getTTot());
    }
}

/*
 * this function simulates a set of N SNP 
 */
vector<vector<int>* >* SimulationResults::simulateSNPN(bool shared) {
    
    if (shared){
        return this->ftShared->drawSNP(this->nSNP);
        cout <<"SimulationResults::simulateSNPN:drawing shared"<<endl;
    }else{
        return this->ft->drawSNP(this->nSNP);
    }
}
