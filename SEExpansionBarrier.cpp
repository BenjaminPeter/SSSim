/* 
 * File:   SEExpansionBarrier.cpp
 * Author: peterb
 * 
 * Created on November 13, 2012, 10:13 AM
 */

#include "SEExpansionBarrier.h"
#include "Parameters.h"

SEExpansionBarrier::SEExpansionBarrier() {
}

SEExpansionBarrier::SEExpansionBarrier(const SEExpansionBarrier& orig) {
}

SEExpansionBarrier::~SEExpansionBarrier() {
    delete[] this->mRate;

}

void SEExpansionBarrier::setMigrationRatesUniform(double north, double south,
        double east, double west) {
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

void SEExpansionBarrier::setCarCapUniform(double cc) {
    this->popSizes.reserve(width * height);
    for (int i = 0; i<this->width; ++i) {
        for (int j = 0; j<this->height; ++j) {
            this->popSizes.push_back(cc);
        }
    }
}

double SEExpansionBarrier::getPopSize(const Coords pos, const double t) {
    if (!this->inBounds(pos))
        return 0;
    //cout << pos << c1d(pos) << this->popSizes.size() << endl;
    return this->popSizes[c1d(pos)];
}

double SEExpansionBarrier::getMigrationRate(const int direction, const Coords pos, const double t) {
    if (!this->inBounds(pos))
        return 0;
    return this->mRates[c1d(pos)][direction];
}

void SEExpansionBarrier::addBarriersToMigrationScheme() {
    cout << "adding barriers" << endl;
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
    }
}