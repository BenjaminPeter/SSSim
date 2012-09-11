/* 
 * File:   InternalLineage.h
 * Author: peterb
 *
 * Created on June 7, 2012, 2:46 PM
 */

#ifndef INTERNALLINEAGE_H
#define	INTERNALLINEAGE_H
#include "Lineage.h"
#include "SFS.h"

class InternalLineage : public Lineage {
public:

    InternalLineage(double startTime, Lineage* left, Lineage* right) {
        this->length = 0;
        this->startTime = startTime;
        this->endTime = startTime;
        this->length = 0;
        this->left = left;
        this->right = right;
        this->parent = NULL;
        this->isTerminal = false;
        this->nPops = left->getNPops();
        this->pops = new int[this->nPops];
        int* lPops = this->left->getPops();
        int* rPops = this->right->getPops();
        for (int i = 0; i < nPops; i++) {
            this->pops[i] = lPops[i] + rPops[i];
        }

        //this->childIds      =       vector<int>(0);
        //this->childIds.insert(this->childIds.end(),this->left->getChildIds(),
        //                     this->left->getChildIds()+this->left->getNChildren());
        //this->childIds.insert(this->childIds.end(),this->right->getChildIds(),
        //                      this->right->getChildIds()+this->right->getNChildren());
        //cout<<"created new lineage "<<this->nl<<" with" << this->childIds.size() << "children"<<endl;
        //inplace_merge(this->childIds.begin(),this->childIds.begin()+this->left->getNChildren(),
        //        this->childIds.end());
        //cout<<"created new lineage with" << this->childIds.size() << "children"<<this->left->childIds.size()<<"/"<<this->right->childIds.size()<<endl;
        //this->children      =       vector<Lineage*>(0);
        //this->children.insert(this->children.end(),this->left->children.begin(),this->left->children.end());
        //this->children.insert(this->children.end(),this->right->children.begin(),this->right->children.end());
        //inplace_merge(this->children.begin(),this->children.begin()+this->left->children.size(),this->children.end());
        this->nChildren = this->left->getNChildren() + this->right->getNChildren();
    }


    virtual void addSubtreeToSFS(SFS* sfs, int pop1, int pop2) {
        int f1 = this->getPops()[pop1];
        int f2 = this->getPops()[pop2];
        //sfs[f1][f2]+=cur->getLength();
        //tTot+=cur->getLength();
        if ((!(f1 == 0 && f2 == 0)) && (!(f1 == sfs->getSize1() && f2 == sfs->getSize2()))) sfs->addToEntry(f1, f2, this->getLength());
        //cout<<"SFS: added"<< this->getLength() << " to entry ["<<f1<<","<<f2;
        //cout<<"], now "<<sfs->getEntry(f1,f2)<<endl;
        if (this->left != NULL)
            this->left->addSubtreeToSFS(sfs, pop1, pop2);
        if (this->right != NULL)
            this->right->addSubtreeToSFS(sfs, pop1, pop2);
    }

    virtual void addSubtreeToSFS(SFS* sfs, int pop) {
        int f1 = this->getPops()[pop];
        //sfs[f1][f2]+=cur->getLength();
        //tTot+=cur->getLength();
        if ((!f1 == 0) && (!(f1 == sfs->getSize1())))
            sfs->addToEntry(f1, this->getLength());
        //cout<<"SFS: added"<< this->getLength() << " to entry ["<<f1<<","<<f2;
        //cout<<"], now "<<sfs->getEntry(f1,f2)<<endl;
        if (this->left != NULL)
            this->left->addSubtreeToSFS(sfs, pop);
        if (this->right != NULL)
            this->right->addSubtreeToSFS(sfs, pop);
    }
    virtual ~InternalLineage();
private:
    //InternalLineage(const InternalLineage& orig);
};

#endif	/* INTERNALLINEAGE_H */

