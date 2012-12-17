/* 
 * File:   StartPos.cpp
 * Author: peterb
 * 
 * Created on December 12, 2012, 12:21 PM
 */

#include "StartPos.h"

StartPos::StartPos() {
    this->startX = 0;
    this->startY = 0;
    this->tStart = 0;
    this->tLag = 0;
    this->expansionK = 0;
}

StartPos::StartPos(int x, int y, double t0, double tLag, int k) {
    this->startX = x;
    this->startY = y;
    this->tStart = t0;
    this->tLag = tLag;
    this->expansionK = k;
}

StartPos::StartPos(const StartPos& orig) {
    this->startX = orig.startX;
    this->startY = orig.startY;
    this->tStart = orig.tStart;
    this->tLag = orig.tLag;
    this->expansionK = orig.expansionK;
}

StartPos::~StartPos() {
}

