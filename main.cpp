/* 
 * File:   main.cpp
 * Author: peterb
 *
 * Created on June 1, 2012, 10:51 AM
 */

#include "SEExpansion.h"


#include <cstdlib>

#include "Simulator.h"
#include "IsolationByDistance.h"
#include "SEExpansion.h"
#include "SequenceSimulator.h"
#include "StatCalculator.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "Sample.h"
#include "IsolationByDistanceExpansion.h"
#include "Parameters.h"
#include "SimulationResults.h"
#include "BootstrapResampler.h"
#include <string>

#define SEED 0;

using namespace std;

IsolationByDistance * ibdTest() {
    IsolationByDistance * ibd = new IsolationByDistance();
    ibd->setCarCapUniform(1.);
    ibd->setMigrationRatesUniform(5, 5, 5, 5);
    ibd->setExpansionK(20);
    ibd->setWidth(3);
    ibd->setHeight(3);
    return ibd;
}

SEExpansion * seeTest() {
    SEExpansion * see = new SEExpansion();
    see->setCarCapUniform(1.);
    see->setMigrationRatesUniform(5, 5, 5, 5);
    see->setStartPos(0, 0);
    see->setTLag(0.5);
    see->setTStart(3.);
    see->setExpansionK(0);
    see->setWidth(1);
    see->setHeight(5);
    see->setupArrivalTimes();

    return see;
}

/*
 * 
 */
int main(int argc, char* argv[]) {
    char s[100];
    Parameters* params = new Parameters(argc, argv);




    Simulator* sim = params->sim;

    if (!sim->migrationSchemeReady()) {
        cout << "width is" << sim->migrationScheme->getWidth() << endl;
        cerr << "mig scheme not properly initialized" << endl;
        return 1;
    }
    ofstream f;
    if (params->outputCommand) {
        sprintf(s, "%s.cmd", params->outputPrefix.c_str());
        f.open(s, ios::out);        
        for (int i = 0; i < argc; i++) {
            f << argv[i] << " ";
        }
        f << endl;
        f.close();
    }
    
    
    
    SimulationResults* res = sim->doSimulations(params);




    if (params->outputStats) {
        sprintf(s, "%s.stats", params->outputPrefix.c_str());
        f.open(s, ios::out);

        f << res->printStats();
        f.close();
    }
    if (params->outputStatsJK) {
        sprintf(s, "%s.statsjk", params->outputPrefix.c_str());
        f.open(s, ios::out);

        f << res->printStatsWithJK();
        f.close();
    }
    //cout <<res->sumSFS[0]->toString();



    //print freqTable
    if (params->outputFT) {

        sprintf(s, "%s.ft", params->outputPrefix.c_str());
        f.open(s, ios::out);
        f << res->ft->toString();
        f.close();
    }

    if (params->outputSFS) {
        //printing SFS
        int n = 0;
        for (int i = 0; i < (params->samples.size() - 1); i++) {
            utils::printProgressBar(100* n / params->samples.size() / (params->samples.size() - 1)*2, "printing sfs ");
            for (int j = i + 1; j < params->samples.size(); j++) {
                char s[100];
                sprintf(s, "%s_%d_%d.sfs", params->outputPrefix.c_str(), i, j);
                f.open(s, ios::out);
                f << res->sumSFS[n]->toString();
                f.close();
                n++;
            }

        }
    }


    //generate & print shared SNP
    if (params->outputSNPShared) {
        SNPTable* snpt = res->simulateSNPN(true);
        vector<vector<int>* >* snps = snpt->snp;
        sprintf(s, "%s.snps", params->outputPrefix.c_str());
        vector<vector<int>* >::const_iterator it1;
        vector<int>::const_iterator it2;
        f.open(s, ios::out);

        int i;
        bool firstCol;
        //
        for (it1 = snps->begin(); it1 != snps->end(); ++it1) {
            firstCol = true;
            for (it2 = (*it1)->begin(); it2 != (*it1)->end(); ++it2) {
                if (firstCol) {
                    firstCol = false;
                } else {
                    f << "\t";
                }
                i = (*it2);
                f << i;
            }
            f << endl;
        }
        f.close();
        if(Parameters::verbose>99)
            cout <<"wrote snps file"<<endl;

        //print associated stats
        if (params->outputSNPSharedStats) {
            sprintf(s, "%s.snpsstats", params->outputPrefix.c_str());
            vector<double>* vFST = snpt->getFST();
            vector<double>* vPsi = snpt->getPsi();
            vector<double>* vDeltaH = snpt->getDeltaH();
            vector<double>* vH = snpt->getHeterozygosity();
            int k = 0;
            f.open(s, ios::out);
            for (int i = 0; i < Parameters::nSamplesStart - 1; i++) {
                for (int j = i + 1; j < Parameters::nSamplesStart; j++) {
                    f << k << "\t" << i << "\t" << j << "\t";
                    f << (*vFST)[k] << "\t";
                    f << (*vPsi)[k] << "\t";
                    f << (*vDeltaH)[k] << endl;

                    ++k;
                }
            }
            f.close();
            delete vFST;
	    delete vPsi;
	    delete vDeltaH;
	    //delete vH;
        }
        if(Parameters::verbose>99)
            cout <<"wrote snps stats"<<endl;


        //print and bootstrap stats to seperate files
        if (params->bootstrapSNPSharedStats > 0) {
            BootstrapResampler::writeHeader(params);

            //actual resampling, threaded
            BootstrapResampler::setupResample(params->bootstrapSNPSharedStats,snpt);
            
            BootstrapResampler::writeFooter();

        }
        delete snpt;
    }

    if (params->outputSNP) {
        //generate & print SNP
	SNPTable* snpt = res->simulateSNPN(false);
        vector<vector<int>* >*snps = snpt->snp;
        sprintf(s, "%s.snp", params->outputPrefix.c_str());
        vector<vector<int>* >::const_iterator it1;
        vector<int>::const_iterator it2;
        f.open(s, ios::out);

        int i;
        bool firstCol;
        for (it1 = snps->begin(); it1 != snps->end(); ++it1) {
            firstCol = true;
            for (it2 = (*it1)->begin(); it2 != (*it1)->end(); ++it2) {
                if (firstCol) {
                    firstCol = false;
                } else {
                    f << "\t";
                }
                i = (*it2);
                f << i;
            }
            f << endl;
        }
        f.close();
	delete snpt;
    }

    delete res;
    delete sim;
    delete params;
    return 0;
}

