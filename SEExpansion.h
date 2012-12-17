/* 
 * File:   SEExpansion.h
 * Author: peterb
 *
 * Created on June 3, 2012, 4:04 PM
 */

#ifndef SEEXPANSION_H
#define	SEEXPANSION_H

#include "MigrationScheme.h"
#include <iostream>
#include <sstream>
#include <set>
#include <list>
#include "StartPos.h"

using namespace std;
class SEExpansion : public MigrationScheme {
public:
    SEExpansion();
    SEExpansion(double tStart, double tLag, int k,int* startPos, int* dim,int expansionK);

    virtual ~SEExpansion();   
    
    virtual vector<ExpansionEvent>* getExpansionEvents(); //key is time when event happens,
    virtual bool isInitialized(void);
    virtual double getPopSize(const Coords pos, const double t);
    virtual double getMigrationRate(const int direction, const Coords pos,
                                    const double t);
    
    virtual void setStartPos(int* startPos);
    virtual void setStartPos(int x,int y);
    virtual void setTLag(double tLag);
    virtual void setTStart(double tStart);
    virtual void setExpansionK(double ek);
    virtual void setCarCapUniform(double cc);
    virtual void setMigrationRatesUniform(double north, double south, double east,
                                  double west);
    //double getArrivalTime(const int x, const int y);
    virtual double getArrivalTime(const Coords pos);
    virtual void setupArrivalTimes();
    string toString(){
        cout << "SEExpansion:\twidth\theight\ttStart\ttLag\tk\tstartX\tstartY\teK"<<endl;
        cout << "\t\t";
        cout << this->width << "\t";
        cout << this->height << "\t";
        cout << sp->tStart << "\t";
        cout << sp->tLag << "\t";
        cout << this->N << "\t";
        cout << sp->startX << "\t";
        cout << sp->startY << "\t";
        cout << sp->expansionK << endl;
        return "";
    }
    
    virtual void init();
    virtual void addBarriersToMigrationScheme(){}
protected:   
    void calcArrivalTime(const int x,const int y, int xDir=1, int yDir=1);
    
    SEExpansion(const SEExpansion& orig);
    StartPos* sp;
    double N;
    double* mRate;
    vector<double> arrivalTimes;
protected:
    void dCheckPos(Coords curPos, Coords newPos, double mult, vector<bool> &visited,
            vector<Coords> &unvisited);

};

#endif	/* SEEXPANSION_H */

