/* 
 * File:   Lineage.cpp
 * Author: peterb
 * 
 * Created on June 1, 2012, 10:52 AM
 */

#include "Lineage.h"
#include <stdio.h>

Lineage::Lineage(){
    this->id            =       -1;
    this->length        =       0;
    this->startTime     =       -1;
    this->endTime       =       -1;
    this->length        =       0;
    this->left          =       NULL;
    this->right         =       NULL;
    this->parent        =       NULL;
    this->isTerminal    =       false;
    
    //this->childIds      =       vector<int>(0);
    this->startPop      =       0;
    this->startX        =       0;
    this->startY        =       0;
    this->pops          =       0;
    //Lineage::nl++;
    //cout<<"new l"<<Lineage::nl<<endl;
}


Lineage::~Lineage() {
    //if a lineage is deleted, it takes all its children with it!
    if (this->left!=NULL){
        delete this->left;
    }
    this->left=NULL;
    if (this->right!=NULL){
        delete this->right;
    }
    this->right=NULL;
    delete[] this->pops;
    //cout<<"del l"<<Lineage::nl<<endl;
}

string Lineage::toString(){
    string s1="",s2="";
    stringstream ss (stringstream::out);
    if (this->isTerminal){
        cout <<"shouldnt be here"<<endl;
        ss<< this->id<<"["<<this->startX<<","<<this->startY<<"]"<<":" <<this->length;
    }else if(this->length==0){
        ss<<"("<< this->left->toString()<<";" << this->right->toString()  <<")";
    }else{
        s1=this->left->toString();
        s2=this->right->toString();
        ss<<"("<<s1<<";"<<s2<<"):"<<this->length;
    }
    return ss.str();
}



    
    bool* Lineage::getSNPString(const int nTotLineages){/*
        vector<int>::const_iterator it;
        bool* snpString = new bool[nTotLineages];
        int nChildren=this->childIds.size();
        if (nChildren==0){
            cout <<"ERROR: childIds is empty for lineage "<<this->toString();
            return snpString;
        }
        //cout <<"---------------"<<endl;
        //cout <<"snpString for lineage with children: ";
        //for (it=this->childIds.begin();it != this->childIds.end();++it){
        //    cout << (*it)<<" ";
       // }
        //cout <<" is"<<endl;
        
        it=this->childIds.begin();
        for(int j=0;j<nTotLineages;++j){
            if (j==(*it)){//j carries derived site
                snpString[j]=true;
                //cout << 1;
                ++it;
            }else{ //j carries ancestral site
                snpString[j]=false;
                //cout << 0;
            }

            
        }
        //cout <<endl;
        return snpString;*/
    }
