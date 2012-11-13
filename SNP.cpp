/* 
 * File:   SNP.cpp
 * Author: peterb
 * 
 * Created on November 12, 2012, 11:39 AM
 */

#include "SNP.h"

/*using namespace std;

SNP::SNP(int nSamples) {
    this->nSamples = 0;
    this->freqs = new vector<int>();
    this->sampleSizes = new vector<int>();
    this->freqs->reserve(nSamples);
    this->sampleSizes->reserve(nSamples);
}

SNP::SNP(std::vector<int>* freqs, std::vector<int>* sampleSizes, int nSamples) {
    if (this->freqs->size() != this->sampleSizes->size()) {
        cerr << "ERROR: sample Sizes and SNP do not have the same length" << endl;
        throw 10;
    }
    this->freqs = freqs;
    this->sampleSizes = sampleSizes;
    this->nSamples = this->freqs->size();
}

SNP::SNP(const SNP& orig) {
    this->nSamples = orig.nSamples;
    this->freqs = new vector(orig->freqs);
    this->sampleSizes = new vector(orig->sampleSizes);
}

SNP::~SNP() {
    delete this->freqs;
}

int SNP::GetNSamples() const {
    return nSamples;
}

std::vector<int>* SNP::GetSampleSizes() const {
    return sampleSizes;
}

void SNP::SetFreqs(std::vector<int>* freqs) {
    if (freqs->size() != freqs->size()) {
        cerr << "new vector has incorrect size " << endl;
        throw 10;
    }
    this->freqs = freqs;
}

std::vector<int>* SNP::GetFreqs() const {
    return freqs;
}*/

