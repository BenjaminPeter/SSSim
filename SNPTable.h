/* 
 * File:   SNPTable.h
 * Author: peterb
 *
 * Created on September 12, 2012, 12:22 PM
 */

#ifndef SNPTABLE_H
#define	SNPTABLE_H

class FreqTable;

#include <vector>
#include "FreqTable.h"
#include "SFS.h"

using namespace std;

class SNPTable {
public:
    
    //constructor & ddestructor
    SNPTable();
    SNPTable(vector<vector<int>* >* snp, int nSNP, int nPops,bool resample = false);
    virtual ~SNPTable();
    
    //generators    
    SNPTable* getBootstrapResample();
    vector<SNPTable*>* getBootstrapResample(int nResamples);
    
    //calculate statistics
    vector<double>* getFST();
    vector<double>* getPsi();
    vector<double>* getDeltaH();
    vector<double>* getHeterozygosity();
    
    //get SFS
    SFS* make2DSFS(int pop1, int pop2);
    SFS* make1DSFS(int pop);
    vector<vector<int>* >* snp;
    
private:
    vector<double>* heterozygosity,*pi;
    vector<SFS*> sfs; 
    bool calcH,calcSFS;
    int nPops;
    int nSNP;
    SNPTable(const SNPTable& orig);
    void calculateAllSFS();
    bool isResample;
    
};

#endif	/* SNPTABLE_H */

