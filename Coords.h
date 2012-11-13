/* 
 * File:   Coords.h
 * Author: peterb
 *
 * Created on June 1, 2012, 2:22 PM
 */

#ifndef COORDS_H
#define	COORDS_H
#include <iostream>
#include <math.h>

typedef std::pair<int, int> Coords;
inline std::ostream& operator<<(std::ostream& os, const Coords& c) {
    return os << "(" << c.first << "," << c.second << ")";
}
inline int operator-(const Coords& c1, const Coords& c2) {
    return sqrt((c1.first-c2.first)*(c1.first-c2.first) + 
            (c1.second-c2.second)*(c1.second-c2.second));
   
}


#endif	/* COORDS_H */


