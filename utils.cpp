/* 
 * File:   utils.cpp
 * Author: peterb
 * 
 * Created on June 1, 2012, 11:33 AM
 */

#include "utils.h"
#include "Event.h"
#include "stdio.h"

gsl_rng* utils::rng=NULL;
    long int** utils::stirlingNumberMatrix=0;
    long double** utils::stirlingNumberMatrixD=0;
    bool utils::stirlingNumbersReady=false;
    int utils::stirlingNumberMax=0;
utils::utils(long seed){
    utils::setupRng(seed);
    utils::stirlingNumberMatrix=0;
    utils::stirlingNumberMatrixD=0;
    utils::stirlingNumbersReady=false;
    utils::stirlingNumberMax=0;
}
utils::~utils(){
    gsl_rng_free (utils::rng);
    /*if (utils::stirlingNumbersReady && utils::stirlingNumberMatrix!=NULL&& utils::stirlingNumberMatrixD!=NULL){
        for (int i=0; i<utils::stirlingNumberMax;i++){
            delete[] utils::stirlingNumberMatrix[i];
            delete[] utils::stirlingNumberMatrixD[i];
        }
        delete[] utils::stirlingNumberMatrix;
        delete[] utils::stirlingNumberMatrixD;**/
    //}
}
void utils::setupRng(long seed){
    gsl_rng_env_setup();
    utils::rng           = gsl_rng_alloc(gsl_rng_mt19937);
    
    if (seed==0){
        seed           = time(NULL) * getpid();
    }
    //printf("rng initialized with seed %ld\n",seed);    
    gsl_rng_set(utils::rng,seed);
}
int* utils::random2(const int n){
    int* arr = new int[2];
    arr[0]= gsl_rng_uniform_int (utils::rng, n);
    arr[1] =gsl_rng_uniform_int (utils::rng, n-1);
    return arr;   
}

int utils::random1(const int n){
    return gsl_rng_uniform_int (utils::rng, n);
}

double utils::randomD(const double n){
    return gsl_rng_uniform(utils::rng)*n;
}

double utils::rexp(const double rate){
    if (rate==0)
        return GSL_POSINF;
    return gsl_ran_exponential(utils::rng,1./rate);

}

int utils::rpois(const double lambda){
    if (lambda==0){
        return 0;
    }
    return gsl_ran_poisson(utils::rng,lambda);
}

unsigned int* utils::rmultinom(int nTrials, int size, double* probabilities){
    unsigned int* res = new unsigned int[size];
    gsl_ran_multinomial(utils::rng,size,nTrials,probabilities,res);
    return res;
}

void utils::rshuffle(std::vector<vector<int>* >* toShuffle){
    int (*fp)(int)=utils::random1;
//    typedef int (utils::*BLA) (int);
//    BLA fp = &utils::random1;
    random_shuffle(toShuffle->begin(),toShuffle->end(),fp);   
}

double utils::nhpp(double rMax, double(*rejFunction)(const double) , bool isConst){
    if (rMax==0)
        return GSL_POSINF;
    if (isConst)
        return gsl_ran_exponential(utils::rng,1./rMax);
                
    int nRejections=0;
    int maxRejections=100000;
    double rve,rvu;
    while (nRejections<maxRejections){
        rve     =       gsl_ran_exponential(utils::rng,1./rMax);
        rvu     =       gsl_rng_uniform(utils::rng);
        if (rejFunction(rve)/rMax < rvu)
            return rve;
        nRejections++;
    }
    fprintf(stderr,"utils::nhpp: could not get next event for PP(rmax=%f)",rMax);
    return -1.0;
    throw 10;
}

double utils::nhpp2(void* obj, double rMax, double (*rejFunction)(void* obj, const double), double tAdd,bool isConst){
    double tmp;
    if (rMax==0)
        return GSL_POSINF;
    if (rMax==GSL_POSINF)
        return 0;
    if (isConst)
        return gsl_ran_exponential(utils::rng,1./rMax);
                
    int nRejections=0;
    int maxRejections=10000;
    double rve,rvu;
    while (nRejections<maxRejections){
        rve     =       gsl_ran_exponential(utils::rng,1./rMax);
        rvu     =       gsl_rng_uniform(utils::rng);
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
    return boost::math::falling_factorial(n,j);//unreliable for large n
}

long double utils::logFallingFactorial(int n, int j){
    return boost::math::lgamma(n+1)-boost::math::lgamma(n-j+1);
}

void utils::setupStirlingNumberTable(int maxK){
    utils::stirlingNumberMatrix=new long int*[maxK];
    for (int i=0;i<maxK;i++){
        utils::stirlingNumberMatrix[i]=new long int[maxK];
        for (int j=0;j<maxK;j++){
            utils::stirlingNumberMatrix[i][j]=0;
        }
    }
    utils::stirlingNumberMatrix[0][0]=1;
    for (int i=1;i<maxK;i++){
        utils::stirlingNumberMatrix[i][0]=1;
        for (int j=1;j<maxK;j++){
            if (i==j){
                utils::stirlingNumberMatrix[i][j]=1;
            }else{
                utils::stirlingNumberMatrix[i][j]=utils::stirlingNumberMatrix[i-1][j-1]+(j+1)*utils::stirlingNumberMatrix[i-1][j];
            }
        }
    }
    //utils::stirlingNumbersReady=true;
    utils::stirlingNumberMax=maxK;
}

long int utils::getStirlingNumber(int i, int j){
    if (!utils::stirlingNumbersReady){
        cerr <<"stirling numbers not initialized"<<endl;
        throw 10;
    }
    if (i>utils::stirlingNumberMax || j>utils::stirlingNumberMax || i<1 || j<1)
        return 0;
    return utils::stirlingNumberMatrix[i-1][j-1];
}

void utils::setupStirlingNumberTableD(int maxK){
    utils::stirlingNumberMatrixD=new long double*[maxK];
    for (int i=0;i<maxK;i++){
        utils::stirlingNumberMatrixD[i]=new long double[maxK];
        for (int j=0;j<maxK;j++){
            utils::stirlingNumberMatrixD[i][j]=0;
        }
    }
    utils::stirlingNumberMatrixD[0][0]=1;
    for (int i=1;i<maxK;i++){
        utils::stirlingNumberMatrixD[i][0]=1;
        for (int j=1;j<maxK;j++){
            if (i==j){
                utils::stirlingNumberMatrixD[i][j]=1;
            }else{
                utils::stirlingNumberMatrixD[i][j]=utils::stirlingNumberMatrixD[i-1][j-1]+(j+1)*utils::stirlingNumberMatrixD[i-1][j];
            }
        }
    }
    utils::stirlingNumbersReady=true;
    utils::stirlingNumberMax=maxK;
}

long double utils::getStirlingNumberD(int i, int j){
    if (!utils::stirlingNumbersReady){
        cerr <<"stirling numbers not initialized"<<endl;
        throw 10;
    }
    if (i>utils::stirlingNumberMax || j>utils::stirlingNumberMax || i<1 || j<1)
        return 0;
    return utils::stirlingNumberMatrixD[i-1][j-1];
}

void utils::printProgressBar(int percent,string label){
  string bar;

  for(int i = 0; i < 50; i++){
    if( i < (percent/2)){
      bar.replace(i,1,"=");
    }else if( i == (percent/2)){
      bar.replace(i,1,">");
    }else{
      bar.replace(i,1," ");
    }
  }

  std::cout<< "\r" "[" << bar << "] ";
  std::cout.width( 3 );
  std::cout<< percent << "% " <<label << std::flush;
}
