/* 
 * File:   BootstrapResampler.h
 * Author: peterb
 *
 * Created on November 7, 2012, 4:09 PM
 */

#ifndef BOOTSTRAPRESAMPLER_H
#define	BOOTSTRAPRESAMPLER_H

#include "SNPTable.h"
#include "Parameters.h"
#include "utils.h"
#include <iostream>
#include "boost/thread.hpp"
#include "boost/bind.hpp"

class BootstrapResampler {
public:
    BootstrapResampler();
    BootstrapResampler(const BootstrapResampler& orig);
    virtual ~BootstrapResampler();
    static void setupResample(int nResamples, SNPTable* snpt);
    static boost::mutex ofMutex,brMutex;

    static void doResample(SNPTable* snpt,ofstream* f,int id=0);     
    static ofstream* writeHeader(Parameters* params);
    static void writeFooter(ofstream* f);
private:
    static int nResamples,maxResamples;

};

#endif	/* BOOTSTRAPRESAMPLER_H */

