./ms2 6 10000 -t 4 -I 9 3 0 0 0 0 0 0 0 3 0 -m 1 2 5 -m 1 4 5 -m 2 1 5 -m 2 3 5 -m 2 5 5 -m 3 2 5 -m 3 6 5 -m 4 1 5 -m 4 5 5 -m 4 7 5 -m 5 2 5 -m 5 4 5 -m 5 6 5 -m 5 8 5 -m 6 3 5 -m 6 5 5 -m 6 9 5 -m 7 4 5 -m 7 8 5 -m 8 5 5 -m 8 7 5 -m 8 9 5 -m 9 6 5 -m 9 8 5 | grep segs |cut -c 10- > ss.ms.txt 
#corresponds to an ibd model with width and height=3, 3 samples taken from each
#from (0,0) and (2,2) and M=5 in all directions:q

