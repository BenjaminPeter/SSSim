/* 
 * File:   IsolationByDistanceBarrier.cpp
 * Author: peterb
 * 
 * Created on November 12, 2012, 10:46 AM
 */

#include "IsolationByDistanceBarrier.h"
#include "Parameters.h"

IsolationByDistanceBarrier::IsolationByDistanceBarrier() {

}

IsolationByDistanceBarrier::IsolationByDistanceBarrier(const IsolationByDistanceBarrier& orig) {
}

IsolationByDistanceBarrier::~IsolationByDistanceBarrier() {
}

void IsolationByDistanceBarrier::setMigrationRatesUniform(double north, double south, double east,
        double west) {
    this->mRates.reserve(width * height);
    for (int i = 0; i<this->width; ++i) {
        for (int j = 0; j<this->height; ++j) {
            double* arr = new double[4];
            arr[NORTH] = north;
            arr[SOUTH] = south;
            arr[EAST] = east;
            arr[WEST] = west;
            this->mRates.push_back(arr);
        }
    }
    //boundaries
    for (int i = 0; i<this->width; ++i) {
        this->mRates[c1d(i, this->height - 1)][NORTH] = 0;
        this->mRates[c1d(i, 0)][SOUTH] = 0;
    }
    for (int i = 0; i<this->height; ++i) {
        this->mRates[c1d(0, i)][WEST] = 0;
        this->mRates[c1d(this->width - 1, i)][EAST] = 0;
    }
}

void IsolationByDistanceBarrier::setCarCapUniform(double cc) {
    this->popSizes.reserve(width * height);
    for (int i = 0; i<this->width; ++i) {
        for (int j = 0; j<this->height; ++j) {
            this->popSizes.push_back(cc);
        }
    }
}

double IsolationByDistanceBarrier::getPopSize(const Coords pos, const double t) {
    if (!this->inBounds(pos))
        return 0;
    return this->popSizes[c1d(pos)];
}

double IsolationByDistanceBarrier::getMigrationRate(const int direction, const Coords pos, const double t) {
    if (!this->inBounds(pos))
        return 0;
    return this->mRates[c1d(pos)][direction];
}

void IsolationByDistanceBarrier::addBarriersToMigrationScheme() {
    this->barriers = Parameters::barriers;
    for (vector<Barrier*>::iterator it = barriers.begin(); it != barriers.end(); ++it) {
        if (Parameters::verbose > 100)
            cout << "added barrier" << endl;
        for (int i = (*it)->bottomleft.first; i < (*it)->topright.first; ++i) {
            for (int j = (*it)->bottomleft.second; j < (*it)->topright.second; ++j) {
                this->popSizes[c1d(i, j)] = (*it)->k;
                this->mRates[c1d(i, j)][NORTH] = (*it)->mInside;
                this->mRates[c1d(i, j)][SOUTH] = (*it)->mInside;
                this->mRates[c1d(i, j)][EAST] = (*it)->mInside;
                this->mRates[c1d(i, j)][WEST] = (*it)->mInside;
            }
        }
        //bottom & top boundary
        for (int i = (*it)->bottomleft.first; i <= (*it)->topright.first; ++i) {
            if ((*it)->bottomleft.second > 0) {
                this->mRates[c1d(i, (*it)->bottomleft.second - 1)][NORTH] = (*it)->mBoundary;
                //cout << "set m[NORTH] to zero at" << Coords(i, (*it)->bottomleft.second - 1) << endl;
            }

            if ((*it)->topright.second < (this->height - 1)) {
                this->mRates[c1d(i, (*it)->topright.second + 1)][SOUTH] = (*it)->mBoundary;
                //cout << "set m[SOUTH] to zero at" << Coords(i, (*it)->topright.second + 1) << endl;
            }
        }

        //left & right boundary
        for (int i = (*it)->bottomleft.second; i <= (*it)->topright.second; ++i) {
            if ((*it)->bottomleft.first > 0) {
                this->mRates[c1d((*it)->bottomleft.first - 1, i)][EAST] = (*it)->mBoundary;
                //cout << "set m[EAST] to zero at" << Coords((*it)->bottomleft.first - 1, i) << endl;
            }

            if ((*it)->topright.first < (this->width - 1)) {
                this->mRates[c1d((*it)->topright.first + 1, i)][WEST] = (*it)->mBoundary;
                //cout << "set m[WEST] to zero at" << Coords((*it)->topright.first + 1, i) << endl;
            }
        }
    }
}

