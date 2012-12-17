import edmonds
import numpy as np

names =["p1Id","p1X","p1Y","p2Id","p2X","p2Y","fst","psi","deltaH"]
types =["i4","i4","i4","i4","i4","i4","f4","f4","f4"]
data = np.loadtxt("test.es",dtype={'names':names,'formats':types})


def findRoot(g):
    candidateRoot=[]
    curMin=0
    for src in g:
        nPos = len(g[src])
        
        if nPos == curMin:
            candidateRoot.append(src)
            print "tiedMin:", candidateRoot, curMin
        elif nPos > curMin:
            candidateRoot=[src]
            curMin = nPos
            print "new min:", candidateRoot, curMin

            
    return candidateRoot      
            



def graphify(data):
    g = {}
    for d in data:
        if d["psi"]>0:
            if not d["p1Id"] in g:
                g[d["p1Id"]]={}
            g[d["p1Id"]][d["p2Id"]] = d["psi"]
        else:
            if not d["p2Id"] in g:
                g[d["p2Id"]]={}
            g[d["p2Id"]][d["p1Id"]] = -d["psi"]
    return g
            
    
    

