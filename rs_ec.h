#ifndef RS_EC_H
#define RS_EC_H 1

#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <sched.h>

#include<stdio.h>
#include<stdlib.h>

#define SET_AFFINITY 

#define TRANSPOSE

// uncomment for using SEQUENTIAL

//#define USE_SEQUENTIAL

// uncomment for using PTHREAD

#define USE_PTHREAD

// uncomment for using OpenMP 
//#define USE_OPENMP

#endif
