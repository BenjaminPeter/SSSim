/* 
 * File:   SFS.h
 * Author: peterb
 *
 * Created on June 7, 2012, 5:22 PM
 */

#ifndef SFS_H
#define	SFS_H
#include <string>
#include <iostream>
using namespace std;
class SFS {
public:
    SFS(){size1=0;data=0;size2=0;tTot=0;normalized=false;};
    SFS(const SFS& orig);   
    SFS(int s1,int s2);
    SFS(int s1);
    
    virtual ~SFS();
    string toString();

    double** getData() const {
        return data;
    }

    void setEntry(int i, int j, double entry);
    double getEntry (int i, int j) const;
    double getEntry (int i) const;
    void addToEntry(int i, int j, double entry);
    void addToEntry(int i, double entry);
    SFS operator+(const SFS &otherSFS);
    void operator+=(const SFS &otherSFS);
    SFS operator-(const SFS &otherSFS);
    void operator-=(const SFS &otherSFS);

    bool isNormalized() const {
        return normalized;
    }

    void setNormalized(bool normalized) {
        this->normalized = normalized;
    }

    int getSize1() const {
        return size1;
    }

    int getSize2() const {
        return size2;
    }

    double getTTot() const {
        return tTot;
    }




private:

    double** data;
    int size1,size2;
    double tTot;
    bool normalized;
};

#endif	/* SFS_H */

