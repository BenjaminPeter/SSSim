/* 
 * File:   IsolationByDistanceExpansion.cpp
 * Author: peterb
 * 
 * Created on June 11, 2012, 9:54 AM
 */

#include "IsolationByDistanceExpansion.h"
#include "IsolationByDistance.h"

using namespace std;
IsolationByDistanceExpansion::IsolationByDistanceExpansion() {
    this->k=-1;
    this->mRate=new double[4];
    this->width=-1;
    this->height=-1;
    this->expansionK=20;
    this->xStart=-1;
    this->yStart=-1;
}

IsolationByDistanceExpansion::IsolationByDistanceExpansion(const IsolationByDistanceExpansion& orig) {
    this->mRate=0;

}

IsolationByDistanceExpansion::~IsolationByDistanceExpansion() {
   // delete[] this->mRate;
}

void IsolationByDistanceExpansion::setMigrationRatesUniform(double nsaway,double nsback, 
                                                   double ewaway, double ewback){
    this->mRate[0]=nsaway;
    this->mRate[1]=nsback;
    this->mRate[2]=ewaway;
    this->mRate[3]=ewback;
}

void IsolationByDistanceExpansion::setCarCapUniform(double cc){
    this->k=cc;
}

double IsolationByDistanceExpansion::getPopSize(const Coords pos, const double t){
    return this->k;
}

double IsolationByDistanceExpansion::getMigrationRate(const int direction, const Coords pos, const double t){
    int x=pos.first;
    int y=pos.second;
    switch (direction){
        case NORTH://north
            if (y>=this->height-1)
                return 0;
            if (y>=this->yStart)
                return this->mRate[0];
            return this->mRate[1];
            break;
        case SOUTH://south
            if (y<=0)
                return 0;
            if (y<=this->yStart)
                return this->mRate[0];
            return this->mRate[1];
            break;
        case EAST://east
            if (x>=this->width-1)
                return 0;
            if (x>=this->xStart)
                return this->mRate[2];
            return this->mRate[3];
            break;
        case WEST://west
            if (x<=0)
                return 0;
            if (x<=this->xStart)
                return this->mRate[2];
            return this->mRate[3];
            break;
    }
//    if (0 <= x < this->width && 0 <= y < this->height){
    //cout<<"migration rate direction "<<direction<< " from " << x <<" / "<<y<<endl;
    //return this->mRate[direction];
    throw 10;
//    }
//    return 0;
}

bool IsolationByDistanceExpansion::isInitialized(){
    //cerr<<"starting test"<<endl;
    if (IsolationByDistance::isInitialized()){
        if (this->xStart<0){
            cerr<<"xStart not initialized"<<endl;
            return false;
        }
        if (this->yStart<0){
            cerr<<"yStart not initialized"<<endl;
            return false;
        }
        return true;
    }
    return false;
}
    