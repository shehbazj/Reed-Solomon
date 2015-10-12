#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdbool.h>
#include<errno.h>

#include"galois.h"

int power(int x, int y){
	if(y == 0)
		return 1;
	else
		return x * power(x , y-1);
}

print2DMatrix(int **F, int row, int col)
{
	int i, j;
	printf("\n");
	for(i = 0 ; i< row ; i++){
		for(j=0 ; j < col; j++){
			printf("%d\t", F[i][j]);
		}
		printf("\n");
	}
}

void encode(int fd, int n , int m, int outputFileMode)
{
	// alloc matrix
	int *D = (int *)malloc (sizeof (int) * n);
	int **F = (int **)malloc (sizeof (int *) *(n + m));
	int i,j,k;
	for(i=0; i < n + m; i++){
		F[i] = (int *)malloc(sizeof(int) * (n));
	}			
	int **C = (int **)malloc(sizeof(int *) * (n+m));
	for(i=0; i < 1; i++){
		C[i] = (int *)malloc(sizeof(int) * (1));
	}			




	// get size of file
	struct stat buf;
	fstat(fd,&buf);

	off_t size = buf.st_size;
	
	printf("file size = %zd\n", size);
	// initialize coder matrix

	for(i = 0 ; i < n ; i++)
		for(j=0; j < n; j++){
			if(i == j)
				F[i][j] = 1;
			else
				F[i][j] = 0;
		}
	for(i = 0 ; i < m ; i++)
		for(j = 0 ; j < n ; j++){
			F[i+n][j] = power(j, i);	
		}

	bool done = false;
	while(!done){
	
	// intitalize D - data matrix

		int bytesRead = read(fd, D, n * sizeof(int));
		if(bytesRead  == 0){
			done = true;
		}		
		// multiply. and store checksum in C 
/*
		for(i = 0 ; i < n+m ; i++){
			for(j=0; j< 1 ; j++){
				for(k = 0; k < n ; k++){
					C[i][j] += C[i][j] + F[i][k] * D[k][j];
				}
			}
		}	
*/	
	}
	print2DMatrix(C, m+n, 1);
	

	// copy to different files

	// repeat 

	// free memory
	free(D);
	for (i=0; i < n; i++){
		free(F[i]);
	}	
	free(F);
	for(i = 0; i< n + m; i++){
		free(C[i]);
	}
	free(C);
}

int main(int argc, char *argv[])
{

/*
  printf("%u\n", galois_log(x, w));
*/
	if(argc != 5){
		printf("Usage ./rs_ec <filename> <n> <m> <0 | 1>\n \
		\tfilename - name of the input file to be encoded\n \
		\tn - code block size\n \
		\tm - number of parities\n \
		\tnull | file\n0 - write encoded files to null\n \
		\t1 - write encoded files to disk\n");
		return 1;
	}

	int fd = open(argv[1],O_RDONLY);
	if(fd < 0){
		printf("Error opening file %s\n", argv[2]);
		return 1;
	}

	int n, m, outputFileMode;
	n = atoi (argv[2]);
	m = atoi (argv[3]);
	outputFileMode = atoi (argv[4]);

	encode(fd, n , m, outputFileMode);
		
	return 0;
}
