/* 
 * File:   parseArgs.cpp
 * Author: peterb
 * 
 * Created on August 13, 2012, 2:24 PM
 */


#include "Parameters.h"

#define SEED 0

using namespace std;

int Parameters::verbose=0;
Parameters::Parameters(int argc, char* argv[]) {
    for (int i=0; i<argc;i++){
        cout <<argv[i]<<" ";
    }
    cout << endl;
    
    this->outputPrefix="out_";
    this->ss = new SequenceSimulator();
    this->sim = new Simulator(SEED);
    this->nReplicates = atoi(argv[1]);
    this->outputFT=false;
    this->outputSFS=false;
    this->outputSNP=false;
    this->outputSNPShared=false;
    this->outputStats=false;
    this->outputStatsJK=false;
    this->mPropagulePool=false;
    this->outputTree=false;
    Parameters::verbose=0;
    this->nSNP=0;
    ss->setTheta(-1);


    //cout <<" "<< nReplicates <<endl;
    bool hasMigScheme = false;
    int x = -1, y = -1, n = -1, i = 2;


    while (i < argc) {
        if (string(argv[i]) == "-t") {
            if(this->nSNP>0)
                cerr <<"Warning: Setting theta and N at the same time"<<endl;
            ss->setTheta(atof(argv[i + 1]));
            i += 1;
        }
        
        //set number of SNP
        if (string(argv[i]) == "-N") {
            if(ss->getTheta()>0)
                cerr <<"Warning: Setting theta and N at the same time"<<endl;
            //cout <<argv[i+1]<<endl;
            this->nSNP=atoi(argv[i+1]);
            i += 1;
        }

//*****************************************************************************        
//*******     output options                                             ********
//***************************************************************************** 
        if (string(argv[i]) == "-o") {
            this->outputPrefix=argv[i+1];
            //cout << this->outputPrefix << endl;
            i += 1;
        }
        if (string(argv[i]) == "--verbose") {
            Parameters::verbose=atoi(argv[i+1]);
            i += 1;
        }
        if (string(argv[i]) == "--oloci") {
            this->outputLoci=true;
        }
        if (string(argv[i]) == "--osfs") {
            this->outputSFS=true;
        }
        if (string(argv[i]) == "--oft") {
            this->outputFT=true;
        }
        if (string(argv[i]) == "--osnp") {
            this->outputSNP=true;
        }
        if (string(argv[i]) == "--osnps") {
            this->outputSNPShared=true;
        }
        if (string(argv[i]) == "--ostats") {
            this->outputStats=true;
        }
        if (string(argv[i]) == "--ostatsjk") {
            this->outputStatsJK=true;
        }
        if (string(argv[i]) == "--otree") {
            this->outputTree=true;
        }

//*****************************************************************************        
//*******     migration options                                        ********
//***************************************************************************** 
        if (string(argv[i]) == "-mpp") {
            this->mPropagulePool=true;
        }

        
//*****************************************************************************        
//*******     get Samples                                              ********
//*****************************************************************************        
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

        //adds sampling grid with -sgrid x0,y0,xend,yend,distance
        if(string(argv[i]) == "-sgrid"){
            int xStart = atoi(argv[i+1]);
            int yStart = atoi(argv[i+2]);
            int xEnd = atoi(argv[i+3]);
            int yEnd = atoi(argv[i+4]);
            int dist = atoi(argv[i+5]);
            int sampSize = atoi(argv[i+6]);
            for (int x = xStart; x < xEnd; x+=dist){
                for (int y = yStart; y < yEnd; y+=dist){
                    int* newSample = new int[3];
                    newSample[0] = x;
                    newSample[1] = y;
                    newSample[2] = sampSize;
                    samples.push_back(newSample);
                }
            }
            i += 6;
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
        
        
        /**********************************************************
         *  Models         
         **********************************************************/
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

    char s[100];
    ofstream f;

    sprintf(s, "%s.loc", this->outputPrefix.c_str());
    f.open(s, ios::out);
    for (vector<int*>::iterator it = samples.begin(); it != samples.end(); ++it) {
        x = (*it)[0];
        y = (*it)[1];
        n = (*it)[2];
        sampleSizes[i] = n;
        if ( this->outputLoci ){
            f << i << "\t"<<x <<"\t"<< y<<endl;
        }
        sim->addSampleStart(x, y, n);
        i++;
    }
    f.close();
}

Parameters::Parameters() {
}

Parameters::Parameters(const Parameters& orig) {
}

Parameters::~Parameters() {
}

