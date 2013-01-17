/* 
 * File:   SEExpansionBarrier.h
 * Author: peterb
 *
 * Created on November 13, 2012, 10:13 AM
 */

#ifndef SEEXPANSIONBARRIER_H
#define	SEEXPANSIONBARRIER_H
#include "SEExpansion.h"
#include <vector>

class Parameters;
class SEExpansionBarrier : public SEExpansion {
public:
    SEExpansionBarrier();
    SEExpansionBarrier(const SEExpansionBarrier& orig);
    virtual ~SEExpansionBarrier();

    virtual void setMigrationRatesUniform(double north, double south, double east,
            double west);
    virtual void setCarCapUniform(double cc);
    virtual void setCarCapGamma(double cc, double alpha);

    virtual double getPopSize(const Coords pos, const double t);
    virtual double getMigrationRate(const int direction, const Coords pos, const double t);
    virtual bool isInitialized(){return true;}
    virtual void addBarriersToMigrationScheme();
    virtual void setupArrivalTimes();
    
    void dumpMigrationMatrix() const;

protected:
    vector<double*> mRates;
    vector<double> popSizes;
};

#endif	/* SEEXPANSIONBARRIER_H */

