/* 
 * File:   FreqTable.h
 * Author: peterb
 *
 * Created on August 30, 2012, 1:02 PM
 */

#ifndef FREQTABLE_H
#define	FREQTABLE_H
#include <string>
#include <vector>
#include <boost/unordered_map.hpp>
#include "utils.h"
#include "SNPTable.h"
#include "SNP.h"


using namespace std;
class FreqTable {
public:
    FreqTable(int nPops);    
    virtual ~FreqTable();
    string toString();
    int nPops;
    boost::unordered_map<SNP, int> idMap;
    vector<double> branchLengths;
    //vector<double> lengths;
    //vector<int*> alleleFreqs;
    void addLine(double length, int* pops, bool checkShared=false);
    SNP* drawSNP();
    SNPTable* drawSNP(int nSNP);
    SNPTable* drawSNP(double theta);

    double getTTot() const {
        return tTot;
    }
    
    int getNEntries();
private:
    FreqTable(const FreqTable& orig);
    double tTot;
    int nEntries;
    
};

#endif	/* FREQTABLE_H */

