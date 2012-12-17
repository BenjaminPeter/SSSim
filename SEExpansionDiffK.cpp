/* 
 * File:   SEEExpansionDiffK.cpp
 * Author: peterb
 * 
 * Created on August 17, 2012, 11:30 AM
 */

#include "SEExpansionDiffK.h"
#include "MigrationScheme.h"

SEExpansionDiffK::SEExpansionDiffK():SEExpansion() {
    this->kCentral=100;
    this->kExternal=100;
}

SEExpansionDiffK::SEExpansionDiffK(const SEExpansionDiffK& orig) {
}

SEExpansionDiffK::~SEExpansionDiffK() {
}

int SEExpansionDiffK::getExpansionK(int x, int y) const{
    
   int distance=sqrt((float(x)-float(this->sp->startX))*(float(x)-float(this->sp->startX))
            +(float(y)-float(this->sp->startY))*(float(y)-float(this->sp->startY)));
   
   
   if(distance>this->centralArea){
       return this->kExternal;
   }
   return this->kCentral;
    
    
}

int SEExpansionDiffK::getExpansionK() const{
    if (this->kExternal>this->kCentral){
        return this->kExternal;
    }
    return this->kCentral;
    
}