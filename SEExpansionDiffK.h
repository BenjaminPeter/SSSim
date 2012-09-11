/* 
 * File:   SEEExpansionDiffK.h
 * Author: peterb
 *
 * Created on August 17, 2012, 11:30 AM
 */
#include "SEExpansion.h"
#include <math.h>

#ifndef SEEEXPANSIONDIFFK_H
#define	SEEEXPANSIONDIFFK_H

class SEExpansionDiffK : public SEExpansion{
public:
    SEExpansionDiffK();
    SEExpansionDiffK(const SEExpansionDiffK& orig);
    virtual ~SEExpansionDiffK();
    virtual int getExpansionK(int i, int j) const;
    void setCentralArea(int c){
        this->centralArea=c;
    }
    void setCentralK(int c){
        this->kCentral=c;
    }
    void setExpansionK(int k){
        this->kExternal=k;
    }
    
    int getExpansionK() const;
private:
    int centralArea,kCentral,kExternal;
    
};

#endif	/* SEEEXPANSIONDIFFK_H */

