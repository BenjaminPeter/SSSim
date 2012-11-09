/* 
 * File:   BootstrapResampler.cpp
 * Author: peterb
 * 
 * Created on November 7, 2012, 4:09 PM
 */

#include "BootstrapResampler.h"


int BootstrapResampler::nResamples = 0;
int BootstrapResampler::maxResamples = 0;
ofstream* BootstrapResampler::f = NULL;


boost::mutex BootstrapResampler::ofMutex, BootstrapResampler::brMutex;

BootstrapResampler::BootstrapResampler() {
}

BootstrapResampler::BootstrapResampler(const BootstrapResampler& orig) {
}

BootstrapResampler::~BootstrapResampler() {
}

void BootstrapResampler::setupResample(int nResamples, SNPTable* snpt) {
    //make sure only one resampler is active at a time
    boost::mutex::scoped_lock uniqueLock(brMutex);

    int nThreads = boost::thread::hardware_concurrency();
    BootstrapResampler::nResamples = nResamples;
    BootstrapResampler::maxResamples = nResamples;


    boost::thread_group tg;
    for (int i = 0; i < nThreads; ++i) {
        tg.create_thread(boost::bind(&doResample, snpt, i));
        //tg.create_thread(boost::bind(&this->doResample, snpt,i));
    }
    tg.join_all();
    uniqueLock.unlock();
}

void BootstrapResampler::doResample(SNPTable* snpt, int id) {
    while (BootstrapResampler::nResamples > 0) {
        //do stuff
        cout << "starting thread" << id << "nResamples:"<<BootstrapResampler::nResamples<<endl;
        utils::printProgressBar(100 * BootstrapResampler::nResamples / BootstrapResampler::maxResamples, "resampling... ");
        SNPTable* snpt2 = snpt->getBootstrapResample();
        vector<double>* vFST = snpt2->getFST();
        vector<double>* vPsi = snpt2->getPsi();
        vector<double>* vDeltaH = snpt2->getDeltaH();
        vector<double>* vH = snpt2->getHeterozygosity();
        delete snpt2;

        cout << "finished " << id << endl;
        boost::mutex::scoped_lock lock(ofMutex);
        cout << "locked " << id << endl;
        int k = 0;
        for (int i = 0; i < Parameters::nSamplesStart - 1; i++) {
            for (int j = i + 1; j < Parameters::nSamplesStart; j++) {
                f[0] << "\t" << (*vFST)[k];
                f[1] << "\t" << (*vPsi)[k];
                f[2] << "\t" << (*vDeltaH)[k];
                ++k;
            }
        }
        for (int i = 0; i < Parameters::nSamplesStart; i++) {
            f[3] << "\t" << (*vH)[k];
        }
        f[0] << endl;
        f[1] << endl;
        f[2] << endl;
        f[3] << endl;
        cout << "unlocked " << id << endl;
        lock.unlock();


        delete vFST;
        delete vPsi;
        delete vDeltaH;
        delete vH;
        BootstrapResampler::nResamples--;

    }
}

void BootstrapResampler::writeHeader(Parameters* params) {
    char s[200];
    BootstrapResampler::f = new ofstream[4];

    sprintf(s, "%s.fst.sbs", params->outputPrefix.c_str());
    f[0].open(s, ios::out);
    sprintf(s, "%s.psi.sbs", params->outputPrefix.c_str());
    f[1].open(s, ios::out);
    sprintf(s, "%s.dh.sbs", params->outputPrefix.c_str());
    f[2].open(s, ios::out);
    sprintf(s, "%s.h.sbs", params->outputPrefix.c_str());
    f[3].open(s, ios::out);


    //write header
    int k = 0;
    for (int i = 0; i < Parameters::nSamplesStart - 1; i++) {
        for (int j = i + 1; j < Parameters::nSamplesStart; j++) {
            f[0] << "\t" << k;
            f[1] << "\t" << k;
            f[2] << "\t" << k;
            ++k;
        }
    }
    for (int i = 0; i < Parameters::nSamplesStart; i++) {
        f[3] << "\t" << k;
    }
    f[0] << endl;
    f[1] << endl;
    f[2] << endl;
    f[3] << endl;


    for (int i = 0; i < Parameters::nSamplesStart - 1; i++) {
        for (int j = i + 1; j < Parameters::nSamplesStart; j++) {
            f[0] << "\t" << i;
            f[1] << "\t" << i;
            f[2] << "\t" << i;
        }
    }
    for (int i = 0; i < Parameters::nSamplesStart; i++) {
        f[3] << "\t" << i;
    }
    f[0] << endl;
    f[1] << endl;
    f[2] << endl;
    f[3] << endl;
    for (int i = 0; i < Parameters::nSamplesStart - 1; i++) {
        for (int j = i + 1; j < Parameters::nSamplesStart; j++) {
            f[0] << "\t" << j;
            f[1] << "\t" << j;
            f[2] << "\t" << j;
        }
    }
    f[0] << endl;
    f[1] << endl;
    f[2] << endl;


}

void BootstrapResampler::writeFooter() {
    f[0].close();
    f[1].close();
    f[2].close();
    f[3].close();
    delete[] f;
}