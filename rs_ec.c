#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdbool.h>
#include<errno.h>
#include<assert.h>
#include<time.h>


//#include"galois.h"
#include "rs_ec.h"

//#define USE_PTHREAD

int A[M][K];
int B[K][N];
int C[M][N];

extern void multiplys();

int power(int x, int y){
	if(y == 0)
		return 1;
	else
		return x * power(x , y-1);
}

/*
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
*/

float **invert(int **B, int *missingRows, int numRows , int numCols){
// TODO replace this by invert function
    int i,j;
    float **B_prime = (float **)malloc (sizeof (float *) * numCols);  //F-- distribution matrix
	for(i = 0; i < numCols; i++){
//		B_prime[i] = (float *)malloc(2 * sizeof(float) * (numCols));
		B_prime[i] = (float *)malloc(sizeof(float) * (numCols));
	}
    float **B_inv = (float **)malloc (sizeof (float *) * numCols);  //F-- distribution matrix
	for(i = 0; i < numCols; i++){
		B_inv[i] = (float *)malloc(2 * sizeof(float) * (numCols));
	}

// TODO replace this by invert function
	return B_prime;/*
    // calculate B_prime
    int k = 0;
    int p = 0;
    float t,x;
    for( i = 0; i < numCols; i++)
    {
        if (i == missingRows[p]){
            if(p < numRows - numCols)
                {p++;}
            for(j = 0; j < numCols; j++)
                {
                    B_prime[i][j] = B[i+p][j];
                }
			}
        else{
            for(j = 0; j < numCols; j++)
            {
                B_prime[i][j] = B[i+p][j];
            }
            }
    }

    //calculate invert(B_prime)
     for(i = 0;i < numCols;i++){
            for(j = 0; j < (2 * numCols); j++)
        {
                if(j < numCols)  B_inv[i][j]= B_prime[i][j];
                else if (j == numCols + i)  B_inv[i][j] = 1.0;
                else  B_inv[i][j] = 0.0;
        }
        for(i = 0; i < numCols; i++)
        {
                for(k = 0;k < numCols; k++)
                {
                if(k != i)
                        {
                         t =  B_inv[k][i] /  B_inv[i][i];
                        for(j = 0 ; j < (2 * numCols) ; j++)
                                {
                                        x =  B_inv[i][j] * t;
                                        B_inv[k][j] =  B_inv[k][j] - x;
                                }
                        }
                }
        }
        for(i = 0; i < numCols; i++)
        {
                t =  B_inv[i][i];
                for( j = 0; j < ( 2 * numCols); j++)
                     B_inv[i][j] =  B_inv[i][j] / t;
        }
        int y = 1;

        for(i = 0; i < numCols; i++)
        y = y *  B_inv[i][i];
        if(y == 0)
        printf("sorry, it does not have an invert matrix \n");
     }

    for ( i = 0;i < numCols;i++){
        free(B_prime[i]);
    }
    free(B_prime);
    return B_inv;*/
}

void encode(int inputFD, int *outFiles)
{
	// alloc matrix
	int i,j,k;

	outFiles = (int *)malloc(sizeof(int) * M);
	static char *filePath = "out/file";
	char *fileName = (char *)malloc(strlen(filePath) +6 );
	for(i = 0 ; i < M; i++){
//		strcpy(fileName, filePath);
//		char name[4];
//		name[0] = (char)random();
//		name[1] = (char) random(); 
//		name[2] = (char) random();
//       		name[3] = (char) random();
//		strcpy(&fileName[(strlen(filePath))],name );
		outFiles[i] = open("/dev/null",O_RDWR | O_CREAT , S_IRWXU);
	}

	// get size of file
	struct stat buf;
	fstat(inputFD,&buf);
	off_t size = buf.st_size;

	printf("file size = %zd\n", size);
	// initialize coder matrix

	for(i = 0 ; i < K ; i++)
		for(j=0; j < K; j++){
			if(i == j)
				A[i][j] = 1;
			else
				A[i][j] = 0;
		}
	for(i = 0 ; i < M-K ; i++)	// parity
		for(j = 0 ; j < K ; j++){
			A[i+K][j] = power(j, i);
		}

	bool done = false;
	while(!done){
		// intitalize D - data matrix
		for(i=0; i < K ; i++){
			for(j=0; j < N ; j++){
	       			int bytesRead = read(inputFD, &B[i][j], sizeof(int));
				if(bytesRead  == 0){
					done = true;
				}/*else{	// check if data was being read
					printf("read %d data\n", bytesRead);
				}*/
			}
		}
		clock_t t1, t2;
		t1 = clock();

#ifdef USE_PTHREAD
		multiplyp();
#else
		multiplys();
#endif	

		t2 = clock();
			
		float diff = (((float)t2 - (float)t1) / 1000000.0F ) * 1000;   
		 printf("\nTIME TAKEN = %f\n",diff);   
		// copy to different files
		for(i=0;i< M; i++){
			for(j=0 ; j < N ; j++)
			if(write(outFiles[i], &C[i][j], sizeof(int))==-1){
				printf("Error during write:%s", strerror(errno));
			}
		}
	}	// repeat

	free(outFiles);
}
/*
void decode(int fd, int* outputFiles, int n, int m, int *missingBlockNumbers, int numMissingBlocks)
{

	// D = B_dash * R

	// alloc matrix
	int i,j,k;
	int dataBlockSize = 4;	// number of parallel Data Blocks to be used

	// data matrix
	int **D = (int **)malloc (sizeof (int *) * n);
	for(i=0; i < n ; i++){
		D[i] = (int *)malloc(sizeof(int) * dataBlockSize);
	}

	int **B = (int **)malloc (sizeof (int *) * (n+m));
	for(i=0; i < n+m ; i++){
		B[i] = (int *)malloc(sizeof(int) * n);
	}

	int **R = (int **)malloc(sizeof(int *) * (n));
	for(i=0; i < n +m ; i++){
		R[i] = (int *)malloc(sizeof(int) * dataBlockSize);
	}

	// get size of file
	//struct stat buf;
	//fstat(inputFD,&buf);
	//off_t size = buf.st_size;

	//printf("file size = %zd\n", size);
	// initialize coder matrix

	for(i = 0 ; i < n ; i++)
		for(j=0; j < n; j++){
			if(i == j)
				B[i][j] = 1;
			else
				B[i][j] = 0;
		}
	for(i = 0 ; i < m ; i++)
		for(j = 0 ; j < n ; j++){
			B[i+n][j] = power(j, i);
		}

//	float **BDash = invert(B, missingBlockNumbers, (n + m) , n);

	float **BDash = (float **)malloc (sizeof (float *) * K);  //F-- distribution matrix
	for(i = 0; i < K; i++){
		BDash[i] = (float *)malloc(sizeof(float) * K);
	}

	bool done = false;
	while(!done){
		// intitalize R - data + parity matrix
		for(i=0; i < n ; i++){
			for(j=0; j < dataBlockSize ; j++){
	       			int bytesRead = read(outputFiles[i], &R[i][j], sizeof(int));
				if(bytesRead  == 0){
					done = true;
				}
			}
		}

		// multiply. and store product in R
		for(i = 0 ; i < n ; i++){
			for(j=0; j< dataBlockSize ; j++){
				for(k = 0; k < n ; k++){
					D[i][j] += D[i][j] + ((int)BDash[i][k]) * R[k][j];
				}
			}
		}

		// copy to outputFile
		for(i=0;i<n; i++){
			for(j=0 ; j < dataBlockSize ; j++)
			if(write(fd, &D[i][j], sizeof(int))==-1){
				printf("%s():%d:Error writing to outfile\n",__func__ , __LINE__);
			}
		}
	}	// repeat

	// free memory
	for (i=0; i < n ; i++){
		free(D[i]);
	}
	free(D);
	for (i=0; i < n + m ; i++){
		free(B[i]);
	}
	free(B);
	for(i = 0; i< n ; i++){
		free(R[i]);
	}
	free(R);
	for(i = 0; i< n ; i++){
		free(BDash[i]);
	}
	free(BDash);
}
*/
int main(int argc, char *argv[])
{
	if(argc != 2){
		printf("Usage ./rs_ec <filename>\n \
		\tfilename - name of the input file to be encoded\n");
		return 1;
	}

	int fd = open(argv[1],O_RDONLY);
	if(fd < 0){
		printf("Error opening input file %s\n", argv[1]);

		printf("Generating Input Data...\n");

		if(system("dd if=/dev/urandom of=infile bs=4096 count=2560") == -1){
			printf("could not create input file\n");
			assert(0);
		}

		printf("Input Data Generation Completed!\n");
	}

	if(system("rm -rf out; mkdir out") == -1){
		printf("unable to create output folder\n");
		assert(0);
	}

	int *outFiles;
/*
	if(M >= 10){ // n + m 
		printf("Cannot create more than 10 out files. Please fix file naming convention in encode function\n");
		assert(0);
	}
*/
	printf("Encode Start...\n");

	// TODO : Handle outFile Management outside encode.
	encode(fd , outFiles);
	printf("Encode Completed!\n");
	close(fd);
/*
	fd = open("out/outfile",O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);

	int missingBlockNumbers[1];
	missingBlockNumbers[0]=  0;
	int numMissingBlocks = 1;

	// assign all active files to outFiles array.
	// we need to read from this outfile into array R dataBlockSize bytes at a time.
	//

	outFiles = (int *)malloc(sizeof(int) * (K));
	static char *outFilePath = "out/file";
	bool skipFileDescriptor = false;
	int count=0;
	int i, j;
	char *outFileName = (char *)malloc(strlen(outFilePath) +3 );

	for(i = 0 ; i < M; i++){
		skipFileDescriptor = false;
		for(j = 0 ; j < numMissingBlocks ; j++){
			if(missingBlockNumbers[j] == i){
				skipFileDescriptor = true;	// do not try to open/read from this file
				break;
			}
		}
		if(skipFileDescriptor)
			continue;	// move to next file among data + parity files.
		strcpy(outFileName, outFilePath);
		printf("i = %d, filestr = %c", i, (char)(i+48));
		outFileName[(strlen(outFilePath))] = (char)(i + 48);
		outFileName[strlen(outFilePath)+1] = '\0';
		outFiles[count] = open(outFileName,O_RDONLY);
		if(outFiles[count] == -1){
			printf("File %s open failed %s\n",outFileName, strerror(errno));
			exit(1);
		}else{
			printf("file %s open successful \n", outFileName);
		}
		count++;
	}

	free(outFileName);

	printf("Decode Start...\n");
//	decode(fd, outFiles , K , M-K , missingBlockNumbers, 1);
	free (outFiles);
	printf("Decode End\n");
*/
	return 0;
}
