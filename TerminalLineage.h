/* 
 * File:   TerminalLineage.h
 * Author: peterb
 *
 * Created on June 7, 2012, 2:45 PM
 */

#ifndef TERMINALLINEAGE_H
#define	TERMINALLINEAGE_H
#include "Lineage.h"
#include "LineageTemplate.h"

class TerminalLineage:public Lineage {
public:

    TerminalLineage(const Lineage& orig,int nPops){
        this->id            =       orig.getId();
        //cout <<"new terminal"<<Lineage::nl<<"\tid:"<<Lineage::id<<endl;
        
    //cout <<"copied lineage"<<this->id<<endl;
        this->length        =       0;
        this->startTime     =       0;
        this->endTime       =       0;
        this->parent        =       NULL;
        this->left          =       NULL;
        this->right         =       NULL;
        this->isTerminal    =       true;
    
        //this->childIds      =       vector<int>(0);//1,orig.getId());
        //this->childIds.push_back(this->id);
    //this->children      =       vector<Lineage*>(0);
    
        this->startPop      =       orig.getStartPop();
        this->startX        =       orig.getStartX();
        this->startY        =       orig.getStartY();
        this->nChildren     =       1;
        this->nPops         =       nPops;
        this->pops          =       new int[nPops];
        for (int i=0; i<nPops;i++){
            this->pops[i]= (i==this->startPop);
        }
    }
    virtual ~TerminalLineage();
    string toString(){
        stringstream ss;
        ss<< this->id<<"["<<this->startX<<","<<this->startY<<"]"<<":" <<this->length;
        return ss.str();
    }
    virtual void addSubtreeToSFS(SFS* sfs,int pop1,int pop2);
    virtual void addSubtreeToSFS(SFS* sfs,int pop1);
    int getNChildren() const;
private:
    //TerminalLineage();
    //TerminalLineage(const TerminalLineage& orig);
};

#endif	/* TERMINALLINEAGE_H */

