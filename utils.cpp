/* 
 * File:   utils.cpp
 * Author: peterb
 * 
 * Created on June 1, 2012, 11:33 AM
 */

#include "utils.h"
#include "Event.h"
#include "stdio.h"

utils::utils(long seed){
    this->setupRng(seed);
    this->stirlingNumberMatrix=0;
    this->stirlingNumbersReady=false;
    this->stirlingNumberMax=0;
}
utils::~utils(){
    //gsl_rng_free (this->rng);
    if (this->stirlingNumbersReady && this->stirlingNumberMatrix!=NULL){
        for (int i=0; i<this->stirlingNumberMax;i++){
            //delete[] this->stirlingNumberMatrix[i];
        }
        //delete[] this->stirlingNumberMatrix;
    }
}
void utils::setupRng(long seed){
    gsl_rng_env_setup();
    this->rng           = gsl_rng_alloc(gsl_rng_mt19937);
    
    if (seed==0){
        seed           = time(NULL) * getpid();
    }
    printf("rng initialized with seed %ld\n",seed);    
    gsl_rng_set(this->rng,seed);
}
int* utils::random2(const int n){
    int* arr = new int[2];
    arr[0]= gsl_rng_uniform_int (this->rng, n);
    arr[1] =gsl_rng_uniform_int (this->rng, n-1);
    return arr;   
}

int utils::random1(const int n){
    return gsl_rng_uniform_int (this->rng, n);
}

double utils::randomD(const double n){
    return gsl_rng_uniform(this->rng)*n;
}

double utils::rexp(const double rate){
    if (rate==0)
        return GSL_POSINF;
    return gsl_ran_exponential(this->rng,1./rate);

}

int utils::rpois(const double lambda){
    if (lambda==0){
        return 0;
    }
    return gsl_ran_poisson(this->rng,lambda);
}

double utils::nhpp(double rMax, double(*rejFunction)(const double) , bool isConst){
    if (rMax==0)
        return GSL_POSINF;
    if (isConst)
        return gsl_ran_exponential(this->rng,1./rMax);
                
    int nRejections=0;
    int maxRejections=10000;
    double rve,rvu;
    while (nRejections<maxRejections){
        rve     =       gsl_ran_exponential(this->rng,1./rMax);
        rvu     =       gsl_rng_uniform(this->rng);
        if (rejFunction(rve)/rMax < rvu)
            return rve;
        nRejections++;
    }
    fprintf(stderr,"utils::nhpp: could not get next event for PP(rmax=%f)",rMax);
    throw 10;
}

double utils::nhpp2(void* obj, double rMax, double (*rejFunction)(void* obj, const double), double tAdd,bool isConst){
    double tmp;
    if (rMax==0)
        return GSL_POSINF;
    if (rMax==GSL_POSINF)
        return 0;
    if (isConst)
        return gsl_ran_exponential(this->rng,1./rMax);
                
    int nRejections=0;
    int maxRejections=10000;
    double rve,rvu;
    while (nRejections<maxRejections){
        rve     =       gsl_ran_exponential(this->rng,1./rMax);
        rvu     =       gsl_rng_uniform(this->rng);
        tmp=rejFunction(obj,rve+tAdd);
        //no event happens
        if (tmp==0){
            //cout<<"no event"<<endl;
            return GSL_POSINF;
        }
        
        if (rvu <= tmp/rMax)
            return rve;
        //fprintf(stdout,"nhpp2:[%d/%d]:unsuccessful w/ rMax=%f, tmp=%f, uniform =%f\n",nRejections,maxRejections,rMax,tmp,rvu);
        nRejections++;
    }
    fprintf(stderr,"utils::nhpp2: could not get next event for PP(rmax=%f)\n",rMax);
    throw 10;    
}

long int utils::fallingFactorial(int n, int j){
    return boost::math::falling_factorial(n,j);
}

void utils::setupStirlingNumberTable(int maxK){
    this->stirlingNumberMatrix=new long int*[maxK];
    for (int i=0;i<maxK;i++){
        this->stirlingNumberMatrix[i]=new long int[maxK];
        for (int j=0;j<maxK;j++){
            this->stirlingNumberMatrix[i][j]=0;
        }
    }
    this->stirlingNumberMatrix[0][0]=1;
    for (int i=1;i<maxK;i++){
        this->stirlingNumberMatrix[i][0]=1;
        for (int j=1;j<maxK;j++){
            if (i==j){
                this->stirlingNumberMatrix[i][j]=1;
            }else{
                this->stirlingNumberMatrix[i][j]=this->stirlingNumberMatrix[i-1][j-1]+(j+1)*this->stirlingNumberMatrix[i-1][j];
            }
        }
    }
    this->stirlingNumbersReady=true;
    this->stirlingNumberMax=maxK;
}

long int utils::getStirlingNumber(int i, int j){
    if (!this->stirlingNumbersReady){
        cerr <<"stirling numbers not initialized"<<endl;
        throw 10;
    }
    if (i>this->stirlingNumberMax || j>this->stirlingNumberMax || i<1 || j<1)
        return 0;
    return this->stirlingNumberMatrix[i-1][j-1];
}