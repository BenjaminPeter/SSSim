/* 
 * File:   MigrationScheme.h
 * Author: peterb
 *
 * Created on June 4, 2012, 10:21 AM
 */

#ifndef MIGRATIONSCHEME_H
#define	MIGRATIONSCHEME_H
#include <vector>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include "Coords.h"
#include "Barrier.h"

using namespace std;
struct ExpansionEvent{
    Coords coords;
    double time;
    int k;
    ExpansionEvent(Coords c, double t, int k){
        this->coords =c;
        this->time = t;
        this->k =k;
    }
};


class MigrationScheme {
public:

    //constructors & destructors

    MigrationScheme() {
        this->expansionEvents = 0;
    }

    virtual ~MigrationScheme() {
        //delete this->expansionEvents;
    }




    //migration directions
    static const int NORTH = 0;
    static const int SOUTH = 1;
    static const int EAST = 2;
    static const int WEST = 3;

    static bool sorter(const ExpansionEvent& x, const ExpansionEvent& y) {
        return x.time > y.time;
    }

    virtual vector<ExpansionEvent >* getExpansionEvents() {
        if (this->expansionEvents == NULL) {
            return new vector<ExpansionEvent > ();
        }
        return this->expansionEvents;
    }

    virtual void init()=0;
    
    virtual bool isInitialized(void) {
        cerr << "x" << std::endl;
    };

    int c1d(const Coords c) const{
        return c.first * this->height + c.second;
    }

    int c1d(const int x, const int y) const {
        return x * this->height + y;
    }
    
    Coords c2d(const int c) const{
        int x = c / this->height;
        int y = c % this->height;
        return Coords(x,y);
    }
    //getters for population size and migration rate

    /*virtual double getPopSize(const int x, const int y, const double t) {
        return this->getPopSize(this->coords2d1d(x, y), t);
    }*/
    virtual double getPopSize(const Coords pos, const double t) = 0;

    /*virtual double getMigrationRate(const int direction, const int x,
            const int y, const double t) {
        return this->getMigrationRate(direction, this->coords2d1d(x, y), t);
    };*/
    virtual double getMigrationRate(const int direction, const Coords pos,
            const double t) = 0;

    //by default, arrival happened a very long time ago

    virtual double getArrivalTime(const Coords pos) {
        return 10000000000;
    }


    //conversion from 1 to 2d

    /*    int* coords1d2d(int x) {
            int* arr = new int[2];
            arr[0] = x / this->height;
            arr[1] = x % this->height;
            //printf("%d=[%d*%d+%d]\n",x,arr[0],this->height,arr[1]);
            return arr;
        }

        int coords2d1d(int x, int y) {
            return x * this->height + y;
        }

        int coords2d1d(int* arr) {
            return arr[0] * this->height + arr[1];
        }*/

    //getters and setters for habitat shape

    int getHeight() const {
        return height;
    }

    void setHeight(int height) {
        this->height = height;
    }

    int getWidth() const {
        return width;
    }

    void setWidth(int width) {
        this->width = width;
    }


    //barriers
    virtual void addBarriersToMigrationScheme() = 0;

    virtual bool inBounds(Coords c){
        return (0 <= c.first && c.first < width && 0 <= c.second && c.second < height);
    }
protected:
    int width, height;
    vector<ExpansionEvent>* expansionEvents;
    vector<Barrier*> barriers;

    virtual void newBarrier(Barrier* b) {
        this->barriers.push_back(b);
    }
};

#endif /*MIGRATIONSCHEME_H */

