/* 
 * File:   IsolationByDistanceBarrier.h
 * Author: peterb
 *
 * Created on November 12, 2012, 10:46 AM
 */

#ifndef ISOLATIONBYDISTANCEBARRIER_H
#define	ISOLATIONBYDISTANCEBARRIER_H
#include "IsolationByDistance.h"

using namespace std;

class IsolationByDistanceBarrier: public IsolationByDistance{
public:
    IsolationByDistanceBarrier();
    IsolationByDistanceBarrier(const IsolationByDistanceBarrier& orig);
    virtual ~IsolationByDistanceBarrier();
    
    virtual void setMigrationRatesUniform(double north,double south, double east, 
                                  double west);
    virtual void setCarCapUniform(double cc);
    
    virtual double getPopSize(const Coords pos, const double t);
    virtual double getMigrationRate(const int direction, const Coords pos, const double t);
    virtual bool isInitialized(){return true;}
    virtual void addBarriersToMigrationScheme();

private:
    vector<double*> mRates;
    vector<double> popSizes;
    

};

#endif	/* ISOLATIONBYDISTANCEBARRIER_H */

