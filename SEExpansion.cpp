/* 
 * File:   SEExpansion.cpp
 * Author: peterb
 * 
 * Created on June 3, 2012, 4:04 PM
 */

#include "SEExpansion.h"
#include "Parameters.h"
#include <math.h>

struct aTime {
    Coords c;
    bool visited;
    double time;
};

bool operator<(const aTime& a1, const aTime& a2) {
    return a1.time > a2.time;

}

SEExpansion::SEExpansion() {
    this->N = -1;
    this->mRate = 0;
    this->width = 0;
    this->height = 0;
    this->sp = new StartPos();
    this->sp->expansionK = -1;
    this->sp->tLag = -1;
    this->sp->tStart = -1;
}

SEExpansion::SEExpansion(double tStart, double tLag, int k, int* startPos,
        int* dims, int expansionK) {
    this->sp->tStart = tStart;
    this->sp->tLag = tLag;
    this->N = k;
    this->sp->startX = startPos[0];
    this->sp->startY = startPos[1];
    this->width = dims[0];
    this->height = dims[1];
    this->sp->expansionK = expansionK;
}

void SEExpansion::init() {
    this->setupArrivalTimes();
}

SEExpansion::SEExpansion(const SEExpansion& orig) {
}

SEExpansion::~SEExpansion() {
    delete[] this->mRate;
    //delete this->expansionEvents;

    //delete this->arrivalTimes;
    //delete this->expansionEvents;
}

vector<ExpansionEvent>* SEExpansion::getExpansionEvents() {
    vector<ExpansionEvent>* evVect = new vector<ExpansionEvent > ();
    double tArrival = 0;
    for (int i = 0; i<this->width; i++) {
        for (int j = 0; j<this->height; j++) {
            tArrival = this->getArrivalTime(Coords(i, j));
            if (tArrival > 0) {
                //coord1d=this->coords2d1d(i,j);
                ExpansionEvent p = ExpansionEvent(Coords(i, j), tArrival,this->sp->expansionK);
                evVect->push_back(p);
            }
        }

    }

    sort(evVect->begin(), evVect->end(), &this->sorter);

    //    for (vector<pair<double,int> >::iterator it=evVect->begin();it!=evVect->end();++it){
    //        int*arr=this->coords1d2d(it->second);
    //        cout << "("<<arr[0]<<"/"<<arr[1]<<"):"<<it->first << endl;
    //        delete arr;
    //    }    

    this->expansionEvents = evVect;
    return evVect;
}

bool SEExpansion::isInitialized() {
    try {
        if (this->mRate == 0) {
            cerr << "mRate not initialized: " << endl;
            //cerr<<"n"<<this->mRate[0]<<endl;
            //cerr<<"n"<<this->mRate[1]<<endl;
            //cerr<<"n"<<this->mRate[2]<<endl;
            //cerr<<"n"<<this->mRate[3]<<endl;
            return false;
        }
        if (this->N <= 0) {
            cerr << "k not initialized" << endl;
            return false;
        }
        if (this->width <= 0) {
            cerr << "width not initialized" << endl;
            return false;
        }
        if (this->height <= 0) {
            cerr << "height not initialized" << endl;
            return false;
        }
        if (this->sp->tStart < 0) {
            cerr << "tStart not initialized" << endl;
            return false;
        }
        if (this->sp->tLag < 0) {
            cerr << "tLag not initialized" << endl;
            return false;
        }
    } catch (...) {
        cerr << "exception" << endl;
    }
    //    cerr<<"passed test"<<endl;
    return true;
}

double SEExpansion::getPopSize(const Coords pos, const double t) {
    if (t<this->getArrivalTime(pos) & t >= 0)
        return this->N;
    return 0;
}

double SEExpansion::getMigrationRate(const int direction, const Coords pos,
        const double t) {
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
    return this->mRate[direction];
    //    if (0 <= x < this->width && 0 <= y < this->height){
    //cout<<"migration rate direction "<<direction<< " from " << x <<" / "<<y<<endl;
}

void SEExpansion::setStartPos(int* startPos) {
    this->sp->startX = startPos[0];
    this->sp->startY = startPos[1];
}

void SEExpansion::setStartPos(int x, int y) {
    this->sp->startX = x;
    this->sp->startY = y;
}

void SEExpansion::setTLag(double t) {
    this->sp->tLag = t;
}

void SEExpansion::setTStart(double t) {
    this->sp->tStart = t;
}

void SEExpansion::setCarCapUniform(double cc) {
    this->N = cc;
}

void SEExpansion::setExpansionK(double ek) {
    this->sp->expansionK = ek;
}

void SEExpansion::setMigrationRatesUniform(double north, double south, double east,
        double west) {
    this->mRate = new double[4];
    this->mRate[NORTH] = north;
    this->mRate[SOUTH] = south;
    this->mRate[EAST] = east;
    this->mRate[WEST] = west;
}

double SEExpansion::getArrivalTime(const Coords pos) {
    if (pos.first > this->width || pos.second > this->height) return 0;
    if (pos.first < 0 || pos.second < 0) return 0;
    return this->arrivalTimes[c1d(pos)];
}

//double SEExpansion::getArrivalTime(const int x, const int y){
//if(x<0||x>=this->width||y<0||y>=this->height)
//    return 0;
//    return this->arrivalTimes[x][y];
//}
//now cares about boundaries

/*double SEExpansion::calcArrivalTime(int x, int y) {
    if (x == this->startX && y == this->startY)
        return 10000000000000;
    double tArrival = this->tStart - this->tLag *
            sqrt((float(x) - float(this->startX))*(float(x) - float(this->startX))
            +(float(y) - float(this->startY))*(float(y) - float(this->startY)));
    if (tArrival < 0)
        return 0;
    return tArrival;
}*/

/*void SEExpansion::setupArrivalTimes(void) {
    this->arrivalTimes.reserve(width * height);
    //this->arrivalTimes=new double[this->width*this->height];
    for (int i = 0; i<this->width; i++) {
        //this->arrivalTimes[i]=new double[this->height];
        for (int j = 0; j<this->height; j++) {
            this->arrivalTimes.push_back(this->calcArrivalTime(i, j));
        }
    }
}*/


void SEExpansion::calcArrivalTime(const int i, const int j, int xDir, int yDir) {
    this->arrivalTimes[c1d(Coords(i, j))] = max(
            this->arrivalTimes[c1d(Coords(i - xDir, j))] - sp->tLag,
            this->arrivalTimes[c1d(Coords(i, j - yDir))] - sp->tLag
            );

    this->arrivalTimes[c1d(Coords(i, j))] = max(
            this->arrivalTimes[c1d(Coords(i, j))],
            this->arrivalTimes[c1d(Coords(i - xDir, j - xDir))] - sp->tLag * sqrt(2)
            );

    for (vector<Barrier*>::iterator it = barriers.begin(); it != barriers.end(); ++it) {
        if ((*it)->isInside(Coords(i, j))&& ((*it)->mBoundary==0 || (*it)->mInside==0)) {
            this->arrivalTimes[c1d(Coords(i, j))] = 0;
        }
    }
    if (Parameters::verbose > 499) {
        cout << "set arrival time for deme " << Coords(i, j) << " to ";
        cout << this->arrivalTimes[c1d(Coords(i, j))] << endl;
    }


}

void SEExpansion::setupArrivalTimes() {

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

        dCheckPos(curPos, Coords(x + 1, y), 1., visited, unvisited);
        dCheckPos(curPos, Coords(x - 1, y), 1., visited, unvisited);
        dCheckPos(curPos, Coords(x, y + 1), 1., visited, unvisited);
        dCheckPos(curPos, Coords(x, y - 1), 1., visited, unvisited);



        dCheckPos(curPos, Coords(x - 1, y - 1), sqrt(2), visited, unvisited);
        dCheckPos(curPos, Coords(x - 1, y + 1), sqrt(2), visited, unvisited);
        dCheckPos(curPos, Coords(x + 1, y + 1), sqrt(2), visited, unvisited);
        dCheckPos(curPos, Coords(x + 1, y - 1), sqrt(2), visited, unvisited);
        if(Parameters::verbose>999)
        cout << "-------------------------------" << endl;
    }





    this->arrivalTimes[c1d(Coords(sp->startX, sp->startY))] = 1e10;
}

void SEExpansion::dCheckPos(Coords curPos, Coords newPos, double f, vector<bool> &visited,
        vector<Coords> &unvisited) {
    
    if(f<1e-10) return;
    
    //check validity of newPos
    if ( !this->inBounds( newPos ) ) {
        if (Parameters::verbose > 999)
            cout << "[dj]: not adjusting " << newPos << " because of oob" << endl;
        return;
    }
    for (vector<Barrier*>::iterator it = this->barriers.begin();
            it != barriers.end(); ++it) {
        if ((*it)->isInside(newPos) && ((*it)->mBoundary==0 || (*it)->mInside==0)) {
            if (Parameters::verbose > 999)
                cout << "[dj]: not adjusting " << newPos << " because of barrier" << endl;
            return;
        }

    }


    if (!visited[c1d(newPos)]) {
        double newTime = arrivalTimes[c1d(curPos)] - f * this->sp->tLag;
        if (newTime > arrivalTimes[c1d(newPos)]) {
            if (Parameters::verbose > 999) {
                cout << "[dj]: better path found to" << newPos << " via " << curPos;
                cout << " times:" << newTime << " vs " << arrivalTimes[c1d(newPos)] << endl;
            }
            arrivalTimes[c1d(newPos)] = newTime;


            //find queue position of newPos
            int i = unvisited.size();
            while (i > 0) {
                if (unvisited[i] == newPos)
                    break;
                i--;
            }
            if (Parameters::verbose > 999)
                cout << "[dj]: found " << newPos << " in queue at pos" << i << endl;
            while (i > 0) {
                --i;
                if (arrivalTimes[c1d(newPos)] > arrivalTimes[c1d(unvisited[i])]) {
                    unvisited[i + 1] = unvisited [i];
                    unvisited[i] = newPos;
                    // cout << "[dj]: switched " << i+1 <<"/"<<unvisited[i + 1] << "with" << i<<"/"<<unvisited[i];
                    //cout << "times are" << arrivalTimes[c1d(unvisited[i + 1])] << " <->" << arrivalTimes[c1d(unvisited[i])] << endl;
                } else {
                    break;
                }
            }
        }
    }
}















/*double SEExpansion::getCarCapMatrixEntry(double time, int x, int y){
    double tArrival = this->getArrivalTime(x,y);
    if (time>tArrival)
        return 0;
    return this->k;
}*/

//double*** SEExpansion::getMigrationMatrix(double time){}

/*double **SEExpansion::getCarCapMatrix(double time){
    double ** mat = new double*[this->width];
    for (int i=0; i<this->width;i++){
        mat[i]= new double[this->height];
        for (int j=0; j<this->height;j++){
            mat[i][j]=this->getCarCapMatrixEntry(time,i,j);
        }
    }
    return mat;
    
}*/
