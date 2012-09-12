/* 
 * File:   Coords.h
 * Author: peterb
 *
 * Created on June 1, 2012, 2:22 PM
 */

#ifndef COORDS_H
#define	COORDS_H

class Coords{
public:
    Coords(int x, int y){
        this->x=x;
        this->y=y;
    }
    int x,y;
    bool operator==(const Coords &other) const {
        return this->x==other.x && this->y==other.y;
    }
    bool operator!=(const Coords &other) const {
        return !(*this == other);
    }
    bool operator<(const Coords& other) const{
        if (this->x==other.x){
            return this->y<other.y;
        }
        return this->x<other.x;
    }
    bool operator>(const Coords& other) const{
        if (this->x==other.x){
            return this->y>other.y;
        }
        return this->x>other.x;
    }
};

#endif	/* COORDS_H */

