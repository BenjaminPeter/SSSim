/* 
 * File:   Sample.cpp
 * Author: peterb
 * 
 * Created on June 1, 2012, 10:53 AM
 */

#include "Sample.h"
using namespace std;

Sample::Sample(int x, int y, vector<Lineage *> lineages) {
    this->x=x;
    this->y=y;
    this->lineages=lineages;
    this->nLineages=lineages.size();
    //Sample::ns++;
    //cout<<"created sample a"<<this->ns<<endl;        
}
Sample::Sample(int* pos, vector<Lineage *> lineages) {
    this->x=pos[0];
    this->y=pos[1];
    this->lineages=lineages;
    this->nLineages=lineages.size();
    //this->ns++;        
    //cout<<"created sample b"<<this->ns<<endl;
}
Sample::Sample(const Sample& orig,int nPops) {
    Lineage* nl;
    this->x=orig.x;
    this->y=orig.y;
    this->lineages=vector<Lineage*>(0);
    this->nLineages=orig.nLineages;
    //this->lineages.resize(this->nLineages);
    for (unsigned int i=0; i<orig.lineages.size();++i){
        nl = new TerminalLineage(*orig.lineages[i],nPops);
        this->lineages.push_back(nl);
    }
    //this->ns++;
    //cout<<"created sample cp"<<this->ns<<endl;
}

Sample::~Sample() {
    //this->ns--;
    //cout<<"deleted sample lineage"<<this->ns<<endl;
    this->deleteAllLineages();
    
}
int Sample::getX() {
    return this->x;
}
int Sample::getY() {
    return this->y;
}

//returns number of children of coal event
int Sample::merge2lineages(int l1,int l2, double curTime){
    //cout <<"merge"<<endl;
    Lineage* tmpl1 = this->lineages[l1];
    this->lineages[l1] = this->lineages[this->nLineages-1];
    Lineage* tmpl2 = this->lineages[l2];
    this->lineages[l2] = this->lineages[this->nLineages-2];
    

    Lineage* newLineage=new InternalLineage(curTime,tmpl1,tmpl2);
    tmpl1->setParent(newLineage);
    tmpl2->setParent(newLineage);
    tmpl1->setEndTime(curTime);
    tmpl2->setEndTime(curTime);
    this->lineages[this->nLineages-2]=newLineage;
    this->lineages.resize(this->nLineages-1,NULL);
    this->nLineages--;  
    if (this->lineages.size()!=this->nLineages){
        cout<<"ERROR in Sample merging\n";
        throw 0;
    }
    return newLineage->getNChildren();
}
void Sample::deleteAllLineages(){
    for(vector<Lineage*>::iterator it=this->lineages.begin();it!=this->lineages.end();it++){
        delete (*it);
    }    
    this->lineages.clear();    
}
int Sample::merge2randomLineages(double curTime){
    int*lids=utils::random2(this->nLineages);
    int nChildren =this->merge2lineages(lids[0],lids[1],curTime);
    delete[] lids;
    return nChildren;
}

Lineage* Sample::getRandomLineage(){
    int lid1 = utils::random1(this->nLineages);
    Lineage * l1 = this->lineages[lid1];
    return l1;
}
Lineage* Sample::getRandomLineageForMigration(){
    int lid1 = utils::random1(this->nLineages);
    Lineage * l1 = this->lineages[lid1];
    this->lineages[lid1] = this->lineages[this->nLineages-1];
    this->lineages.resize(this->nLineages-1,NULL);
    this->nLineages--;  
    if (this->lineages.size()!=this->nLineages){
        cout<<"ERROR in getRdmLineage\n";
    }
    return l1;
}
void Sample::addLineage(Lineage* lineage){
    this->lineages.resize(this->nLineages+1,lineage);
    this->nLineages++;  
    if (this->lineages.size()!=this->nLineages){
        cout<<"ERROR in getRdmLineage\n";
    }
}
string Sample::toStringShort(){
    stringstream ss;
    try{
        ss <<"Sample at ("<<this->getX()<<",";
        ss << this->getY() << ") with ";
        ss << this->getNlineages()   <<  " lineages.\n";
        return ss.str();
    }
    catch(...){
        fprintf(stderr,"sth wrong in Sample::toStringShort");
    }
}
string Sample::toString(){
    stringstream ss;
    ss <<"Sample at ("<<this->getX()<<",";
    ss << this->getY() << ") with ";
    ss << this->getNlineages()   <<  " lineages:\n";
    for (int l=0<l;l<this->getNlineages();l++){
        ss<<this->lineages[l]->toString()<<"|";
    }
    ss<<"\n";
    return ss.str();
}
