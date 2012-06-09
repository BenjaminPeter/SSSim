/* 
 * File:   IsolationByDistance.cpp
 * Author: peterb
 * 
 * Created on June 4, 2012, 10:30 AM
 */

#include "IsolationByDistance.h"


IsolationByDistance::IsolationByDistance() {
    this->k=-1;
    this->mRate=new double[4];
    this->width=-1;
    this->height=-1;
    this->expansionK=20;
}

IsolationByDistance::IsolationByDistance(const IsolationByDistance& orig) {
    this->mRate=NULL;
}

IsolationByDistance::~IsolationByDistance() {
    delete[] this->mRate;
}

//double*** IsolationByDistance::getMigrationMatrix(double time){
//}
//double** IsolationByDistance::getCarCapMatrix(double time){}


vector<pair<double,int> >* IsolationByDistance::getExpansionEvents(){
    
    return new vector<pair<double,int> >(0);
    /*v->push_back(pair<double,int>(0.02,0));
    v->push_back(pair<double,int>(0.2,4));
    v->push_back(pair<double,int>(10,3));
    v->push_back(pair<double,int>(100,3));
    sort(v->begin(),v->end(),&this->sorter);
    cout <<"adding expansion events:"<<v->size()<<endl;
    for (vector<pair<double,int> >::iterator it=v->begin();it!=v->end();++it){
        cout << it->first << "/"<<it->second<<endl;
    }
    return v;*/
}

void IsolationByDistance::setMigrationRatesUniform(double north,double south, 
                                                   double east, double west){
    /*int dirs[4]={north,south,east,west};
    this->mRates = new double**[4];
    for(int d=0; d<4;d++){
        this->mRates[d] = new double*[this->width];
        for (int i=0;i<this->width; i++){
            this->mRates[d][i]=new double[this->height];
            for (int j=0;j<this->height;j++){
                this->mRates[d][i][j]=dirs[d];
            }
        }
    }*/
    this->mRate[NORTH]=north;
    this->mRate[SOUTH]=south;
    this->mRate[EAST]=east;
    this->mRate[WEST]=west;
}

void IsolationByDistance::setCarCapUniform(double cc){
    this->k=cc;
}

double IsolationByDistance::getPopSize(const int pos, const double t){
    /* int* arr=this->coords1d2d(pos);
    int x=arr[0];
    int y=arr[1];
    delete[] arr;
    
    if (0 <= x && x < this->width && 0 <= y && y < this->height){
        return this->k;
    }
    return 0;*/
    return this->k;
}

double IsolationByDistance::getMigrationRate(const int direction, const int pos, const double t){
    int* arr=this->coords1d2d(pos);
    int x=arr[0];
    int y=arr[1];
    delete[] arr;
    switch (direction){
        case NORTH://north
            if (y>=this->height-1)
                return 0;
            break;
        case SOUTH://south
            if (y<=0)
                return 0;
            break;
        case EAST://east
            if (x>=this->width-1)
                return 0;
            break;
        case WEST://west
            if (x<=0)
                return 0;
            break;
    }
//    if (0 <= x < this->width && 0 <= y < this->height){
    //cout<<"migration rate direction "<<direction<< " from " << x <<" / "<<y<<endl;
    return this->mRate[direction];
//    }
//    return 0;
}


bool IsolationByDistance::isInitialized(){
    //cerr<<"starting test"<<endl;
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
            
    }catch(...){
        cerr <<"exception"<<endl;
    }
    //cerr<<"passed test"<<endl;
    return true;
}