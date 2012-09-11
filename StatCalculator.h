/* 
 * File:   StatCalculator.h
 * Author: peterb
 *
 * Created on June 8, 2012, 10:26 AM
 */

#ifndef STATCALCULATOR_H
#define	STATCALCULATOR_H
#include "SFS.h"
#include <stdio.h>

class StatCalculator {
public:
    StatCalculator();
    StatCalculator(const StatCalculator& orig);
    virtual ~StatCalculator();
    static double getDeltaH(SFS* sfs);
    static double getFST(SFS* sfs);
    static double getPsi(SFS* sfs);
private:

};

#endif	/* STATCALCULATOR_H */

