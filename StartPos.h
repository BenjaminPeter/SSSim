/* 
 * File:   StartPos.h
 * Author: peterb
 *
 * Created on December 12, 2012, 12:21 PM
 */

#ifndef STARTPOS_H
#define	STARTPOS_H

class StartPos {
public:
    StartPos();
    StartPos(int x, int y, double t0, double tLag, int k);
    StartPos(const StartPos& orig);
    virtual ~StartPos();
            
    double tStart, tLag, expansionK;
    int startX, startY;
private:

};

#endif	/* STARTPOS_H */

