/* 
 * File:   utils.h
 * Author: peterb
 *
 * Created on June 1, 2012, 11:33 AM
 */

#ifndef UTILS_H
#define	UTILS_H
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include "time.h"
#include <unistd.h>
#include <gsl/gsl_math.h>
#include <boost/math/special_functions/factorials.hpp>
#include <boost/math/special_functions/gamma.hpp>


class utils {
public:
    utils(long seed=0);
    ~utils();
    void setupRng(long seed=0);
    int* random2(const int n);
    int random1(const int n);
    double randomD(const double d);
    double rexp(const double rate);
    int rpois(const double lambda);
    double nhpp(double rMax, double(*rejFunction)(const double),bool isConst=false);
    double nhpp2(void* obj, double rMax, double (*rejFunction)(void* obj, const double), double tAdd=0,bool isConst=false);    
    gsl_rng* rng;
    void setupStirlingNumberTable(int maxK);
    long int getStirlingNumber(int i,int j);
    void setupStirlingNumberTableD(int maxK);
    long double getStirlingNumberD(int i,int j);
    long int fallingFactorial(int n, int j);   
    long double logFallingFactorial(int n, int j);  
private:
    long int ** stirlingNumberMatrix;
    long double ** stirlingNumberMatrixD;
    int stirlingNumberMax;
    bool stirlingNumbersReady;

};

#endif	/* UTILS_H */

