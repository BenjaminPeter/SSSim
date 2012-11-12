/* 
 * File:   IsolationByDistanceExpansion.h
 * Author: peterb
 *
 * Created on June 11, 2012, 9:54 AM
 */

#ifndef ISOLATIONBYDISTANCEEXPANSION_H
#define	ISOLATIONBYDISTANCEEXPANSION_H
#include "IsolationByDistance.h"

class IsolationByDistanceExpansion: public IsolationByDistance {
public:
    IsolationByDistanceExpansion();
    IsolationByDistanceExpansion(const IsolationByDistanceExpansion& orig);
    virtual ~IsolationByDistanceExpansion();
    
    //these two function are setting up ibd migration
    virtual void setMigrationRatesUniform(double awayNS,double backNS, double awayEW, 
                                  double backEW);
    virtual void setCarCapUniform(double cc);
    virtual double getPopSize(const Coords pos, const double t);

    virtual double getMigrationRate(const int direction, const Coords pos, const double t);
    virtual bool isInitialized();
    
    int getXStart() const {
        return xStart;
    }

    void setXStart(int xStart) {
        this->xStart = xStart;
    }

    int getYStart() const {
        return yStart;
    }

    void setYStart(int yStart) {
        this->yStart = yStart;
    }

private:
    int xStart,yStart;
};

#endif	/* ISOLATIONBYDISTANCEEXPANSION_H */

