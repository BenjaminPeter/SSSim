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
    
    void setStartPos(int* startPos);
    void setStartPos(int x,int y);
    void setTLag(double tLag);
    void setTStart(double tStart);
    virtual void setCarCapUniform(double cc);
    virtual void setMigrationRatesUniform(double north, double south, double east,
                                  double west);
    //double getArrivalTime(const int x, const int y);
    virtual double getArrivalTime(const Coords pos);
    void setupArrivalTimes();
    string toString(){
        cout << "SEExpansion:\twidth\theight\ttStart\ttLag\tk\tstartX\tstartY\teK"<<endl;
        cout << "\t\t";
        cout << this->width << "\t";
        cout << this->height << "\t";
        cout << this->tStart << "\t";
        cout << this->tLag << "\t";
        cout << this->k << "\t";
        cout << this->startX << "\t";
        cout << this->startY << "\t";
        cout << this->expansionK << endl;
        return "";
    }
    
    
    virtual void addBarriersToMigrationScheme(){
        cerr << "use IsolationByDistanceBarrier instead" << endl;
    }
protected:   
    double calcArrivalTime(const int x,const int y);
    
    SEExpansion(const SEExpansion& orig);
    double tStart,tLag,k;
    int startX,startY;
    
    double* mRate;
    vector<double> arrivalTimes;

};

#endif	/* SEEXPANSION_H */

