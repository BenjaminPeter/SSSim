/* 
 * File:   parseArgs.cpp
 * Author: peterb
 * 
 * Created on August 13, 2012, 2:24 PM
 */


#include "Parameters.h"

#define SEED 0

using namespace std;

Parameters::Parameters(int argc, char* argv[]) {
    for (int i=0; i<argc;i++){
        cout <<argv[i]<<" ";
    }
    cout << endl;
    this->outputPrefix="out_";
    this->ss = new SequenceSimulator();
    this->sim = new Simulator(SEED);
    this->nReplicates = atoi(argv[1]);


    //cout <<" "<< nReplicates <<endl;
    bool hasMigScheme = false;
    int x = -1, y = -1, n = -1, i = 2;


    while (i < argc) {
        //cout << i <<"/"<<argc<<":"<<argv[i]<<endl;

        if (string(argv[i]) == "-o") {
            this->outputPrefix=argv[i+1];
            //cout << this->outputPrefix << endl;
            i += 1;
        }
        if (string(argv[i]) == "-t") {
            cout << "bla" << endl;
            ss->setTheta(atof(argv[i + 1]));
            cout << "set Theta to " << ss->getTheta() << endl;
            i += 1;
        }
        //add new sample with -s x,y,n
        if (string(argv[i]) == "-s") {
            int* newSample = new int[3];
            newSample[0] = atoi(argv[i + 1]);
            newSample[1] = atoi(argv[i + 2]);
            newSample[2] = atoi(argv[i + 3]);
            samples.push_back(newSample);
            i += 3;
        }

        //add sequence of samples with -s x,ystart,yend,n
        if (string(argv[i]) == "--seq") {
            int yStart = atoi(argv[i + 2]);
            int yEnd = atoi(argv[i + 3]);
            for (int y = yStart; y < yEnd; ++y) {
                int* newSample = new int[3];
                newSample[0] = atoi(argv[i + 1]);
                newSample[1] = y;
                newSample[2] = atoi(argv[i + 4]);
                samples.push_back(newSample);
            }
            i += 4;
        }

        //add diagonal sequence of samples with -s xstart,ystart,yend,n
        if (string(argv[i]) == "--seqd") {
            int xStart = atoi(argv[i + 1]);
            int yStart = atoi(argv[i + 2]);
            int yEnd = atoi(argv[i + 3]);
            for (int j = 0; j < yEnd - yStart; ++j) {
                int* newSample = new int[3];
                newSample[0] = xStart + j;
                newSample[1] = yStart + j;
                newSample[2] = atoi(argv[i + 4]);
                samples.push_back(newSample);
            }
            i += 4;
        }
        //ibd: set width, height, k, 1 migration rate
        if (string(argv[i]) == "--ibd") {
            if (hasMigScheme) {
                cerr << "error: migration scheme already defined";
                exit;
            }
            hasMigScheme = true;
            int width = atoi(argv[i + 1]);
            int height = atoi(argv[i + 2]);
            double k = atof(argv[i + 3]);
            double m = atof(argv[i + 4]);
            IsolationByDistance * ibd = new IsolationByDistance();
            ibd->setCarCapUniform(k);
            ibd->setMigrationRatesUniform(m, m, m, m);
            ibd->setWidth(width);
            ibd->setHeight(height);
            this->ms = ibd;

            i += 4;
        }
        //ibde: set width, height, k, 1 migration rate away, migrationRate back,xStart,yStart
        if (string(argv[i]) == "--ibde") {
            if (hasMigScheme) {
                cerr << "error: migration scheme already defined";
                exit;
            }
            hasMigScheme = true;
            int width = atoi(argv[i + 1]);
            int height = atoi(argv[i + 2]);
            double k = atof(argv[i + 3]);
            double ma = atof(argv[i + 4]); //away
            double mb = atof(argv[i + 5]); //back
            int startX = atoi(argv[i + 6]);
            int startY = atoi(argv[i + 7]);
            IsolationByDistanceExpansion * ibd = new IsolationByDistanceExpansion();
            ibd->setCarCapUniform(k);
            ibd->setMigrationRatesUniform(ma, mb, ma, mb);
            ibd->setWidth(width);
            ibd->setHeight(height);
            ibd->setXStart(startX);
            ibd->setYStart(startY);
            this->ms = ibd;

            i += 4;
        }
        //Slatkin & Excoffier expansion
        //width, height, k, ek, m, x0, y0, t0, tLag
        if (string(argv[i]) == "--see") {
            if (hasMigScheme) {
                cerr << "error: migration scheme already defined";
                exit;
            }
            hasMigScheme = true;
            int width = atoi(argv[i + 1]);
            int height = atoi(argv[i + 2]);
            double k = atof(argv[i + 3]);
            double ek = atoi(argv[i + 4]);
            double m = atof(argv[i + 5]);
            x = atoi(argv[i + 6]);
            y = atoi(argv[i + 7]);
            double t0 = atof(argv[i + 8]);
            double tLag = atof(argv[i + 9]);

            SEExpansion * see = new SEExpansion();
            see->setCarCapUniform(k);
            see->setMigrationRatesUniform(m, m, m, m);
            see->setStartPos(x, y);
            see->setTLag(tLag);
            see->setCarCapUniform(k);
            see->setTStart(t0);
            see->setExpansionK(ek);
            see->setWidth(width);
            see->setHeight(height);
            see->setupArrivalTimes();
            //cout <<see->toString()<<endl;
            this->ms = see;

            i += 9;
        }
  

        //Slatkin & Excoffier expansion with central area with different expansion k
        //width, height, k, ek, m, x0, y0, t0, tLag centralArea ekCentral
        if (string(argv[i]) == "--seedk") {
            if (hasMigScheme) {
                cerr << "error: migration scheme already defined";
                exit;
            }
            hasMigScheme = true;
            int width = atoi(argv[i + 1]);
            int height = atoi(argv[i + 2]);
            double k = atof(argv[i + 3]);
            double ek = atoi(argv[i + 4]);
            double m = atof(argv[i + 5]);
            x = atoi(argv[i + 6]);
            y = atoi(argv[i + 7]);
            double t0 = atof(argv[i + 8]);
            double tLag = atof(argv[i + 9]);
            int centralArea = atof(argv[i + 10]);
            int ekCentral = atof(argv[i + 11]);

            SEExpansionDiffK * see = new SEExpansionDiffK();
            see->setCarCapUniform(k);
            see->setMigrationRatesUniform(m, m, m, m);
            see->setStartPos(x, y);
            see->setTLag(tLag);
            see->setCarCapUniform(k);
            see->setTStart(t0);
            see->setExpansionK(ek);
            see->setWidth(width);
            see->setHeight(height);
            see->setupArrivalTimes();
            see->setCentralArea(centralArea);
            see->setCentralK(ekCentral);
            //cout <<see->toString()<<endl;
            this->ms = see;

            i += 11;
        }
        i++;        
        
    }
    sim->setMigrationScheme(ms);
    sim->addSequenceSimulator(ss);

    this->sampleSizes = new int[samples.size()];
    i = 0;
    for (vector<int*>::iterator it = samples.begin(); it != samples.end(); ++it) {
        x = (*it)[0];
        y = (*it)[1];
        n = (*it)[2];
        sampleSizes[i] = n;
        sim->addSampleStart(x, y, n);
        i++;
    }
}

Parameters::Parameters() {
}

Parameters::Parameters(const Parameters& orig) {
}

Parameters::~Parameters() {
}

