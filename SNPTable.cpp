/* 
 * File:   SNPTable.cpp
 * Author: peterb
 * 
 * Created on September 12, 2012, 12:22 PM
 */

#include "SNPTable.h"
#include "Parameters.h"

SNPTable::SNPTable() {
    this->nPops = 0;
    this->nSNP = 0;
    this->snp = new vector<vector<int>* >();
    this->calcH = false;
    this->heterozygosity = NULL;
}

SNPTable::SNPTable(vector<vector<int>* >* snp, int nSNP, int nPops) {
    this->snp = snp;
    //cout <<snp->size()<<endl;//\t"<<(*snp)[0]->size()<<endl;

    this->nSNP = nSNP;
    this->nPops = nPops;

    this->calcH = false;
    this->calcSFS = false;
    this->heterozygosity = NULL;
}

SNPTable::SNPTable(const SNPTable& orig) {
}

SNPTable::~SNPTable() {
    for(int i=0; i<this->snp->size();++i)
        delete this->snp->at(i);
    delete this->snp;
    //delete this->heterozygosity;
    delete this->pi;
        
    //delete this->heterozygosity;
}



//generators

SNPTable* SNPTable::getBootstrapResample() {
    vector<vector<int>* >* snpResample = new vector<vector<int>* >();
    snpResample->reserve(nSNP);


    double p[nSNP];
    for (int i = 0; i < nSNP; ++i)
        p[i] = 1. / nSNP;
    unsigned int* snp = utils::rmultinom(nSNP, nSNP, p);


    for (int i = 0; i < nSNP; ++i) {
        for (int j = 0; j < snp[i]; ++j) {
            snpResample->push_back((*this->snp)[i]);
        }
    }
    utils::rshuffle(snpResample);
    SNPTable* st = new SNPTable(snpResample, nSNP, this->nPops);
    delete[] snp;
    return st;
}

vector<SNPTable*>* SNPTable::getBootstrapResample(int nResamples) {
    vector<SNPTable*>* vSNP = new vector<SNPTable*>();
    vSNP->reserve(nResamples);

    for (int i = 0; i < nResamples; ++i) {
        vSNP->push_back(this->getBootstrapResample());
    }
    return vSNP;
}


// get SFS

SFS* SNPTable::make2DSFS(int pop1, int pop2) {
    int p1Size = Parameters::sampleSizes[pop1];
    int p2Size = Parameters::sampleSizes[pop2];
    SFS* sfs = new SFS(p1Size, p2Size);

    vector<vector<int>* >::const_iterator it;
    for (it = this->snp->begin(); it != this->snp->end(); ++it) {
        vector<int>* curEntry = (*it);
        sfs->addToEntry((*curEntry)[pop1], (*curEntry)[pop2], 1.0);
    }
    return sfs;

}

SFS* SNPTable::make1DSFS(int pop1) {
    int p1Size = Parameters::sampleSizes[pop1];
    SFS* sfs = new SFS(p1Size);

    vector<vector<int>* >::const_iterator it;
    for (it = this->snp->begin(); it != this->snp->end(); ++it) {
        vector<int>* curEntry = (*it);
        sfs->addToEntry((*curEntry)[pop1], 1.0);
    }
    return sfs;

}


//this stuff computes stats directly on freqtable. Works, but 

vector<double>* SNPTable::getFST() {
    if (this->calcSFS) { //SFS version should be faster, if available
        int nComp = this->nPops * (this->nPops - 1) / 2;
        vector<double>* fst = new vector<double>();
        fst->reserve(nComp);

        int k = 0;
        for (int i = 0; i < (this->nPops - 1); ++i) {
            for (int j = i + 1; j<this->nPops; ++j) {
                fst->push_back(StatCalculator::getFST(sfs[k]));
                ++k;
            }
        }
        return fst;
    }
    if (!this->calcH)
        this->getHeterozygosity();

    int* n = Parameters::sampleSizes;
    int nComp = this->nPops * (this->nPops - 1) / 2;
    vector<double>* fst = new vector<double>();
    fst->reserve(nComp);

    int k = 0;
    for (int i = 0; i < (this->nPops - 1); ++i) {
        for (int j = i + 1; j<this->nPops; ++j) {
            //utils::printProgressBar(100 * k / nComp, "calculating FST");
            double pib = 0;
            for (int s = 0; s<this->nSNP; ++s) {
                pib += this->snp->at(s)->at(i) * (n[j] - this->snp->at(s)->at(j))
                        + this->snp->at(s)->at(j) *
                        (n[i] - this->snp->at(s)->at(i));

            }
            pib /= (n[i] * n[j]);
            //cout<<"fist"<<pi1<<"\t"<<pi2<<"\t"<<pib<<endl;
            fst->push_back(1 - (this->pi->at(i) + this->pi->at(j)) / 2 / pib);
            ++k;
        }
    }
    delete[] n;
    return fst;
}

vector<double>* SNPTable::getPsi() {
    if (this->calcSFS) { //SFS version should be faster, if available
        int nComp = this->nPops * (this->nPops - 1) / 2;
        vector<double>* psi = new vector<double>();
        psi->reserve(nComp);

        int k = 0;
        for (int i = 0; i < (this->nPops - 1); ++i) {
            for (int j = i + 1; j<this->nPops; ++j) {
                psi->push_back(StatCalculator::getPsi(sfs[k]));
                ++k;
            }
        }
        return psi;
    }

    int* n = Parameters::sampleSizes;
    int nComp = this->nPops * (this->nPops - 1) / 2;
    vector<double>* psi = new vector<double>();
    psi->reserve(nComp);

    int k = 0;
    for (int i = 0; i < (this->nPops - 1); ++i) {
        for (int j = i + 1; j<this->nPops; ++j) {
            // utils::printProgressBar(100 * k / nComp, "calculating Psi");
            int nPresent = 0, diff = 0;
            for (int s = 0; s<this->nSNP; ++s) {

                if (this->snp->at(s)->at(i) > 0 && this->snp->at(s)->at(j) > 0) { //not zero
                    if (this->snp->at(s)->at(i) < n[i] && this->snp->at(s)->at(j) < n[j]) { //not fixed

                        diff += (this->snp->at(s)->at(i) - this->snp->at(s)->at(j));
                        nPresent++;
                    }
                }
            }
            //cout <<"psi "<< diff << "\t"<<nPresent<<endl;
            if (nPresent > 0) {
                psi->push_back(float(diff) / nPresent);
            } else {
                psi->push_back(0);
            }
            ++k;
        }
    }
    delete[] n;
    return psi;
}

vector<double>* SNPTable::getDeltaH() {
    if (!this->calcH)
        this->getHeterozygosity();

    int nComp = this->nPops * (this->nPops - 1) / 2;
    vector<double>* v = new vector<double>();
    v->reserve(nComp);

    int k = 0;
    for (int i = 0; i < (this->nPops - 1); ++i) {
        for (int j = i + 1; j<this->nPops; ++j) {

            //utils::printProgressBar(100 * k / nComp, "calculating DeltaH");
            v->push_back(heterozygosity->at(i) - heterozygosity->at(j));
            ++k;
        }
    }

    return v;
}

vector<double>* SNPTable::getHeterozygosity() {
    if (this->calcH)
        return this->heterozygosity;


    int* n = Parameters::sampleSizes;
    vector<double>* h = new vector<double>();
    vector<double>* pi = new vector<double>();
    h->reserve(this->nPops);
    pi->reserve(this->nPops);

    for (int i = 0; i<this->nPops; ++i) {
        //utils::printProgressBar(100 * i / nPops, "calculating H");
        int nPresent = 0;
        pi->push_back(0);
        for (int j = 0; j<this->nSNP; ++j) {
            int f = this->snp->at(j)->at(i);
            if (f > 0) {
                pi->at(i) += f * (n[i] - f);
                nPresent++;
            }
        }
        pi->at(i) /= (n[i]*(n[i] - 1) / 2);
        h->push_back(pi->at(i) / nPresent);
        //cout<<"h"<<h->at(i)<<"\t"<<pi->at(i)<<"\t"<<nPresent<<endl;

    }
    this->heterozygosity = h;
    this->pi = pi;
    this->calcH = true;
    return h;
    delete[] n;


}

void SNPTable::calculateAllSFS() {
    vector<vector<int>* >::iterator it;
    int* n = Parameters::sampleSizes;
    int nComp = this->nPops * (this->nPops - 1) / 2;
    sfs.reserve(nComp);
    for (int i = 0; i < (this->nPops - 1); ++i) {
        for (int j = i + 1; j<this->nPops; ++j) {
            sfs.push_back(new SFS(n[i], n[j]));
        }
    }


    int k = 0;
    for (it = this->snp->begin(); it != this->snp->end(); ++it) {
        for (int i = 0; i < (this->nPops - 1); ++i) {
            for (int j = i + 1; j<this->nPops; ++j) {
                sfs[k]->addToEntry((*it)->at(i), (*it)->at(j), 1);
                ++k;
            }
        }
    }
    this->calcSFS = true;
}
