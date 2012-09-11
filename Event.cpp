/* 
 * File:   Event.cpp
 * Author: peterb
 * 
 * Created on June 1, 2012, 10:53 AM
 */

#include "Event.h"

Event::Event(int type, Sample* sample, double time) {
    this->type=type;
    this->sample=sample;
    this->time=time;
}

Event::Event(const Event& orig) {
}

Event::~Event() {
//    delete this->type;
    this->sample=NULL;
//    delete this->time;
            
}

string Event::toString(){
    stringstream ss;
    if (this->type==0)
        return "termination event!\n";
    if (this->sample==0){
        cerr <<"invalid sample";
        throw 10;
    }
    ss << this->type;
    string evType = ss.str();
    switch (this->type){
        case 1:
            evType="coal";
            break;
        case 2:
            evType="migN";
            break;            
        case 3:
            evType="migS";
            break;
        case 4:
            evType="migE";
            break;
        case 5:
            evType="migW";
            break;            
    }
    char buffer[200];
    ss.clear();
    sprintf(buffer,"E[%2.6f]:%s\t%s",this->time,evType.c_str(),this->sample->toStringShort().c_str());
    ss <<buffer;
    return ss.str();
}

