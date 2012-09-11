/* 
 * File:   SequenceSimulator.cpp
 * Author: peterb
 * 
 * Created on June 5, 2012, 11:29 PM
 */

#include "SequenceSimulator.h"
#include "Lineage.h"
#include "utils.h"
#include <iostream>
#include <stdio.h>
#define MAXSNP 100000

class SeqLineage{
public:
    vector<int>childIds;
    int nMutations;
};

SequenceSimulator::SequenceSimulator() {
    this->theta=.020;
}

SequenceSimulator::SequenceSimulator(const SequenceSimulator& orig) {
}

SequenceSimulator::~SequenceSimulator() {
}


SFS* SequenceSimulator::create2DSFS(Lineage* l,int pop1, int pop2){
    Lineage * cur;
    int f1,f2;
    //double tTot=0.0;
    if (pop1>=l->getNPops() || pop2>=l->getNPops()){
        cerr <<"tried to do sfs for pops not available:";
        cerr << l->getNPops() <<"/"<<pop1<<"/"<<pop2<<endl;
        throw 10;                
    }
    int max1,max2;
    max1=l->getPops()[pop1];
    max2=l->getPops()[pop2];        
    SFS* sfs = new SFS(max1,max2);
    
    l->addSubtreeToSFS(sfs,pop1,pop2);                
    
    return sfs;        
}

SFS* SequenceSimulator::create1DSFS(Lineage* l,int pop){
    //double tTot=0.0;
    if (pop>=l->getNPops()){
        cerr <<"tried to do sfs for pops not available:";
        cerr << l->getNPops() <<"/"<<pop<<endl;
        throw 10;                
    }
    int max1;
    max1=l->getPops()[pop];
    SFS* sfs = new SFS(max1);
    
    l->addSubtreeToSFS(sfs,pop);                
    
    return sfs;        
}
bool** SequenceSimulator::createSequenceMatrix(Lineage* l){
    //l->getChildIds();
    Lineage * cur;
    int nChildren = l->getNChildren();
    int nSNP=0,nLocalSNP;
    vector<int>::const_iterator it;
    
    
    //cout << "found " << l->childIds.size() << "children";
    
    
    string* sPos = new string[nChildren];
    bool ** dataSet= new bool*[nChildren];
    for(int i=0;i<nChildren;i++){
        dataSet[i]=new bool[MAXSNP];
    }
    l->addSubtreeSNP(this->ut,dataSet,sPos,nSNP,nChildren,theta);
    //l->getSNPString(l->childIds.size());
    
    /*cur=l;
    while (cur->left !=NULL){
        cur=cur->left;
    }
    while (cur != NULL){
        bool* snpString = cur->getSNPString(nChildren);
        double length = cur->getLength();
        nLocalSNP    =   this->ut->rpois(length*this->theta);
        if (nLocalSNP>0){
            for(int i=nSNP;i<nSNP+nLocalSNP;i++){
                for(int j=0;j<nChildren;j++){
                    dataSet[j][i]=snpString[j];
                }
            }
            nSNP+=nLocalSNP;
        }
        if (cur->isIsTerminal()){
            ss << cur->getStartPop() <<":["<<cur->getStartX()<<","<<cur->getStartX();
            ss << "]\t";
            sPos[cur->getId()] = ss.str();
            ss.clear();
            ss.str("");
        }
        
        if (cur->right !=NULL){
            cur=cur->right;

            while (cur->left !=NULL){
                cur=cur->left;
            }
        }else{
            while (cur->parent!=NULL && cur->parent->right==cur)
                cur=cur->parent;
            cur=cur->parent;
        }
    }*/
    
    
    
    cout <<"segsites: "<<nSNP<<endl;
    for(int j=0;j<nChildren;j++){
    cout << sPos[j];
        for(int i=0;i<nSNP;i++){            
            if (dataSet[j][i]) cout <<"1"; else cout <<"0";
        }
        cout <<endl;
    }

    delete[] sPos;
    


}