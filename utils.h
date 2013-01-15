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
    static void setupRng(long seed=0);
    static int* random2(const int n);
    static int random1(const int n);
    static double randomD(const double d);
    static double rexp(const double rate);
    static int rpois(const double lambda);
    static double rgamma(const double alpha, const double beta);
    static double nhpp(double rMax, double(*rejFunction)(const double),bool isConst=false);
    static double nhpp2(void* obj, double rMax, double (*rejFunction)(void* obj, const double), double tAdd=0,bool isConst=false);    
    static gsl_rng* rng;
    static void setupStirlingNumberTable(int maxK);
    static long int getStirlingNumber(int i,int j);
    static void setupStirlingNumberTableD(int maxK);
    static long double getStirlingNumberD(int i,int j);
    static long int fallingFactorial(int n, int j);   
    static long double logFallingFactorial(int n, int j);  
    static unsigned int* rmultinom(int nTrials, int size, double* probabilities);
    static void rshuffle(std::vector<std::vector<int>* >* toShuffle);
    static void printProgressBar(int percent,std::string label="");
private:
    static long int ** stirlingNumberMatrix;
    static long double ** stirlingNumberMatrixD;
    static int stirlingNumberMax;
    static bool stirlingNumbersReady;

};

#endif	/* UTILS_H */

