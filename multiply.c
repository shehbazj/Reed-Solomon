
#include "rs_ec.h"
#include<pthread.h>
#include<stdlib.h>


#define SET_AFFINITY 1

#define CPUSET_SIZE 4

extern int A [M][K];// = { {1,4}, {2,5}, {3,6} };
extern int B [K][N];// = { {8,7,6}, {5,4,3} };
extern int C [M][N];

struct v {
	int i; /* row */
	int j; /* column */
};

void *runner(void *param); /* the thread */


void *runner(void *s) {
#ifdef SET_AFFINITY
	int p, q;
    cpu_set_t cpuset;
    pthread_t thread;

    thread = pthread_self();
    /* Set affinity mask to include CPUs 0 to 7 */

    CPU_ZERO(&cpuset);
    CPU_SET(random() % CPUSET_SIZE, &cpuset);

//	printf("THREAD CHoose CPU =%d\n", thread % CPUSET_SIZE );
    p = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
    if (p != 0)
        printf("error %d pthread_setaffinity_np",p);

    /* Check the actual affinity mask assigned to the thread */
//    p = pthread_getaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
  //  if (p != 0)
//	printf("pthread_getaffinity_npi %d", p);

 //   printf("Set returned by pthread_getaffinity_np() contained:\n");
  //  for (q = 0; q < CPUSET_SIZE; q++)
    //    if (CPU_ISSET(q, &cpuset))
      //      printf("%d    CPU %d\n",thread % CPUSET_SIZE);

#endif



	int slice = (int *)s;
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
			for(k = 0; k < K ; k++){
		for(j=0; j< N ; j++){
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
