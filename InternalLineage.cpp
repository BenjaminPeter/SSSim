/* 
 * File:   InternalLineage.cpp
 * Author: peterb
 * 
 * Created on June 7, 2012, 2:46 PM
 */

#include "InternalLineage.h"



//InternalLineage::InternalLineage(const InternalLineage& orig) {
//}

InternalLineage::~InternalLineage() {
    //Lineage::nl--;
    //cout<<"del internal"<<Lineage::nl<<"\t"<<this->toString()<<endl;
    //delete[] this->pops;

    if (this->left!=NULL){
        delete this->left;
    }
    this->left=NULL;
    if (this->right!=NULL){
        delete this->right;
    }
    this->right=NULL;
    
}

