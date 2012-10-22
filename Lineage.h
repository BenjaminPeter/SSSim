/* 
 * File:   Lineage.h
 * Author: peterb
 *
 * Created on June 1, 2012, 10:52 AM
 */

#ifndef LINEAGE_H
#define	LINEAGE_H

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include "SFS.h"
#include "utils.h"
#include "FreqTable.h"
//
using namespace std;
class Lineage {
public:
    //static int nl;
    Lineage();
    //Lineage(double startTime, Lineage* left, Lineage*right);
    //Lineage(int id, bool isTerminal,int startX=0,int startY=0, int popId=0);
    //Lineage(const Lineage& orig);
    
    
    virtual ~Lineage();
    Lineage *left;
    Lineage *right;
    Lineage *parent;
    void setEndTime(double et){
        this->endTime   =       et;
        this->length    =       this->endTime-this->startTime;
    }
    void setParent(Lineage* parent){
        this->parent    =       parent;
    }
    virtual string toString();

    double getLength() const {
        return length;
    }
    //void getChildIds();
    //vector<Lineage*> getChildren();
    
    void addToFreqTable(FreqTable* ft, bool checkShared){
        ft->addLine(this->length,this->pops,checkShared);
        //cerr << this->length << "\t" << this->pops[0]<< "\t"<<this->pops[1]<<endl;
        if (this->left != NULL){
            this->left->addToFreqTable(ft,checkShared);
        }
        if (this->right != NULL){
            this->right->addToFreqTable(ft,checkShared);
        }
    }
    
        
    
    virtual bool* getSNPString(const int nTotLineages);
    virtual void addSubtreeToSFS(SFS* sfs,int pop1, int pop2){
        cerr<<"shoulndt be here: Lineage::addSubtreeToSFS "<<endl;
    }    
    virtual void addSubtreeToSFS(SFS* sfs,int pop1){
        cerr<<"shoulndt be here: Lineage::addSubtreeToSFS "<<endl;
    }    
    virtual void addSubtreeSNP(utils* ut, bool** dataSet,string* sPos, int nSNP, 
                                const int nChildren, 
                                const double theta){/*
        stringstream ss;
        bool* snpString = this->getSNPString(nChildren);
        double length = this->getLength();
        int nLocalSNP    =   ut->rpois(length*theta);
        if (nLocalSNP>0){
            for(int i=nSNP;i<nSNP+nLocalSNP;i++){
                for(int j=0;j<nChildren;j++){
                    dataSet[j][i]=snpString[j];
                }
            }
            nSNP+=nLocalSNP;
        }
        if (this->isIsTerminal()){
            ss << this->getStartPop() <<":["<<this->getStartX()<<","<<this->getStartX();
            ss << "]\t";
            sPos[this->getId()] = ss.str();
            ss.clear();
            ss.str("");
        }
        if (this->left!=NULL){
            this->left->addSubtreeSNP(ut, dataSet, sPos, nSNP, nChildren, theta);
        }
        if (this->right!=NULL){
            this->right->addSubtreeSNP(ut, dataSet, sPos, nSNP, nChildren, theta);
        }*/
    }

    bool isIsTerminal() const {
        return isTerminal;
    }

    int getId() const {
        return id;
    }

    int getStartPop() const {
        return startPop;
    }

    int getStartX() const {
        return startX;
    }

    int getStartY() const {
        return startY;
    }

    virtual int getNChildren() const {
        return nChildren;
    }

    //int* getChildIds(){
    //    return &childIds[0];
    //}

    int getNPops() const {
        return nPops;
    }

    int* getPops() const {
        return pops;
    }
    
    
    //vector<Lineage*> children;
protected:       
    int* pops;
    int id,nMutation,nChildren,nPops;
    double length;
    double startTime;
    double endTime;
    bool isTerminal;
    int startX,startY,startPop;
    vector<int> childIds;
};

#endif	/* LINEAGE_H */

