/* 
 * File:   Event.h
 * Author: peterb
 *
 * Created on June 1, 2012, 10:53 AM
 */

#ifndef EVENT_H
#define	EVENT_H
#include "Sample.h"
#include <stdio.h>
#include <sstream>
#include <string>
#include <gsl/gsl_math.h>

class Event {
public:
    Event(int type=0, Sample* sample=NULL, double time=GSL_POSINF);
    
    virtual ~Event();
    int type;
    Sample* sample;
    double time;
    string toString();
private:
    Event(const Event& orig);
};



#endif	/* EVENT_H */

