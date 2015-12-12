
// exporting threads OMP_NUM_THREADS=2
// compiling with openmp use flag -fopenmp
// eg. gcc -o omp_helloc -fopenmp omp_hello.c

#include "rs_ec.h"
#include<pthread.h>
#include<stdlib.h>
#include<stdio.h>

#define CPUSET_SIZE 4

#ifdef USE_PTHREAD
void *runner(void *param); // the thread 

struct param{
	int N;
	int K;
	int s;
	int **A;
	int **B;
	int **C;
};

void *runner(void *s) {
#ifdef SET_AFFINITY
	int a, b;
    cpu_set_t cpuset;
    pthread_t thread;

    thread = pthread_self();
    // Set affinity mask to include CPUs 0 to 7 

    CPU_ZERO(&cpuset);
    CPU_SET(random() % CPUSET_SIZE, &cpuset);

    a = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
    if (a != 0)
        printf("error %d pthread_setaffinity_np",a);
#endif
	struct param p = *((struct param *)s);

	int slice = p.s;
	int K = p.K;
	int N = p.N;
	int **A = p.A;
	int **B = p.B;
	int **C = p.C;

	int from = slice;
	int to = slice + 1;
	int i,j,k;
	for(i = from ; i < to ; i++){
			for(k = 0; k < K ; k++){
		for(j=0; j< N ; j++){
				C[i][j] += C[i][j] + A[i][k] * B[k][j];
			}
		}
	}
}

void multiplyp(int **C, int **A, int **B, int M, int K, int N)
{
	int i,j, count = 0;
	pthread_t *tid = (pthread_t *)malloc(sizeof(pthread_t) * M);// = (pthread_t *)malloc (sizeof(pthread_t) * N);       //Thread ID
	struct param p;
	p.N = N;
	p.K = K;
	p.A = A;
	p.B = B;
	p.C = C;
	for(i = 0; i < M; i++) {
		p.s = i;
		pthread_create(&tid[i], NULL, runner, (void *)&p);	
	}
	for(i = 0; i < M; i++) {
		pthread_join(tid[i], NULL);
	}
	free(tid);
}
#endif
#ifdef USE_SEQUENTIAL 
void multiplys(int **C, int **A, int **B, int M, int K, int N)
{
	int i,j,k;
	// multiply. and store product in C
	for(i = 0 ; i < M ; i++){
			for(k = 0; k < K ; k++){
		for(j=0; j< N ; j++){
				C[i][j] += C[i][j] + A[i][k] * B[k][j];
			}
		}
	}
}

#endif

#ifdef USE_OPENMP
int multiplyo(int **C, int **A, int **B, int M, int K, int N)
{
  int i,j,k;
#pragma omp parallel shared(C,A,B) private(i,j,k) 
   {
#pragma omp for  schedule(static)
   for (i=0; i<M; i=i+1){
      for (j=0; j<N; j=j+1){
         C[i][j]=0;
         for (k=0; k<K; k=k+1){
            C[i][j]=(C[i][j])+((A[i][k])*(B[k][j]));
         }
      }
   }
   }
   return 0;
}
#endif
/*
int main()
{
	int a,b,c;
	// R = P * Q
//	float P[M][K], Q [K][N], R[M][N];

	float **P = (float **) malloc(sizeof(float *) * M);
	for( a = 0 ; a < M ; a++)
		P[a] = (float *) malloc (sizeof(float) * K);

	float **Q = (float **) malloc(sizeof(float *) * K);
	for( a = 0 ; a < K ; a++)
		Q[a] = (float *) malloc (sizeof(float) * N);

	float **R = (float **) malloc(sizeof(float *) * M);
	for( a = 0 ; a < M ; a++)
		R[a] = (float *) malloc (sizeof(float) * N);
	
	for(a = 0; a < M ; a++)
		for(b = 0; b < K; b++)
			P[a][b] = a+b;
	for(a = 0; a < K ; a++)
		for(b = 0; b < N; b++)
			Q[a][b] = a+b;

	multiplyo(M, N, K, R, P, Q);

	
	for( a = 0 ; a < M ; a++)
		free(P[a]);

	free(P);

	for( a = 0 ; a < K ; a++)
		free(Q[a]);

	free(Q);

	for( a = 0 ; a < M ; a++)
		free(R[a]);

	free(R);	

//
//	for(a = 0; a < M ; a++)
//		for(b = 0; b < N; b++)
//			printf("%f\t",R[a][b]);
//		printf("\n");
//
}
*/
