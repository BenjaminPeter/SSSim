/* 
 * File:   SEExpansion.cpp
 * Author: peterb
 * 
 * Created on June 3, 2012, 4:04 PM
 */

#include "SEExpansion.h"
#include <math.h>


SEExpansion::SEExpansion(){
    this->k=-1;
    this->mRate=0;
    this->width=0;
    this->height=0;
    this->expansionK=-1;
    this->tLag=-1;
    this->tStart=-1;
}
SEExpansion::SEExpansion(double tStart, double tLag, int k, int* startPos,
                        int* dims, int expansionK) {
    this->tStart        =       tStart;
    this->tLag          =       tLag;
    this->k             =       k;
    this->startX        =       startPos[0];
    this->startY        =       startPos[1];
    this->width         =       dims[0];
    this->height        =       dims[1];
    this->expansionK    =       expansionK;
}

SEExpansion::SEExpansion(const SEExpansion& orig) {
}

SEExpansion::~SEExpansion() {
    delete[] this->mRate;
    //delete this->expansionEvents;
   
    delete[] this->arrivalTimes;
}


vector<pair<double,int> >* SEExpansion::getExpansionEvents(){
    vector<pair<double,int> >* evVect = new vector<pair<double,int> >(0);
    double tArrival=0;
    int coord1d=0;
    for (int i=0; i<this->width;i++){
        for (int j=0; j<this->height;j++){
            tArrival=this->getArrivalTime(i*this->height+j);
            if (tArrival>0){
                coord1d=this->coords2d1d(i,j);
                pair<double,int> p = pair<double,int>(tArrival,coord1d);
                evVect->push_back(p);
            }
        }
    
    }

    sort(evVect->begin(),evVect->end(),&this->sorter);
    
//    for (vector<pair<double,int> >::iterator it=evVect->begin();it!=evVect->end();++it){
//        int*arr=this->coords1d2d(it->second);
//        cout << "("<<arr[0]<<"/"<<arr[1]<<"):"<<it->first << endl;
//        delete arr;
//    }    
    
    this->expansionEvents=evVect;
    return evVect;    
}


bool SEExpansion::isInitialized(){
    try{
        if (this->mRate==0){
            cerr<<"mRate not initialized: "<<endl;
            //cerr<<"n"<<this->mRate[0]<<endl;
            //cerr<<"n"<<this->mRate[1]<<endl;
            //cerr<<"n"<<this->mRate[2]<<endl;
            //cerr<<"n"<<this->mRate[3]<<endl;
            return false;
        }
        if (this->k <= 0){
            cerr<<"k not initialized"<<endl;
            return false;
        }
        if (this->width<=0){
            cerr<<"width not initialized"<<endl;
            return false;
        }
        if (this->height<=0){
            cerr<<"height not initialized"<<endl;
            return false;
        }
        if (this->tStart < 0){
            cerr<<"tStart not initialized"<<endl;
            return false;
        }            
        if (this->tLag < 0){
            cerr<<"tLag not initialized"<<endl;
            return false;
        }            
    }catch(...){
        cerr <<"exception"<<endl;
    }
//    cerr<<"passed test"<<endl;
    return true;    
}



    double SEExpansion::getPopSize(const int pos, const double t){            
        if (t<this->getArrivalTime(pos)&t>0)
            return this->k;
        return 0;        
    }
    
double SEExpansion::getMigrationRate(const int direction, const int pos, 
                                     const double t){
    int* arr=this->coords1d2d(pos);

    int x=arr[0];
    int y=arr[1];
    delete[] arr;
    if (t>this->getArrivalTime(pos))
        return 0;
    switch (direction){
        case NORTH://north
            if (y>=this->height-1||t>this->getArrivalTime(pos+1))
                return 0;              
            break;
        case SOUTH://south
            if (y<=0||t>this->getArrivalTime(pos-1))
                return 0;
            break;
        case EAST://east
            if (x>=this->width-1||t>this->getArrivalTime(pos+this->height))
                return 0;
            break;
        case WEST://west
            if (x<=0||t>this->getArrivalTime(pos-this->height))
                return 0;
            break;
    }
    return this->mRate[direction];
//    if (0 <= x < this->width && 0 <= y < this->height){
    //cout<<"migration rate direction "<<direction<< " from " << x <<" / "<<y<<endl;
}
    
void SEExpansion::setStartPos(int* startPos){
    this->startX=startPos[0];
    this->startY=startPos[1];
}
void SEExpansion::setStartPos(int x,int y){
    this->startX=x;
    this->startY=y;
}

void SEExpansion::setTLag(double t){
    this->tLag=t;
}

void SEExpansion::setTStart(double t){
    this->tStart=t;
}
void SEExpansion::setCarCapUniform(double cc){
    this->k=cc;
}
void SEExpansion::setMigrationRatesUniform(double north, double south, double east,
                                  double west){
    this->mRate=new double[4];
    this->mRate[NORTH]=north;
    this->mRate[SOUTH]=south;
    this->mRate[EAST]=east;
    this->mRate[WEST]=west;
}

double SEExpansion::getArrivalTime(const int pos){
    if( pos>=this->width*this->height) return 0;
    if( pos<0) return 0;
    return this->arrivalTimes[pos];
}

//double SEExpansion::getArrivalTime(const int x, const int y){
    //if(x<0||x>=this->width||y<0||y>=this->height)
    //    return 0;
//    return this->arrivalTimes[x][y];
//}
//does not care about boundaries
double SEExpansion::calcArrivalTime(int x, int y){
    if (x==this->startX&&y==this->startY)
        return 10000000000000;
    double tArrival = this->tStart-this->tLag*
        sqrt((float(x)-float(this->startX))*(float(x)-float(this->startX))
            +(float(y)-float(this->startY))*(float(y)-float(this->startY)));
    if (tArrival<0)
        return 0;
    return tArrival;
}


void SEExpansion::setupArrivalTimes(void){
    int h=this->height;
    this->arrivalTimes=new double[this->width*this->height];
    for (int i=0;i<this->width;i++){
        //this->arrivalTimes[i]=new double[this->height];
        for (int j=0;j<this->height;j++){
            this->arrivalTimes[i*h+j]=this->calcArrivalTime(i,j);        
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