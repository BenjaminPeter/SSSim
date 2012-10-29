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
#include <gsl/gsl_permutation.h>
#include "time.h"
#include <unistd.h>
#include <vector>
#include <gsl/gsl_math.h>
#include <boost/math/special_functions/factorials.hpp>
#include <boost/math/special_functions/gamma.hpp>
#include <string>


class utils {
public:
    utils(long seed=0);
    ~utils();
    void setupRng(long seed=0);
    static int* random2(const int n);
    static int random1(const int n);
    static double randomD(const double d);
    static double rexp(const double rate);
    static int rpois(const double lambda);
    static double nhpp(double rMax, double(*rejFunction)(const double),bool isConst=false);
    static double nhpp2(void* obj, double rMax, double (*rejFunction)(void* obj, const double), double tAdd=0,bool isConst=false);    
    static gsl_rng* rng;
    void setupStirlingNumberTable(int maxK);
    long int getStirlingNumber(int i,int j);
    void setupStirlingNumberTableD(int maxK);
    long double getStirlingNumberD(int i,int j);
    long int fallingFactorial(int n, int j);   
    long double logFallingFactorial(int n, int j);  
    static unsigned int* rmultinom(int nTrials, int size, double* probabilities);
    static void rshuffle(std::vector<std::vector<int> >* toShuffle);
    static void printProgressBar(int percent,std::string label="");
private:
    long int ** stirlingNumberMatrix;
    long double ** stirlingNumberMatrixD;
    int stirlingNumberMax;
    bool stirlingNumbersReady;

};

#endif	/* UTILS_H */

