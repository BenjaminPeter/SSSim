/* 
 * File:   StatCalculator.cpp
 * Author: peterb
 * 
 * Created on June 8, 2012, 10:26 AM
 */

#include "StatCalculator.h"

StatCalculator::StatCalculator() {
}

StatCalculator::StatCalculator(const StatCalculator& orig) {
}

StatCalculator::~StatCalculator() {
}

double StatCalculator::getDeltaH(SFS* sfs) {
    double all = 0;
    double h1 = 0, h2 = 0;
    int n1 = sfs->getSize1();
    int n2 = sfs->getSize2();
    for (int i = 0; i < n1 + 1; ++i) {
        for (int j = 0; j < n2 + 1; ++j) {
            double entry = sfs->getEntry(i, j);
            h1 += entry * i * (n1 - i) / float(n1) / n1;
            h2 += entry * j * (n2 - j) / float(n2) / n2;
            if ((i != 0 || j != 0) && (i != sfs->getSize1() || j != sfs->getSize2()))
                all += entry;
        }
    }
    return (h1 - h2) / all;
}

double StatCalculator::getFST(SFS* sfs) {
    double pi1 = 0, pi2 = 0, pib = 0;
    int n1 = sfs->getSize1();
    int n2 = sfs->getSize2();


    for (int i = 0; i < n1 + 1; ++i) {
        for (int j = 0; j < n2 + 1; ++j) {
            double entry = sfs->getEntry(i, j);
            pi1 += entry * i * (n1 - i);
            pi2 += entry * j * (n2 - j);
            pib += entry * (i + j)*((n1 + n2)-(i + j));
            //printf("i: %d; j: %d; n1: %d; n2: %d sfs: %f pi: %f\n",i,j,n1,n2,entry,entry*i*(n1-i));
        }
    }
    pi1 /= (n1 * (n1 - 1) / 2);
    pi2 /= (n2 * (n2 - 1) / 2);
    pib /= ((n1 + n2)*((n1 + n2) - 1) / 2);
    //printf("pi1: %f; pi2: %f; pib: %f\n",pi1,pi2,pib);
    return 1. - (pi1 + pi2) / 2 / pib;

}

double StatCalculator::getPsi(SFS* sfs) {
    double psi = 0, all = 0;
    for (int i = 1; i < sfs->getSize1() + 1; ++i) {
        for (int j = 1; j < sfs->getSize2() + 1; ++j) {
            double entry = sfs->getEntry(i, j);
            psi += (float(i) / sfs->getSize1() - float(j) / sfs->getSize2()) * entry;
            if ((i != 0 || j != 0) && (i != sfs->getSize1() || j != sfs->getSize2()))
                all += entry;
        }
    }
    if (all == 0) {
        return 0;
    }
    return psi / all;
} 