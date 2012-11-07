/* 
 * File:   SNPTable.cpp
 * Author: peterb
 * 
 * Created on September 12, 2012, 12:22 PM
 */

#include "SNPTable.h"
#include "Parameters.h"

SNPTable::SNPTable() {
    this->nPops=0;
    this->nSNP=0;
    this->snp = new vector<vector<int>* >();
}

SNPTable::SNPTable(vector<vector<int>* >* snp, int nSNP, int nPops){
    this->snp = snp;
    //cout <<snp->size()<<endl;//\t"<<(*snp)[0]->size()<<endl;
    
    this->nSNP =nSNP;
    this->nPops = nPops;
}

SNPTable::SNPTable(const SNPTable& orig) {
}

SNPTable::~SNPTable() {
    delete this->snp;
}



//generators

SNPTable* SNPTable::getBootstrapResample(){
    vector<vector<int>* >* snpResample = new vector<vector<int>* >();
    snpResample->reserve(nSNP);

    
    double p[nSNP];
    for(int i =0; i<nSNP;++i)
        p[i]=1./nSNP;
    unsigned int* snp=utils::rmultinom(nSNP,nSNP,p);
    
    
    for(int i=0; i<nSNP;++i){
        for(int j=0; j<snp[i];++j){
            snpResample->push_back( (*this->snp)[i]);
        }
    }
    utils::rshuffle(snpResample);
    SNPTable* st = new SNPTable(snpResample,nSNP,this->nPops);
    return st;
}

vector<SNPTable*>* SNPTable::getBootstrapResample(int nResamples){
    vector<SNPTable*>* vSNP = new vector<SNPTable*>();
    vSNP->reserve(nResamples);
    
    for(int i=0; i<nResamples;++i){
        vSNP->push_back(this->getBootstrapResample());
    }
    return vSNP;    
}


// get SFS
SFS* SNPTable::make2DSFS(int pop1, int pop2){
    int p1Size = Parameters::sampleSizes[pop1];
    int p2Size = Parameters::sampleSizes[pop2];
    SFS* sfs = new SFS(p1Size,p2Size);
    
    vector<vector<int>* >::const_iterator it;
    for( it = this->snp->begin(); it != this->snp->end(); ++ it){
        vector<int>* curEntry = (*it);
        sfs->addToEntry((*curEntry)[pop1],(*curEntry)[pop2],1.0);
    }
    return sfs;
    
}

SFS* SNPTable::make1DSFS(int pop1){
    int p1Size = Parameters::sampleSizes[pop1];
    SFS* sfs = new SFS(p1Size);
    
    vector<vector<int>* >::const_iterator it;
    for( it = this->snp->begin(); it != this->snp->end(); ++ it){
        vector<int>* curEntry = (*it);
        sfs->addToEntry((*curEntry)[pop1],1.0);
    }
    return sfs;
    
}

vector<double>* SNPTable::getFST(){
    int nComp = this->nPops*(this->nPops-1)/2;
    vector<double>* v = new vector<double>();
    SFS* sfs;
    v->reserve(nComp);
    int k=0;
    for(int i = 0; i<(this->nPops-1); ++i){
        for(int j=i+1; j<this->nPops; ++j){
            sfs = this->make2DSFS(i,j);
            (*v)[k] = StatCalculator::getFST(sfs);
            ++k; 
            delete sfs;
        }
    }
    return v;
}

vector<double>* SNPTable::getPsi(){
    int nComp = this->nPops*(this->nPops-1)/2;
    vector<double>* v = new vector<double>();
    SFS* sfs;
    v->reserve(nComp);
    int k=0;
    for(int i = 0; i<(this->nPops-1); ++i){
        for(int j=i+1; j<this->nPops; ++j){
            sfs = this->make2DSFS(i,j);
            (*v)[k] = StatCalculator::getPsi(sfs);
            ++k; 
            delete sfs;
        }
    }
    return v;
}

vector<double>* SNPTable::getDeltaH(){
    int nComp = this->nPops*(this->nPops-1)/2;
    vector<double>* v = new vector<double>();
    SFS* sfs;
    v->reserve(nComp);
    int k=0;
    for(int i = 0; i<(this->nPops-1); ++i){
        for(int j=i+1; j<this->nPops; ++j){
            sfs = this->make2DSFS(i,j);
            (*v)[k] = StatCalculator::getDeltaH(sfs);
            ++k; 
            delete sfs;
        }
    }
    return v;
}

vector<double>* SNPTable::getHeterozygosity(){
    vector<double>* v = new vector<double>();
    v->reserve(this->nPops);
    
    SFS* sfs;
    int k=0;
    for(int i = 0; i < this->nPops; ++i){
            sfs = this->make1DSFS(i);
            (*v)[k] = StatCalculator::getH(sfs);
            ++k; 
            delete sfs;
    }
    return v;
}