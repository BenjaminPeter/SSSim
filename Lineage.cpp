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
/*Lineage::Lineage(double startTime, Lineage* left, Lineage* right){
    this->id            =       -1;
    this->length        =       0;
    this->startTime     =       startTime;
    this->endTime       =       startTime;
    this->length        =       0;
    this->left          =       left;
    this->right         =       right;
    this->parent        =       NULL;
    this->isTerminal    =       false;
    
    this->childIds      =       vector<int>(0);
    this->childIds.insert(this->childIds.end(),this->left->childIds.begin(),this->left->childIds.end());
    this->childIds.insert(this->childIds.end(),this->right->childIds.begin(),this->right->childIds.end());
    //cout<<"created new lineage with" << this->childIds.size() << "children"<<endl;
    inplace_merge(this->childIds.begin(),this->childIds.begin()+this->left->childIds.size(),this->childIds.end());
    //cout<<"created new lineage with" << this->childIds.size() << "children"<<this->left->childIds.size()<<"/"<<this->right->childIds.size()<<endl;
    //this->children      =       vector<Lineage*>(0);
    //this->children.insert(this->children.end(),this->left->children.begin(),this->left->children.end());
    //this->children.insert(this->children.end(),this->right->children.begin(),this->right->children.end());
    //inplace_merge(this->children.begin(),this->children.begin()+this->left->children.size(),this->children.end());
        
    
    this->startPop      =       0;
    this->startX        =       0;
    this->startY        =       0;
}
Lineage::Lineage(const Lineage& orig) {    
    this->id            =       orig.id;
    //cout <<"copied lineage"<<this->id<<endl;
    this->length        =       orig.length;
    this->startTime     =       orig.startTime;
    this->endTime       =       orig.endTime;
    this->length        =       orig.length;
    this->parent        =       NULL;
    this->isTerminal    =       orig.isTerminal;
    if (orig.left!=NULL){
        this->left          =       new Lineage(*orig.left);
        this->left->parent  =       this;
    }else{
        this->left      =0;
    }
    if (orig.right!=NULL){
        this->right          =      new Lineage(*orig.right);
        this->right->parent  =      this;
    }else{
        this->right      =0;
    }
    
    this->childIds      =       vector<int>(1,orig.id);
    //this->children      =       vector<Lineage*>(0);
    
    this->startPop      =       orig.startPop;
    this->startX        =       orig.startX;
    this->startY        =       orig.startY;
}**/

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
/*void Lineage::getChildIds(){
        if (this->isTerminal){
            this->childIds.push_back(this->id);
        }else{
            this->left->getChildIds();
            this->right->getChildIds();
            //merge(this->left->childIds.begin(),this->left->childIds.end(),
            //      this->right->childIds.begin(),this->right->childIds.end(),
            //      this->childIds.begin());
            this->childIds.insert(this->childIds.end(),this->left->childIds.begin(),this->left->childIds.end());
            this->childIds.insert(this->childIds.end(),this->right->childIds.begin(),this->right->childIds.end());
            inplace_merge(this->childIds.begin(),this->childIds.begin()+this->left->childIds.size(),this->childIds.end());
        }
        //cout <<"found node with " <<this->childIds.size()<<"children: ";
        //for (int i=0; i<this->childIds.size(); ++i){
        //    cout <<"\t"<< this->childIds[i];
        //}
        //cout <<endl;
    }**/

//vector<Lineage*> Lineage::getChildren(){
//    if (!this->isTerminal){
//        this->left->getChildren();
//        this->right->getChildren();
//    }
//}
    
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