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
    //delete[] this->mRate;
    for(int i=0; i<this->mRates.size();++i){
        delete[] this->mRates[i];
    }

}

void SEExpansionBarrier::setMigrationRatesUniform(double north, double south,
        double east, double west) {

    this->mRate = new double[4];
    this->mRate[NORTH] = north;
    this->mRate[SOUTH] = south;
    this->mRate[EAST] = east;
    this->mRate[WEST] = west;


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

void SEExpansionBarrier::setCarCapGamma(double cc, double alpha) {
    this->popSizes.reserve(width * height);
    for (int i = 0; i<this->width; ++i) {
        for (int j = 0; j<this->height; ++j) {
            double d = utils::rgamma(cc / alpha, cc / alpha);
            this->popSizes.push_back(d);
            cout << Coords(i, j) << ":" << d << endl;
        }
    }
}

double SEExpansionBarrier::getPopSize(const Coords pos, const double t) {
    if (!this->inBounds(pos))
        return 0;

    if (t<this->getArrivalTime(pos) & t >= 0)
        //cout << pos << c1d(pos) << this->popSizes.size() << endl;
        return this->popSizes[c1d(pos)];


    return 0;
}

double SEExpansionBarrier::getMigrationRate(const int direction, const Coords pos, const double t) {
    if (!this->inBounds(pos))
        return 0;

    int x = pos.first;
    int y = pos.second;
    if (t>this->getArrivalTime(pos))
        return 0;
    switch (direction) {
        case NORTH://north
            if (y >= this->height - 1 || t>this->getArrivalTime(Coords(x, y + 1)))
                return 0;
            break;
        case SOUTH://south
            if (y <= 0 || t>this->getArrivalTime(Coords(x, y - 1)))
                return 0;
            break;
        case EAST://east
            if (x >= this->width - 1 || t>this->getArrivalTime(Coords(x + 1, y)))
                return 0;
            break;
        case WEST://west
            if (x <= 0 || t>this->getArrivalTime(Coords(x - 1, y)))
                return 0;
            break;
    }
    return this->mRates[c1d(pos)][direction];

}

void SEExpansionBarrier::addBarriersToMigrationScheme() {
    this->barriers = Parameters::barriers;
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
                this->arrivalTimes[c1d(i, j)] == 0;
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

    //this->dumpMigrationMatrix();
}

void SEExpansionBarrier::dumpMigrationMatrix() const {
    for (int i = 0; i<this->width; ++i) {
        for (int j = 0; j<this->height; ++j) {

            cout << this->popSizes[c1d(i, j)] << " ";
        }
        cout << endl;
    }
}

void SEExpansionBarrier::setupArrivalTimes() {

    this->arrivalTimes.reserve(width * height);
    vector<Coords> unvisited;
    vector<bool> visited;
    visited.reserve(width * height);
    //    unvisited.resize(width*height);

    for (int i = 0; i<this->width; i++) {
        for (int j = 0; j<this->height; j++) {
            this->arrivalTimes.push_back(0);
            visited.push_back(false);
            unvisited.push_back(Coords(i, j));
        }
    }

    //start with starting pos
    unvisited[0] = Coords(sp->startX, sp->startY);
    unvisited[c1d(sp->startX, sp->startY)] = Coords(0, 0);
    arrivalTimes[c1d(sp->startX, sp->startY)] = this->sp->tStart;
    int nVisited = width*height;


    while (nVisited > 0) {
        nVisited--;
        if (Parameters::verbose > 999)
            cout << "[dj]: nodes left" << nVisited << endl;
        Coords curPos = (*unvisited.begin());
        visited[c1d(curPos)] = true;
        unvisited.erase(unvisited.begin());
        if (Parameters::verbose > 999) {
            cout << "-------------------------------" << endl;
            cout << "[dj]: visiting " << curPos << "time:" << arrivalTimes[c1d(curPos)] << endl;
        }

        //if (arrivalTimes[curPos] == 0) {
        //    cerr << "could not reach all demes:" << curNode.c <<endl;
        //}

        int x = curPos.first;
        int y = curPos.second;

        int cp1d = c1d(curPos);
        dCheckPos(curPos, Coords(x + 1, y), 1. / this->mRates[cp1d][EAST] * this->mRate[EAST], visited, unvisited);
        dCheckPos(curPos, Coords(x - 1, y), 1. / this->mRates[cp1d][WEST] * this->mRate[WEST], visited, unvisited);
        dCheckPos(curPos, Coords(x, y + 1), 1. / this->mRates[cp1d][NORTH] * this->mRate[NORTH], visited, unvisited);
        dCheckPos(curPos, Coords(x, y - 1), 1. / this->mRates[cp1d][SOUTH] * this->mRate[SOUTH], visited, unvisited);



        dCheckPos(curPos, Coords(x - 1, y - 1), sqrt(2) / ((this->mRates[cp1d][WEST]/this->mRate[WEST] + this->mRates[cp1d][SOUTH]/this->mRate[SOUTH]) / 2), visited, unvisited);
        dCheckPos(curPos, Coords(x - 1, y + 1), sqrt(2) / ((this->mRates[cp1d][WEST]/this->mRate[WEST] + this->mRates[cp1d][NORTH]/this->mRate[NORTH]) / 2), visited, unvisited);
        dCheckPos(curPos, Coords(x + 1, y + 1), sqrt(2) / ((this->mRates[cp1d][EAST]/this->mRate[EAST] + this->mRates[cp1d][SOUTH]/this->mRate[SOUTH]) / 2), visited, unvisited);
        dCheckPos(curPos, Coords(x + 1, y - 1), sqrt(2) / ((this->mRates[cp1d][EAST]/this->mRate[EAST] + this->mRates[cp1d][NORTH]/this->mRate[NORTH]) / 2), visited, unvisited);
        if (Parameters::verbose > 999)
            cout << "-------------------------------" << endl;
    }





    this->arrivalTimes[c1d(Coords(sp->startX, sp->startY))] = 1e10;
}
