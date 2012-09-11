/* 
 * File:   SFS.cpp
 * Author: peterb
 * 
 * Created on June 7, 2012, 5:22 PM
 */

#include "SFS.h"
#include <iostream>
#include <sstream>

SFS::SFS(int max1, int max2) {
    this->size1=max1;
    this->size2=max2;
    this->tTot=0;
    this->normalized=true;
    
    this->data= new double*[max1+1];
    for(int i=0;i<max1+1;i++){
        data[i]=new double[max2+1];
        for(int j=0;j<max2+1;j++){
            data[i][j]=0;
        }
    }
}
SFS::SFS(int max1) {
    this->size1=max1;
    this->size2=0;
    this->tTot=0;
    this->normalized=true;
    
    this->data= new double*[max1+1];
    for(int i=0;i<max1+1;i++){
        data[i]=new double[1];
        for(int j=0;j<1;j++){
            data[i][j]=0;
        }
    }
}

SFS::SFS(const SFS& orig) {
    this->size1=orig.size1;
    this->size2=orig.size2;
    this->tTot=orig.tTot;
    this->normalized=orig.normalized;
    this->data= new double*[size1+1];
    
    for(int i=0;i<size1+1;i++){
        data[i]=new double[size2+1];
        for(int j=0;j<size2+1;j++){
            data[i][j]=orig.data[i][j];
        }
    }
            
}

SFS::~SFS() {
    for(int i=0;i<this->size1+1;i++){
        
        delete[] data[i];
    }
    delete[] data;
}

    SFS SFS::operator+(const SFS &otherSFS){
        if ( this->size1!=otherSFS.size1 || this->size2!=otherSFS.size2){
            cerr <<"cannot add SFS of different sizes"<<endl;
            throw 10;
        }
        SFS* newSFS = new SFS(this->size1,this->size2);
        for (int i=0; i<size1+1;i++){
            for (int j=0; j<size2+1;j++){
                newSFS->setEntry(i,j,this->getEntry(i,j)+otherSFS.getEntry(i,j));
            }
        }        
        return (*newSFS);
        
    }
    void SFS::operator+=(const SFS &otherSFS){
        if ( this->size1!=otherSFS.size1 || this->size2!=otherSFS.size2){
            cerr <<"cannot add SFS of different sizes"<<endl;
            throw 10;
        }
        
        SFS* thisSFS=this;
        for (int i=0; i<size1+1;i++){
            for (int j=0; j<size2+1;j++){
                this->setEntry(i,j,thisSFS->getEntry(i,j)+otherSFS.getEntry(i,j));
            }
        }
        //return (*this);
        
    }
    SFS SFS::operator-(const SFS &otherSFS){
        if ( this->size1!=otherSFS.size1 || this->size2!=otherSFS.size2){
            cerr <<"cannot add SFS of different sizes"<<endl;
            throw 10;
        }        
        SFS* newSFS = new SFS(this->size1,this->size2);
        for (int i=0; i<size1+1;i++){
            for (int j=0; j<size2+1;j++){
                newSFS->setEntry(i,j,this->getEntry(i,j)-otherSFS.getEntry(i,j));
            }
        }
        return (*newSFS);
        
    }
    void SFS::operator-=(const SFS &otherSFS){
        if ( this->size1!=otherSFS.size1 || this->size2!=otherSFS.size2){
            cerr <<"cannot add SFS of different sizes"<<endl;
            throw 10;
        }        
        SFS* thisSFS=this;
        for (int i=0; i<size1+1;i++){
            for (int j=0; j<size2+1;j++){
                this->setEntry(i,j,thisSFS->getEntry(i,j)-otherSFS.getEntry(i,j));
            }
        }
        //return this;        
    }
    
    void SFS::setEntry(int i, int j, double entry){

        if(0<=i && i<=this->size1 && 0<=j  && j<=this->size2){
            this->tTot+= (entry - this->data[i][j]);
            this->data[i][j]=entry;
        }        
        
    }
    
    void SFS::addToEntry(int i, int j, double entry){
        //if(0==i && j==0)
        //    return;
        //if(0==this->size1 && j==this->size2);
        //    return;        
        if(0<=i && i<=this->size1 && 0<=j  && j<=this->size2){
            this->tTot+= entry;
            this->data[i][j]+=entry;
        }        
    }
    void SFS::addToEntry(int i, double entry){
        this->addToEntry(i,0,entry);
    }
    
    double SFS::getEntry(int i, int j)const {
        if(0<=i && i<=this->size1 && 0<=j  && j<=this->size2){
            return this->data[i][j];
        }
        return 0;
    }
    double SFS::getEntry(int i)const {
        return this->getEntry(i,0);
    }
    
    
    string SFS::toString(){
        stringstream ss;
        ss<<"sfs: tTot:"<< tTot<<endl;
        ss.precision(6);

        ss.setf(ios::fixed,ios::floatfield);
        for(int i=0;i<this->size1+1;i++){        
            for(int j=0;j<this->size2+1;j++){       
                if(this->normalized){
                    ss <<this->data[i][j]/this->tTot <<"\t"; 
                }else{
                    ss <<this->data[i][j] <<"\t";     
                }
            }
            ss << endl;
        }
        ss.unsetf(ios::floatfield);
        
        return ss.str();

    }