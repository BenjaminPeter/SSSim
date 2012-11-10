/* 
 * File:   LineageTemplate.h
 * Author: peterb
 *
 * Created on June 7, 2012, 2:47 PM
 */

#ifndef LINEAGETEMPLATE_H
#define	LINEAGETEMPLATE_H
#include "Lineage.h"
#include <string>
#include <sstream>


class LineageTemplate:public Lineage {
public:
    LineageTemplate(int idd,int startX,int startY, int startPop) {
    this->id            =       idd;
    this->isTerminal    =       true;
    
    this->startPop      =       startPop;
    this->startX        =       startX;
    this->startY        =       startY;
    //cout <<"new LT" << this->nl << "\t"<< idd<<endl;
}
    virtual void addSubtreeToSFS(SFS* sfs, int pop1, int pop2){
        cerr <<"NYI";
        throw 10;
    }
    virtual ~LineageTemplate();
    string toString(){
        stringstream ss;
        ss <<this->startPop <<"\t"<<startX<<"\t"<<startY<<endl;
        return ss.str();
    }
private:
    LineageTemplate(const LineageTemplate& orig);
};

#endif	/* LINEAGETEMPLATE_H */

