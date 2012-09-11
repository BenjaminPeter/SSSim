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
#include <algorithm>

using namespace std;
class MigrationScheme {
public:
    MigrationScheme(){
        this->expansionEvents=0;
    }
    virtual ~MigrationScheme(){
        //delete this->expansionEvents;
    }
    static bool sorter (const pair<double,int>& x, const pair<double,int>& y) { return x.first>y.first; }
    //virtual double*** getMigrationMatrix(double time)=0;
    //virtual double** getCarCapMatrix(double time)=0;
    static const int NORTH=0;
    static const int SOUTH=1;
    static const int EAST=2;
    static const int WEST=3;
    virtual vector<pair<double,int> >* getExpansionEvents(){
        if (this->expansionEvents==NULL){
            return new vector<pair<double,int> >();
        }
        return this->expansionEvents;
    }
    virtual bool isInitialized(void)=0;
    virtual double getPopSize(const int x, const int y, const double t){
        return this->getPopSize(this->coords2d1d(x,y),t);
    }
    virtual double getPopSize(const int pos, const double t)=0;

    virtual double getMigrationRate(const int direction, const int x, 
                                    const int y, const double t){
        return this->getMigrationRate(direction,this->coords2d1d(x,y),t);
    };
    virtual double getMigrationRate(const int direction, const int pos, 
                                    const double t)=0;
    //virtual double getArrivalTime(const int x, const int y){
    //    return 1000000000;
    //}
    
    virtual double getArrivalTime(const int pos){
        return 10000000000;
    }
    int* coords1d2d(int x){
        int* arr = new int[2]; 
        arr[0]=x/this->height; 
        arr[1]=x%this->height;
        //printf("%d=[%d*%d+%d]\n",x,arr[0],this->height,arr[1]);
        return arr;
    }
    int coords2d1d(int x,int y){return x*this->height+y;}
    int coords2d1d(int* arr){return arr[0]*this->height+arr[1];}
    
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

    virtual int getExpansionK() const {
        return expansionK;
    }
    
    virtual int getExpansionK(int x, int y) const{
        return expansionK;
    }

    virtual void setExpansionK(int expansionK) {
        this->expansionK = expansionK;
    }

protected:
    int width,height,expansionK;
    vector<pair<double,int> >* expansionEvents;
};

#endif	/* MIGRATIONSCHEME_H */

