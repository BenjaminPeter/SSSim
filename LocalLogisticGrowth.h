/* 
 * File:   LocalLogisticGrowth.h
 * Author: peterb
 *
 * Created on June 4, 2012, 10:40 AM
 */

#ifndef LOCALLOGISTICGROWTH_H
#define	LOCALLOGISTICGROWTH_H
#include <vector>
using namespace std;

class LocalLogisticGrowth {
public:
    LocalLogisticGrowth(double growthRate, double k, int* dims);
    LocalLogisticGrowth(const LocalLogisticGrowth& orig);
    virtual ~LocalLogisticGrowth();
    
    int*** getMigrationMatrix(double time);
    int** getCarCapMatrix(double time);
    vector<pair<double,int> >* getExpansionEvents();    
private:

};

#endif	/* LOCALLOGISTICGROWTH_H */

