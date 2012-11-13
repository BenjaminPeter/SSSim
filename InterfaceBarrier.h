/* 
 * File:   MigrationSchemeBarrier.h
 * Author: peterb
 *
 * Created on November 12, 2012, 10:48 AM
 */

#ifndef INTERFACEBARRIER_H
#define	INTERFACEBARRIER_H

#include <vector>
#include "MigrationScheme.h"
#include "Barrier.h"

class InterfaceBarrier {
public:
    virtual void newBarrier(Barrier* b){
        this->barriers->push_back(b);
    }
    virtual void addBarriersToMigrationScheme()=0;
private:
    vector<Barrier*> barriers;
};


#endif	 INTERFACEBARRIER_H

