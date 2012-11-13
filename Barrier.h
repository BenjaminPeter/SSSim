/* 
 * File:   Barrier.h
 * Author: peterb
 *
 * Created on November 12, 2012, 10:49 AM
 */

#ifndef BARRIER_H
#define	BARRIER_H

#include "Coords.h"

class Barrier {
public:
    Barrier(Coords bottomleft, Coords topright, double mBoundary=0, double mInside=0);
    Barrier(const Barrier& orig);
    virtual ~Barrier();
    Coords bottomleft, topright; //coords of bottom left and top right corner
    double k; //population size
    double mBoundary, mInside;
private:        
    double tStart,tEnd; //time of arrival/end of barrier;

};

#endif	/* BARRIER_H */

