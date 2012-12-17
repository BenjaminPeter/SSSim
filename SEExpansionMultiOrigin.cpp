/* 
 * File:   SEExpansionMultiOrigin.cpp
 * Author: peterb
 * 
 * Created on December 12, 2012, 12:17 PM
 */

#include "SEExpansionMultiOrigin.h"
#include "Parameters.h"

SEExpansionMultiOrigin::SEExpansionMultiOrigin() {
}

SEExpansionMultiOrigin::SEExpansionMultiOrigin(const SEExpansionMultiOrigin& orig) {
}

SEExpansionMultiOrigin::~SEExpansionMultiOrigin() {
}

vector<ExpansionEvent>* SEExpansionMultiOrigin::getExpansionEvents() {
    vector<ExpansionEvent>* evVect = new vector<ExpansionEvent > ();
    double tArrival = 0;
    for (int i = 0; i<this->width; i++) {
        for (int j = 0; j<this->height; j++) {
            tArrival = this->getArrivalTime(Coords(i, j));
            if (tArrival > 0) {
                //coord1d=this->coords2d1d(i,j);

                //difference to parent: add k=-200 for founders
                bool isStart = false;
                for (vector<StartPos*>::iterator it = this->startPositions.begin();
                        it != this->startPositions.end(); ++it) {
                    if ((*it)->startX == i && (*it)->startY == j) {
                        isStart = true;
                        ExpansionEvent p = ExpansionEvent(Coords(i, j), tArrival, -200);
                        evVect->push_back(p);
                        break;
                    }
                }
                if (!isStart) {
                    ExpansionEvent p = ExpansionEvent(Coords(i, j), tArrival, this->sp->expansionK);
                    evVect->push_back(p);
                }

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

void SEExpansionMultiOrigin::setupArrivalTimes() {
    vector<vector<double> >tempArrivalTimes;
    for (vector<StartPos*>::iterator it = this->startPositions.begin();
            it != this->startPositions.end(); ++it) {
        vector<double> newArrivalTime = *(new vector<double>());
        this->arrivalTimes = newArrivalTime;
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
        this->sp = (*it);
        unvisited[0] = Coords(sp->startX, sp->startY);
        unvisited[c1d(sp->startX, sp->startY)] = Coords(0, 0);
        this->arrivalTimes[c1d(sp->startX, sp->startY)] = sp->tStart;
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
                cout << "[dj]: visiting " << curPos << "time:" << this->arrivalTimes[c1d(curPos)] << endl;
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
            if (Parameters::verbose > 999)
                cout << "-------------------------------" << endl;
        }

        tempArrivalTimes.push_back(this->arrivalTimes);
    }

    this->arrivalTimes = vector<double>();
    for (int i = 0; i < tempArrivalTimes[0].size(); ++i) { //outer loops is over all demes
        double curMax = 0;


        if (Parameters::verbose > 499) {
            Coords c = c2d(i);
            cout << c << ":";
        }


        for (int j = 0; j < tempArrivalTimes.size(); ++j) { //inner loop over all startPos
            if (Parameters::verbose > 499) {
                cout << tempArrivalTimes[j][i] << "\t";
            }
            if (tempArrivalTimes[j][i] > curMax) {
                curMax = tempArrivalTimes[j][i];
            }
        }


        if (Parameters::verbose > 499) {
            cout << "selected: " << curMax << endl;
        }

        this->arrivalTimes.push_back(curMax);
    }



    //this->arrivalTimes[c1d(Coords(this->sp->startX, this->sp->startY))] = 1e10;
}