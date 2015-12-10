#include "rs_ec.h"
#include<pthread.h>

extern int A [M][K];// = { {1,4}, {2,5}, {3,6} };
extern int B [K][N];// = { {8,7,6}, {5,4,3} };
extern int C [M][N];

struct v {
	int i; /* row */
	int j; /* column */
};

void *runner(void *param); /* the thread */


void *runner(void *s) {
	int slice = (int *)s;
	int from = slice;
	int to = slice + 1;
	int i,j,k;
	for(i = from ; i < to ; i++){
		for(j=0; j< N ; j++){
			for(k = 0; k < K ; k++){
				C[i][j] += C[i][j] + A[i][k] * B[k][j];
			}
		}
	}
}

void multiplyp()
{
	int i,j, count = 0;
	pthread_t tid[M];// = (pthread_t *)malloc (sizeof(pthread_t) * N);       //Thread ID
	
	for(i = 0; i < M; i++) {
		pthread_create(&tid[i], NULL, runner, (void *)i);	
	}
	for(i = 0; i < M; i++) {
		pthread_join(tid[i], NULL);
	}
}

void multiplys()
{
	int i,j,k;
	// multiply. and store product in C
	for(i = 0 ; i < M ; i++){
		for(j=0; j< N ; j++){
			for(k = 0; k < K ; k++){
				C[i][j] += C[i][j] + A[i][k] * B[k][j];
			}
		}
	}
}

/*
int main()
{
	multiply();

		ff=/dev/urandom of=qiannan bs=4096 count=10000or(j = 0; j < N; j++) {
			printf("%d ", C[i][j]);
		}
		printf("\n");
	}
}
*/
