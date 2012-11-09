#!/bin/bash

g++ -g -o sssim *cpp -lm -lgsl -lgslcblas -lboost_thread
cp sssim ..
