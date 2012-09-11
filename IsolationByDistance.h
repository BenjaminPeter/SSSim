/* 
 * File:   IsolationByDistance.h
 * Author: peterb
 *
 * Created on June 4, 2012, 10:30 AM
 */

#ifndef ISOLATIONBYDISTANCE_H
#define	ISOLATIONBYDISTANCE_H

#include "MigrationScheme.h"
#include <iostream>
using namespace std;

class IsolationByDistance: public MigrationScheme {
public:
    IsolationByDistance();
    IsolationByDistance(const IsolationByDistance& orig);
    virtual ~IsolationByDistance();
    //double*** getMigrationMatrix(double time);
    //double** getCarCapMatrix(double time);
    
    
    
    //these two function are setting up ibd migration
    virtual void setMigrationRatesUniform(double north,double south, double east, 
                                  double west);
    virtual void setCarCapUniform(double cc);
    //------------------------------------------

    
    virtual vector<pair<double,int> >* getExpansionEvents();
    virtual double getPopSize(const int pos, const double t);
    virtual double getMigrationRate(const int direction, const int pos, const double t);
    virtual bool isInitialized();

    double getK() const {
        return k;
    }

    double* getMRate() const {
        return mRate;
    }

protected:
    //double*** mRates;
    double* mRate;
    //int width,height;
    double k;
};

#endif	/* ISOLATIONBYDISTANCE_H */
