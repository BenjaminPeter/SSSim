/* 
 * File:   Sample.h
 * Author: peterb
 *
 * Created on June 1, 2012, 10:53 AM
 */

#ifndef SAMPLE_H
#define	SAMPLE_H

#include <vector>
#include "Lineage.h"
#include "InternalLineage.h"
#include "LineageTemplate.h"
#include "TerminalLineage.h"
//#include "Coords.h"
#include "utils.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;
class Sample {
public:
    //static int ns;
    Sample(int x, int y, vector<Lineage*> lineages);
    Sample(int* pos, vector<Lineage*> lineages);
    Sample(const Sample& orig,int nPops);    
    //Sample(Coords* pos, vector<Lineage*> lineages);

    virtual ~Sample();
    void merge2lineages(int l1,int l2, double curTime);
    void merge2randomLineages(double curTime);
    Lineage* getRandomLineageForMigration();
    Lineage* getRandomLineage();
    void addLineage(Lineage* lineage);
    string toString();
    string toStringShort();
    utils* ut;
    int getX();
    int getY();
    int getNlineages(){return this->nLineages;}
    //Coords* getCoords(){return this->coords;}
    void deleteAllLineages();
private:
    
    int x,y,nLineages;
    vector<Lineage*> lineages;
    //Coords* coords;

};

#endif	/* SAMPLE_H */

