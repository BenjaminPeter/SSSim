#!/usr/bin/python
import sys,os
import numpy as np
#from calcStatResamplingPar import *
 
id=sys.argv[1]

loc = np.loadtxt("%s.loc"%id)
nPops = loc.shape[0]
nComp=nPops*(nPops-1)/2

os.system("Rscript /data/surfing/scripts/mergeStuff.r %s"%id) #merge

resampleFile = np.loadtxt("%s.psi.sbs"%id)
vars = np.apply_along_axis(np.var,0,resampleFile[3:])
np.savetxt("%s.bsvar"%id,vars)

#parse msCommand
pwd=os.getcwd()
path= pwd + "/%s.es"%id
varPath = pwd + "/%s.bsvar"%id
outId = path+".out"

os.system("echo \"tdoa3('%s','%s')\" > cmd.m"%(path,varPath))
os.system("matlab -nojvm -nodisplay -nosplash < cmd.m >/dev/null")
os.system("cat %s"%outId)

