/* 
 * File:   FreqTable.cpp
 * Author: peterb
 * 
 * Created on August 30, 2012, 1:02 PM
 */

#include <sstream>
#include <boost/unordered_map.hpp>
#include <vector>
#include "FreqTable.h"

FreqTable::FreqTable(int nPops) {
    this->nPops = nPops;
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
    cout <<"tTot:"<<"\t"<<this->tTot<<endl;
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

void FreqTable::addLine(double length, int* pops) {
    vector<int> vect(pops, pops + this->nPops);
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