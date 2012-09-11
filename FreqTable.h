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

using namespace std;
class FreqTable {
public:
    FreqTable(int nPops);    
    virtual ~FreqTable();
    string toString();
    int nPops;
    boost::unordered_map<vector<int>, double> freqs;
    //vector<double> lengths;
    //vector<int*> alleleFreqs;
    void addLine(double length, int* pops);
private:
    FreqTable(const FreqTable& orig);
    double tTot;
};

#endif	/* FREQTABLE_H */
