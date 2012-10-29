/* 
 * File:   FreqTable.cpp
 * Author: peterb
 * 
 * Created on August 30, 2012, 1:02 PM
 */

#include <sstream>
#include <boost/unordered_map.hpp>
#include <vector>
#include <iostream>
#include "FreqTable.h"
#include "Parameters.h"

FreqTable::FreqTable(utils* ut, int nPops) {
    this->nPops = nPops;
    this->ut = ut;
}

FreqTable::FreqTable(const FreqTable& orig) {
}

FreqTable::~FreqTable() {
    //delete this->lengths;
    //for(vector<int*>::iterator it=this->alleleFreqs.begin(); it != this->alleleFreqs.end(); ++it){
    //     delete[] (*it);
    //}
    //delete this->alleleFreqs;
}

string FreqTable::toString() {
    stringstream ss;
    std::cout <<"tTot:"<<"\t"<<this->tTot<<endl;
    boost::unordered_map<vector<int>, double>::iterator iter;
    for (iter = this->freqs.begin(); iter != this->freqs.end(); ++iter) {

        const vector<int>* v = &iter->first;
        double length = iter->second;
        if (length > 0) {
            ss << length / this->tTot;
            for (vector<int>::const_iterator it2 = v->begin(); it2 != v->end(); ++it2) {
                ss << "\t" << (*it2);
            }
            ss << endl;
        }
    }
    return ss.str();
}

void FreqTable::addLine(double length, int* pops,bool checkShared) {
    
    vector<int> vect(pops, pops + this->nPops);
    if(checkShared){
        int nShared=0;
        for(vector<int>::const_iterator it=vect.begin();it != vect.end(); ++it){
            if(*it>0){
                nShared++;
            }
        }
        if (nShared<2)
            return;
    }
    //cout <<"vect"<<vect[0]<<"\t"<<vect[1]<<endl;
    if (this->freqs.find(vect) != this->freqs.end()) {
       // cout << "found"<<"\t"<<this->freqs[vect];
        this->freqs[vect] += length;
        //cout << "\t"<<this->freqs[vect]<<endl;
    } else {
        this->freqs.insert(pair<vector<int>, double >(vect, length));
        this->nEntries++;
    }
    this->tTot += length;
}

/*
 * draws a single SNP from the frequency table
 */
vector<int> FreqTable::drawSNP(){
    boost::unordered_map<vector<int>,double>::const_iterator iter;
    double rn = this->ut->randomD(this->tTot);
    for (iter = this->freqs.begin(); iter != this->freqs.end(); ++iter) {
        rn -= iter->second;
        if(rn <0){
            return iter->first;
        }
    }    
}

/*
 * draws a set of n SNP according to some theta
 */

vector<vector<int> >* FreqTable::drawSNP(double theta){
    int nSNP = this->ut->rpois(theta);
    return this->drawSNP(nSNP);
}

/*
 * draws n SNP, old version  that draws one snp at a time
 */
/*vector<vector<int> >* FreqTable::drawSNP(int nSNP){
    vector<vector<int> >* v = new vector<vector<int>* >(nSNP);
    for(int i=0; i<nSNP; ++i){
        utils::printProgressBar(100*i/nSNP,"drawing SNP");
        v[i]=this->drawSNP();
    }
    cout <<"done!"<<endl;
    return v;
}*/
/*
 * draws n SNP, new version that draws from a multinomial, maybe faster?
 */
vector<vector<int> >* FreqTable::drawSNP(int nSNP){
    if (Parameters::verbose>99) cout << "FreqTable::drawSNP:nEntries: "<<this->nEntries<<endl;
    vector<vector<int> > snpRows;
    vector<vector<int> >* snpFinal = new vector<vector<int> >();
    vector<double > branchLengths;
    snpRows.reserve(this->nEntries);
    snpFinal->reserve(nSNP);
    branchLengths.reserve(this->nEntries);
    
    for(boost::unordered_map<vector<int>, double>::const_iterator it = this->freqs.begin(); it!=this->freqs.end(); ++it){
        snpRows.push_back(it->first);
        branchLengths.push_back(it->second);
    }
    if (Parameters::verbose>99) cout << "copied freqTable: "<<this->nEntries<<endl;
    
    unsigned int* snp=this->ut->rmultinom(nSNP,nEntries,&branchLengths[0]);
    if (Parameters::verbose>99) cout << "got SNP pos: "<<this->nEntries<<endl;
    
    gsl_ran_shuffle (ut->rng, snp, nSNP, sizeof(unsigned int) );
    if (Parameters::verbose>99) cout << "done randomizing: " <<endl;
    
    for(int i=0; i<nSNP;++i){
        (*snpFinal)[i] = snpRows[snp[i]];
    }
    if (Parameters::verbose>99) cout << "got SNP: "<<endl;
    
    
    
    cout <<"done!"<<endl;
    return snpFinal;
}

int FreqTable::getNEntries(){
    return this->nEntries;
}