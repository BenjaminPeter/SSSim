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

    Parameters* params = new Parameters(argc, argv);

    Simulator* sim = params->sim;

    if (!sim->migrationSchemeReady()) {
        cout << "width is" << sim->migrationScheme->getWidth() << endl;
        cerr << "mig scheme not properly initialized" << endl;
        return 1;
    }



    SimulationResults* res = sim->doSimulations(params);



    res->printStatsWithJK();
    //cout <<res->sumSFS[0]->toString();
    ofstream f;


    //print freqTable
    char s[100];
    sprintf(s, "%s.ft", params->outputPrefix.c_str());
    f.open(s, ios::out);
    f << res->ft->toString();
    f.close();




    //printing SFS
    int n = 0;
    //cout  <<"nSamp:"<<params->samples.size()<<endl;
    for (int i = 0; i < (params->samples.size() - 1); i++) {
        for (int j = i + 1; j < params->samples.size(); j++) {
            //cout  <<i<<","<<j<<","<<params->samples.size()<<endl;
            char s[100];
            sprintf(s, "%s_%d_%d.sfs", params->outputPrefix.c_str(), i, j);
            f.open(s, ios::out);

            f << res->sumSFS[n]->toString();
            f.close();
            n++;
        }

    }


    delete res;
    delete sim;
    delete params;
    return 0;
}
