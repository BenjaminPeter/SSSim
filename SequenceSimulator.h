/* 
 * File:   SequenceSimulator.h
 * Author: peterb
 *
 * Created on June 5, 2012, 11:29 PM
 */

#ifndef SEQUENCESIMULATOR_H
#define	SEQUENCESIMULATOR_H

#include "utils.h"
#include "Lineage.h"
#include "SFS.h"

class SequenceSimulator {
public:
    utils * ut;
    SequenceSimulator();
    
    virtual ~SequenceSimulator();

    double getTheta() const {
        return theta;
    }

    void setTheta(double theta) {
        this->theta = theta;
    }
    
    bool** createSequenceMatrix(Lineage* l);
    SFS* create1DSFS(Lineage* l,int pop);
    SFS* create2DSFS(Lineage* l,int pop1,int pop2);
    void createMDSFS(Lineage* l,int nSamples=0);

private:
    SequenceSimulator(const SequenceSimulator& orig);
    double theta;

};

#endif	/* SEQUENCESIMULATOR_H */

