#include<iostream>
//#include"galois.h"
#include<cstdio>
#include<cstdlib>

extern "C" int galois_log(int , int);

using namespace std;

int main(int argc, char *argv[])
{
  unsigned int x, w;

  if (argc != 3) {
    fprintf(stderr, "usage: gf_log x w - returns the discrete log if x in GF(2^w)\n");
    return 1;
  }

  sscanf(argv[1], "%u", &x);
  w = atoi(argv[2]);

  if (w < 1 || w > 32) { fprintf(stderr, "Bad w\n"); exit(1); }

  if (x == 0 || (w < 32 && x >= (1 << w))) { fprintf(stderr, "x must be in [1,%d]\n", (1 << w)-1); exit(1); }

  printf("%u\n", galois_log(x, w));
//  exit(0);
	
	return 0;
}
