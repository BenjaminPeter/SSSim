/* 
 * File:   Simulator.cpp
 * Author: peterb
 * 
 * Created on June 1, 2012, 10:53 AM
 */

#include "Simulator.h"


class TreeSimulator;

SequenceSimulator* Simulator::seqSim = 0;
int Simulator::replicate = 0;
boost::mutex Simulator::ftMutex, Simulator::ft2Mutex, Simulator::sfsMutex;
boost::mutex Simulator::counterMutex;

Simulator::Simulator(long seed) {
    utils::setupRng(seed);
    //this->ut = new utils(seed);
    this->migrationScheme = 0;
}

Simulator::Simulator(const Simulator& orig) {
}

Simulator::~Simulator() {
    //delete this->ut;

    //delete all lineages in all samples, then delete all samples
    boost::unordered_map<int, Sample*>::iterator it;

    for (it = this->sampMap.begin(); it != this->sampMap.end(); ++it) {
        it->second->deleteAllLineages();
        delete it->second;
        it->second = NULL;
    }


    delete this->seqSim;
}

void Simulator::getNewGeneTree(Parameters* params, SimulationResults* res, int id) {

    while (Simulator::replicate > 0) {
        cout << "starting, replicate = " << Simulator::replicate << "Thread: " << id << endl;
        boost::mutex::scoped_lock ctrlock(Simulator::counterMutex);
        Simulator::replicate--;
        ctrlock.unlock();
        TreeSimulator* ts = new TreeSimulator(params);
        Lineage* l = ts->run();
        delete ts;

        vector<int*> samples = params->samples;

        int pos = 0;
        //ofstream f;
        SFS* sfs;


        //if (params->outputTree)
        //    f << l->toString() << endl;

        //cout << "thread " << id << " finished" << endl;


        boost::mutex::scoped_lock ftlock(Simulator::ftMutex);
        //cout << "thread " << id << " locked" << endl;
        l->addToFreqTable(res->ft);
        //cout << "thread " << id << " unlocked" << endl;
        ftlock.unlock();

        boost::mutex::scoped_lock ft2lock(Simulator::ft2Mutex);
        l->addToFreqTable(res->ftShared, true);
        ft2lock.unlock();

        if (params->outputStats) {
            boost::mutex::scoped_lock sfslock(Simulator::sfsMutex);
            pos = 0;
            for (unsigned int i = 0; i < samples.size() - 1; i++) {
                for (unsigned int j = i + 1; j < samples.size(); j++) {
                    sfs = Simulator::seqSim->create2DSFS(l, i, j);
                    *(res->sumSFS[pos]) += *sfs;

                    delete sfs;
                    pos++;
                }
            }
            for (int i = 0; i < samples.size(); i++) {
                sfs = Simulator::seqSim->create1DSFS(l, i);
                *(res->sumSFS1d[i]) += *sfs;
                delete sfs;
            }



            sfslock.unlock();
        }
        //tmrca += this->timeSinceStart;
        //nMigrations += this->nMigrationEvents;
        delete l;
        utils::printProgressBar(100 * Simulator::replicate / params->nReplicates, "computing trees");
    }
}

void Simulator::addSequenceSimulator(SequenceSimulator * ss) {
    this->seqSim = ss;
}

SimulationResults* Simulator::doSimulations(Parameters* params) {
    vector<int*> samples = params->samples;

    int nThreads = boost::thread::hardware_concurrency();
    this->replicate = params->nReplicates;
    this->setMigrationScheme(params->ms);
    this->addSequenceSimulator(params->ss);
    
    SimulationResults* res = new SimulationResults();
    res->initialize(params, samples.size(), params->sampleSizes);
    FreqTable* ft = new FreqTable(params->samples.size());
    FreqTable* ftShared = new FreqTable(params->samples.size());
    res->ft = ft;
    res->ftShared = ftShared;


    boost::thread_group tg;
    for (int i = 0; i < nThreads; ++i) {
        tg.create_thread(boost::bind(&this->getNewGeneTree, params, res, i));
    }
    tg.join_all();




    cout << "done!" << endl;


    if (params->outputStats) {
        int pos = 0;
        for (int i = 0; i < samples.size() - 1; i++) {
            utils::printProgressBar(100 * pos * 2 / samples.size() / (samples.size() - 1), "printing stats ");
            for (int j = i + 1; j < samples.size(); j++) {
                res->fst[pos] = StatCalculator::getFST(res->sumSFS[pos]);
                res->psi[pos] = StatCalculator::getPsi(res->sumSFS[pos]);
                res->deltaH[pos] = StatCalculator::getDeltaH(res->sumSFS[pos]);
                pos++;
            }
        }
    }

    return res;

}
