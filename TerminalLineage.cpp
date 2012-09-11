/* 
 * File:   TerminalLineage.cpp
 * Author: peterb
 * 
 * Created on June 7, 2012, 2:45 PM
 */

#include "TerminalLineage.h"

/*TerminalLineage::TerminalLineage() {
}

TerminalLineage::TerminalLineage(const TerminalLineage& orig) {
}*/

TerminalLineage::~TerminalLineage() {
    //Lineage::nl--;
    //cout<<"del terminal"<<Lineage::nl<<"\t"<<this->id<<endl;
    //cout<<"deleted internal lineage"<<endl;
}

void TerminalLineage::addSubtreeToSFS(SFS* sfs, int pop1, int pop2) {
    /*do stuff*/
    int f1 = this->getPops()[pop1];
    int f2 = this->getPops()[pop2];
    //sfs[f1][f2]+=cur->getLength();
    //tTot+=cur->getLength();
    if ((!(f1 == 0 && f2 == 0)) && (!(f1 == sfs->getSize1() && f2 == sfs->getSize2())))
        sfs->addToEntry(f1, f2, this->getLength());
    //cout<<"TLSFS: added"<< this->getLength() << " to entry ["<<f1<<","<<f2;
    //cout<<"], now "<<sfs->getEntry(f1,f2)<<endl;

}

void TerminalLineage::addSubtreeToSFS(SFS* sfs, int pop1) {
    /*do stuff*/
    int f1 = this->getPops()[pop1];
    //sfs[f1][f2]+=cur->getLength();
    //tTot+=cur->getLength();
    if ((!(f1 == 0)) && (!(f1 == sfs->getSize1())))
        sfs->addToEntry(f1, this->getLength());
    //cout<<"TLSFS: added"<< this->getLength() << " to entry ["<<f1<<","<<f2;
    //cout<<"], now "<<sfs->getEntry(f1,f2)<<endl;

}



int TerminalLineage::getNChildren() const {
    return 1;
}