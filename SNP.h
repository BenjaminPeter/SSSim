/* 
 * File:   SNP.h
 * Author: peterb
 *
 * Created on November 12, 2012, 11:39 AM
 */

#ifndef SNP_H
#define	SNP_H

#include <vector>

typedef std::vector<int> SNP;

/*class SNP {
public:
    SNP(int nSamples=0);
    SNP(std::vector<int>* freqs, std::vector<int>* sampleSizes);
    SNP(const SNP& orig);
    virtual ~SNP();
    
    //getter & setter
    int GetNSamples() const;
    std::vector<int>* GetSampleSizes() const;
    void SetFreqs(std::vector<int>* freqs);
    std::vector<int>* GetFreqs() const;
    
    static std::size_t hash_value(SNP const& s){
        std::size_t seed = 0;
        boost::hash_combine(seed,s.freqs);
    }
    
    bool operator==(SNP const& s1){
        return this->freqs == s1.freqs;
    }
private:
    std::vector<int>* freqs;
    std::vector<int>* sampleSizes;
    int nSamples;


};*/

#endif	/* SNP_H */

