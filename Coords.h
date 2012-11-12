/* 
 * File:   Coords.h
 * Author: peterb
 *
 * Created on June 1, 2012, 2:22 PM
 */

#ifndef COORDS_H
#define	COORDS_H
#include <iostream>

typedef std::pair<int, int> Coords;
inline std::ostream& operator<<(std::ostream& os, const Coords& c) {
   return os << "(" << c.first << "," << c.second << ")";
}


#endif	/* COORDS_H */


