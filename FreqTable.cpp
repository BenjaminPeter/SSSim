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

FreqTable::FreqTable(int nPops) {
    this->nPops = nPops;
    this->nEntries = 0;
}

FreqTable::FreqTable(const FreqTable& orig) {
}

FreqTable::~FreqTable() {
    //delete this->idMap;
    //delete this->lengths;
    //for(vector<int*>::iterator it=this->alleleFreqs.begin(); it != this->alleleFreqs.end(); ++it){
    //     delete[] (*it);
    //}
    //delete this->alleleFreqs;
}

string FreqTable::toString() {
    stringstream ss;
    std::cout <<"tTot:"<<"\t"<<this->tTot<<endl;
    boost::unordered_map<vector<int>, int>::iterator iter;
    for (iter = this->idMap.begin(); iter != this->idMap.end(); ++iter) {

        const vector<int> v = iter->first;
        double length = this->branchLengths[iter->second];
        if (length > 0) {
            ss << length / this->tTot;
            for (vector<int>::const_iterator it2 = v.begin(); it2 != v.end(); ++it2) {
                ss << "\t" << (*it2);
            }
            ss << endl;
        }
    }
    return ss.str();
}

void FreqTable::addLine(double length, int* pops,bool checkShared) {
    
    vector<int>* vect= new vector<int>(pops, pops + this->nPops);
    if(checkShared){
        int nShared=0;
        for(vector<int>::const_iterator it=vect->begin();it != vect->end(); ++it){
            if(*it>0){
                nShared++;
                if(nShared>1)
                    break;
            }
        }
        if (nShared<2){
	    delete vect;
            return;
	}
    }
    //cout <<"vect"<<vect[0]<<"\t"<<vect[1]<<endl;
    boost::unordered_map<vector<int>,int>::iterator it;
    
    if(this->idMap.find(*vect) != this->idMap.end()){
        //we found it
        int id = this->idMap[*vect];
        this->branchLengths[id] += length;
        this->tTot += length;
    }else{
        int id = this->nEntries;
        this->idMap.insert(pair<vector<int>,int>(*vect,id));
        this->branchLengths.push_back(length);
        this->nEntries++;
    }
    this->tTot += length;    
    delete vect;
}

/*
 * draws a single SNP from the frequency table
 */
vector<int>* FreqTable::drawSNP(){
    boost::unordered_map<vector<int>,int>::const_iterator iter;
    double rn = utils::randomD(this->tTot);
    for (iter = this->idMap.begin(); iter != this->idMap.end(); ++iter) {
        rn -= this->branchLengths[iter->second];
        if(rn <0){
            return new vector<int>(iter->first);
        }
    }    
}

/*
 * draws a set of n SNP according to some theta
 */

SNPTable* FreqTable::drawSNP(double theta){
    int nSNP = utils::rpois(theta);
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
SNPTable* FreqTable::drawSNP(int nSNP){
    if (Parameters::verbose>99) cout << "FreqTable::drawSNP:nEntries: "<<this->nEntries<<endl;

    vector<vector<int>* >* snpFinal = new vector<vector<int>* >();
    snpFinal->reserve(nSNP);

    
    
    boost::unordered_map<vector<int>, int>::iterator it;

    //if (Parameters::verbose>99) cout << "copied freqTable: "<<this->nEntries<<endl;
    
    unsigned int* snp=utils::rmultinom(nSNP,nEntries,&branchLengths[0]);
    if (Parameters::verbose>99) cout << "got SNP pos: "<<this->nEntries<<endl;
    
    
    int nSNPDrawn = 0;
    for(it = this->idMap.begin(); it!= this->idMap.end(); ++it){        
        int i = it->second;
        nSNPDrawn+=snp[i];
        utils::printProgressBar(100*nSNPDrawn/nSNP,"drawing SNP... ");
        for(int j=0; j<snp[i];++j){
            //vector<int>* v = it->first;
            vector<int>* newSNP = new vector<int>(it->first);
            snpFinal->push_back( newSNP);
        }
    }
    delete snp;
    if (Parameters::verbose>99) cout << "got SNP: "<<endl;
    utils::rshuffle(snpFinal);
    if (Parameters::verbose>99) cout << "done randomizing: " <<endl;
    

    
    cout <<"done!"<<endl;
    SNPTable* st = new SNPTable(snpFinal,nSNP,this->nPops);
    return st;
}

int FreqTable::getNEntries(){
    return this->nEntries;
}

