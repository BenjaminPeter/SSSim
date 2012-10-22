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
        for(vector<int>::const_iterator it=vect.begin();it != vect.end(); ++it){
            if(it==0)
                return;
        }
    }
    //cout <<"vect"<<vect[0]<<"\t"<<vect[1]<<endl;
    if (this->freqs.find(vect) != this->freqs.end()) {
       // cout << "found"<<"\t"<<this->freqs[vect];
        this->freqs[vect] += length;
        //cout << "\t"<<this->freqs[vect]<<endl;
    } else {
        this->freqs.insert(pair<vector<int>, double >(vect, length));
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
 * draws n SNP
 */
vector<vector<int> >* FreqTable::drawSNP(int nSNP){
    vector<vector<int> >* v = new vector<vector<int> >();
    for(int i=0; i<nSNP; ++i){
        v->push_back(this->drawSNP());
    }
    return v;
}
