/* 
 * File:   parseArgs.cpp
 * Author: peterb
 * 
 * Created on August 13, 2012, 2:24 PM
 */


#include "Parameters.h"


using namespace std;

int Parameters::verbose = 0;
SampleList Parameters::sampMapStart = SampleList();
int Parameters::nLineagesStart = 0;
int Parameters::nSamplesStart = 0;
int* Parameters::sampleSizes = NULL;
bool Parameters::mPropagulePool = false;
int Parameters::seed = 0;
int Parameters::nThreads = boost::thread::hardware_concurrency();
bool Parameters::memoryEfficient = true;
double Parameters::theta = -1;

void Parameters::printHelp() {
    cout << "Usage and options: /sssim <number of trees>" << endl;
    cout << "\t-t" << "\ttheta = 4Nmu for generation of SNP" << endl;
    cout << "\t--theta" << "\tsame as -t" << endl;
    cout << "\t-N" << "\tnumber of SNP to sample, cannot be set at the same time with -t" << endl;
    cout << "\t-h" << "\tshows this help dialog" << endl;
    cout << "\t--help" << "\tsame as -h" << endl;
    cout << "\t--verbose\t" << "debug level, the higher, the more detailed debug messages will be displayed" << endl;
    cout << "\t--threads\t" << "max number of threads to use" << endl;
    cout << "\t--mpp\t" << "use migrant pool propagatin" << endl;
    cout << endl;

    cout << "\t-o <prefix>\t" << "defines an output prefix, defaults to out_" << endl;
    cout << "\t--oloci\t" << "outputs a file (*.loc) with the sample locations" << endl;
    cout << "\t--osfs\t" << "outputs a file (*.loc) with pairwise SFS" << endl;
    cout << "\t--oft\t" << "outputs a file (*.ft) with frequency table" << endl;
    cout << "\t--osnp\t" << "ouputs a file (*.snp) with SNPs drawn from the generated trees" << endl;
    cout << "\t--osnps\t" << "ouputs a file (*.snps) with SNPs drawn from the generated trees shared between at least two populations" << endl;
    cout << "\t--ostats\t" << "outputs a file (*.stats) with FST, psi and DH" << endl;
    cout << "\t--ostatsJK\t" << "outputs a file (*.statsjk) with FST, psi and DH, along with se based on Jackknife (NOT WORKING)" << endl;
    cout << "\t--otree\t" << "outputs a file (*.tree) with the trees simualted in newick format" << endl;
    cout << endl;

    cout << "\t-s <x> <y> <n>\t" << "adds a new sample of size n at location (x,y)" << endl;
    cout << "\t--sseq <x> <ystart> <yend> <n> [offset]\t" << "adds a sequence of samples" << endl;
    cout << "\t--sseqd <x> <ystart> <yend> <n> [offset]\t" << "adds a diagonal sequence of samples" << endl;
    cout << "\t--sgrid <x0> <y0> <xend> <yend> <offset> <n>\t" << "adds grid of samples from (x0,y0) to (xend,yend), with offset k and sample size n" << endl;
    cout << endl;

    cout << "\t--ibd <w> <h> <N> <m>\t" << "adds ibd migration scheme on a w x h grid with deme size N and scaled migration rate m" << endl;
    cout << "\t--ibde <w> <h> <N> <m_a> <m_b> <x_s> <x_e>\t" << endl;
    cout << "\t--see <w> <h> <N> <k> <m> <x_s> <y_s> <t0> <t_Lag>\t" << endl;
    cout << "\t--seedk <w> <h> <N> <k> <m> <x_s> <y_s> <t0> <t_Lag> <centralArea> <kCentral>\t" << endl;
    cout << endl;
    cout << "\t\t<mandatory arguments in angle brackets>" << endl;
    cout << "\t\t[optional arguments in square brackets]" << endl;
    exit;
}

Parameters::Parameters(int argc, char* argv[]) {
    for (int i = 0; i < argc; i++) {
        cout << argv[i] << " ";
    }
    cout << endl;

    this->seed = 0;
    this->mPropagulePool = false;
    this->outputPrefix = "out_";
    this->nReplicates = atoi(argv[1]);
    this->outputFT = false;
    this->outputSFS = false;

    this->outputSNP = false;
    this->outputSNPShared = false;

    this->outputStats = false;
    this->outputStatsJK = false;

    this->outputSNPStats = false;
    this->outputSNPSharedStats = false;
    this->bootstrapSNPSharedStats = 0;
    this->bootstrapSNPStats = 0;
    this->outputCommand = false;

    this->outputTree = false;

    bool doRandomSample = false;
    int rsPar1, rsPar2;

    Parameters::verbose = 0;
    this->nSNP = 0;



    //cout <<" "<< nReplicates <<endl;
    bool hasMigScheme = false;
    int x = -1, y = -1, n = -1, i = 2;


    while (i < argc) {
        if (string(argv[i]) == "-t" || string(argv[i]) == "--theta") {
            if (this->nSNP > 0)
                cerr << "Warning: Setting theta and N at the same time" << endl;
            this->theta = atof(argv[i + 1]);
            i += 1;
        }

        //set number of SNP
        if (string(argv[i]) == "-N") {
            if (this->theta > 0)
                cerr << "Warning: Setting theta and N at the same time" << endl;
            //cout <<argv[i+1]<<endl;
            this->nSNP = atoi(argv[i + 1]);
            i += 1;
        }
        //*****************************************************************************        
        //*******     performance options                                             ********
        //***************************************************************************** 

        //force number of threads to be lower than max hardware
        if (string(argv[i]) == "--threads") {
            Parameters::nThreads = min(Parameters::nThreads, atoi(argv[i + 1]));
            i += 1;
        }

        //use more memory efficient, but slower implementation
        if (string(argv[i]) == "--waste-memory") {
            Parameters::memoryEfficient = false;
        }

        //rng seed
        if (string(argv[i]) == "--seed") {
            Parameters::seed = atoi(argv[i + 1]);
            i += 1;
        }
        //*****************************************************************************        
        //*******     output options                                             ********
        //***************************************************************************** 
        if (string(argv[i]) == "-h" || string(argv[i]) == "--help") {
            this->printHelp();
            return;
        }

        if (string(argv[i]) == "-o") {
            this->outputPrefix = argv[i + 1];
            //cout << this->outputPrefix << endl;
            i += 1;
        }
        if (string(argv[i]) == "--ocmd") {
            this->outputCommand = true;
        }
        if (string(argv[i]) == "--verbose") {
            Parameters::verbose = atoi(argv[i + 1]);
            i += 1;
        }


        if (string(argv[i]) == "--osnpstats") {
            this->outputSNPStats = true;
        }
        if (string(argv[i]) == "--osnpsstats") {
            this->outputSNPSharedStats = true;
        }
        if (string(argv[i]) == "--obssnp") {
            this->bootstrapSNPStats = atoi(argv[i + 1]);
            i += 1;
        }

        if (string(argv[i]) == "--obssnps") {
            this->bootstrapSNPSharedStats = atoi(argv[i + 1]);
            i += 1;
        }



        if (string(argv[i]) == "--oloci") {
            this->outputLoci = true;
        }
        if (string(argv[i]) == "--osfs") {
            this->outputSFS = true;
        }
        if (string(argv[i]) == "--oft") {
            this->outputFT = true;
        }
        if (string(argv[i]) == "--osnp") {
            this->outputSNP = true;
        }
        if (string(argv[i]) == "--osnps") {
            this->outputSNPShared = true;
        }
        if (string(argv[i]) == "--ostats") {
            this->outputStats = true;
        }
        if (string(argv[i]) == "--ostatsjk") {
            this->outputStatsJK = true;
        }
        if (string(argv[i]) == "--otree") {
            this->outputTree = true;
        }

        //*****************************************************************************        
        //*******     migration options                                        ********
        //***************************************************************************** 
        if (string(argv[i]) == "--mpp") {
            this->mPropagulePool = true;
        }
        //*****************************************************************************        
        //*******     add barrier                                              ********
        //***************************************************************************** 
        if (string(argv[i]) == "-b") {
            Coords bottomleft = Coords(atoi(argv[i + 1]), atoi(argv[i + 2]));
            Coords topright = Coords(atoi(argv[i + 3]), atoi(argv[i + 4]));
            double k = atof(argv[i + 5]);
            double mInside = atof(argv[i + 6]);
            double mBoundary = atof(argv[i + 7]);
            this->barriers.push_back(new Barrier(bottomleft, topright, k, mInside, mBoundary));
            i += 7;
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

        //add n random samples
        if (string(argv[i]) == "--srandom") {
            rsPar1 = atoi(argv[i + 1]);
            rsPar2 = atoi(argv[i + 2]);
            doRandomSample = true;

            i += 2;
        }

        //add sequence of samples with -s x,ystart,yend,n [offset]
        if (string(argv[i]) == "--sseq") {
            int yStart = atoi(argv[i + 2]);
            int yEnd = atoi(argv[i + 3]);

            int offset = 1;
            if (i + 5 < argc) {
                if (argv[i + 5][0] != '-') {
                    offset = atoi(argv[i + 5]);
                }
            }
            for (int y = yStart; y < yEnd; y += offset) {
                int* newSample = new int[3];
                newSample[0] = atoi(argv[i + 1]);
                newSample[1] = y;
                newSample[2] = atoi(argv[i + 4]);
                samples.push_back(newSample);
            }
            i += 4;
        }

        //adds sampling grid with -sgrid x0,y0,xend,yend,distance
        if (string(argv[i]) == "--sgrid") {
            int xStart = atoi(argv[i + 1]);
            int yStart = atoi(argv[i + 2]);
            int xEnd = atoi(argv[i + 3]);
            int yEnd = atoi(argv[i + 4]);
            int dist = atoi(argv[i + 5]);
            int sampSize = atoi(argv[i + 6]);
            for (int x = xStart; x < xEnd; x += dist) {
                for (int y = yStart; y < yEnd; y += dist) {
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
        if (string(argv[i]) == "--sseqd") {
            int xStart = atoi(argv[i + 1]);
            int yStart = atoi(argv[i + 2]);
            int yEnd = atoi(argv[i + 3]);

            int offset = 1;
            if (i + 5 < argc) {
                if (argv[i + 5][0] != '-') {
                    offset = atoi(argv[i + 5]);
                }
            }

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
            IsolationByDistance * ibd;
            if (Parameters::barriers.size() == 0)
                ibd = new IsolationByDistance();
            else
                ibd = new IsolationByDistanceBarrier();
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
            SEExpansion * see;
            if (Parameters::barriers.size() == 0)
                see = new SEExpansion();
            else
                see = new SEExpansionBarrier();

            see->setWidth(width);
            see->setHeight(height);
            see->setCarCapUniform(k);
            see->setMigrationRatesUniform(m, m, m, m);
            see->setStartPos(x, y);
            see->setTLag(tLag);
            see->setTStart(t0);
            see->setExpansionK(ek);

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
    //sim->setMigrationScheme(ms);
    //sim->addSequenceSimulator(ss);


    //has to be done afterwards because of rng init
    utils::setupRng(this->seed);
    if (doRandomSample)
        this->generateRandomSample(rsPar1, rsPar2);





    this->ms->addBarriersToMigrationScheme();

    this->sampleSizes = new int[samples.size()];
    i = 0;

    char s[100];
    ofstream f;

    if (this->outputLoci) {
        sprintf(s, "%s.loc", this->outputPrefix.c_str());
        f.open(s, ios::out);
    }

    for (vector<int*>::iterator it = samples.begin(); it != samples.end(); ++it) {
        x = (*it)[0];
        y = (*it)[1];
        n = (*it)[2];
        sampleSizes[i] = n;
        if (this->outputLoci) {
            f << i << "\t" << x << "\t" << y << endl;
        }
        this->addSampleStart(x, y, n);
        i++;
    }
    if (this->outputLoci) {
        f.close();
    }

    if (Parameters::barriers.size() > 0) {

    }
}

Parameters::Parameters() {
}

Parameters::Parameters(const Parameters& orig) {
}

Parameters::~Parameters() {
    for (int i = 0; i < samples.size(); ++i) {
        delete[] samples[i];
    }
    delete[] sampleSizes;
    for (SampleList::iterator it = sampMapStart.begin();
            it != sampMapStart.end(); ++it) {
        delete it->second;
    }
    delete this->ms;
}

void Parameters::addSampleStart(int* pos, int nNewLineages, bool outputLoci, stringstream* sOutputLoci) {
    this->addSampleStart(pos[0], pos[1], nNewLineages, outputLoci, sOutputLoci);
}

void Parameters::addSampleStart(int x, int y, int nNewLineages, bool outputLoci, stringstream* sOutputLoci) {
    //int pos = ms->coords2d1d(x, y);
    Coords pos = Coords(x, y);
    if (ms->getArrivalTime(pos) == 0) {
        cerr << "Error: sample (" << x << "," << y << ") outside the colonized area,   ";
        cerr << "ArrivalTime:" << ms->getArrivalTime(pos) << endl;
        throw 10;
    } else {
    }

    Lineage* l;
    vector<Lineage*>newLineages;
    if (outputLoci) {
        (*sOutputLoci) << this->nSamplesStart << "\t" << x << "\t" << y << "\t" << nNewLineages << endl;
    }
    for (int i = this->nLineagesStart; i<this->nLineagesStart + nNewLineages; i++) {
        l = new LineageTemplate(i, x, y, this->nSamplesStart);
        newLineages.push_back(l);
    }
    Sample* newSample = new Sample(x, y, newLineages);
    this->nLineagesStart += nNewLineages;
    this->nSamplesStart++;
    Parameters::sampMapStart.insert(pair<Coords, Sample*>(pos, newSample));

}

void Parameters::generateRandomSample(int nSamples, int sampSize) {
    if (this->ms == NULL) {
        cerr << "migration scheme has to be set first" << endl;
        throw 10;
    }

    int height = this->ms->getHeight();
    int width = this->ms->getWidth();

    int nSuccessfulSamples = 0, nUnsuccessfulSamples = 0;
    //int propX, propY;
    Coords prop;
    vector<Coords> vCoords;
    vector<Coords>::iterator it;
    bool isAccepted = true;


    while (nSuccessfulSamples < nSamples) {
        prop = Coords(utils::random1(width), utils::random1(height));

        for (vector<Barrier*>::iterator itb = this->barriers.begin();
                itb != this->barriers.end(); ++itb) {
            isAccepted = !(*itb)->isInside(prop);
        }

        for (it = vCoords.begin(); it != vCoords.end(); ++it) {
            if ((*it) - prop == 0) {
                isAccepted = false;
            }
        }
        if (isAccepted) {
            vCoords.push_back(prop);
            cout << "accepted " << prop << sampSize << endl;
            nSuccessfulSamples++;
        } else {
            nUnsuccessfulSamples++;
        }

        if (nUnsuccessfulSamples > 1000) {
            cerr << "couldn't generate random sample" << endl;
        }
    }


    for (it = vCoords.begin(); it != vCoords.end(); ++it) {
        int* arr = new int[3];
        arr[0] = (*it).first;
        arr[1] = (*it).second;
        arr[2] = sampSize;
        this->samples.push_back(arr);
    }
}