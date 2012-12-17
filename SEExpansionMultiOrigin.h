/* 
 * File:   SEExpansionMultiOrigin.h
 * Author: peterb
 *
 * Created on December 12, 2012, 12:17 PM
 */

#ifndef SEEXPANSIONMULTIORIGIN_H
#define	SEEXPANSIONMULTIORIGIN_H

#include "SEExpansionBarrier.h"

using namespace std;
class SEExpansionMultiOrigin : public SEExpansionBarrier {
public:
    SEExpansionMultiOrigin();
    SEExpansionMultiOrigin(const SEExpansionMultiOrigin& orig);
    virtual ~SEExpansionMultiOrigin();
    
    virtual vector<ExpansionEvent>* getExpansionEvents(); //key is time when event happens,

    virtual void setupArrivalTimes();
    vector<StartPos*> startPositions;
private:
   


};

#endif	/* SEEXPANSIONMULTIORIGIN_H */

