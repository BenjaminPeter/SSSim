/* 
 * File:   Barrier.cpp
 * Author: peterb
 * 
 * Created on November 12, 2012, 10:49 AM
 */

#include "Barrier.h"
#include "Coords.h"

Barrier::Barrier(Coords bottomleft, Coords topright,double mBoundary, double mInside) {
    this->bottomleft = bottomleft;
    this->topright = topright;
    this->mBoundary = mBoundary;
    this->mInside = mInside;
}

Barrier::Barrier(const Barrier& orig) {
}

Barrier::~Barrier() {
}
