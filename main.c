/*
	Name: Nicholas Prater
	Course: CS 481 OS
	Professor: Dr. Chiu
	Date: 3/7/23
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include "rtclock.h"
#include "mmm.h"

double **sMatrix, **pMatrix, **array1, **array2, seqTime, parTime;
int size, numThreads;

double seqThree();

int main(int argc, char *argv[]) {
	double clockstart, clockend;
	//double totalTime = 0;

	if(strcmp(argv[0],"./mmm") == 0 && argc == 1)
	{
		printf("Usage: ./mmm <mode> [num threads] <size>\n");
	}
	else if(strcmp(argv[1], "S") == 0)
	{

		if(argc == 3)
		{
			size = atoi(argv[2]);
			printf("========\nmode: sequential\nthread count: 1\nsize: %d\n========\n",size);
			double totalTime = seqThree();
			printf("Sequential Time (avg of 3 runs): %.6f sec\n", (totalTime/3));

			mmm_freeup();
		}
		else
		{
			printf("Usage: ./mmm <mode> [num threads] <size>\n");
			//notJump = false;
		}
	}
	else if(strcmp(argv[1], "P") == 0)
	{
		if(argc == 4)
		{
			size = atoi(argv[3]);
			printf("========\nmode: parallel\nthread count: %d\nsize: %d\n========\n", atoi(argv[2]), atoi(argv[3]));
			numThreads = atoi(argv[2]);

			seqTime = seqThree();
			// mmm_init();
			// mmm_seq();
			printf("Sequential Time (avg of 3 runs): %.6f\n", seqTime/3);
			clockstart = rtclock();
			
			// prepare thread arguments
			thread_args *args = (thread_args*) malloc(numThreads * sizeof(thread_args));
			for (int i = 0; i < numThreads; i++) {
				args[i].tid = i;
				args[i].begin = 0;
				args[i].end = size;
			}

			// allocate space to hold threads
			pthread_t *threads = (pthread_t*) malloc(numThreads * sizeof(pthread_t));
			for (int i = 0; i < numThreads; i++) {
				//printf("Creating Thread %d\n",i);
				pthread_create(&threads[i], NULL, mmm_par, &args[i]);
			}

			/** JOIN PHASE **/
			// wait for threads to finish; combine partially computed sum
			for (int i = 0; i < numThreads; i++) 
			{
				pthread_join(threads[i], NULL);
			}
			clockend = rtclock();

			printf("Parallel Time (avg of 3 runs): %.6f\n", parTime = clockend-clockstart);
			printf("Speedup: %.6f\n", seqTime/parTime);

			double dif = mmm_verify();
			printf("Verifying... largest error between parallel and sequential matrix: %.6f\n", dif);


			mmm_freeup();		
		}
		else
		{
			printf("Usage: parallel mode requires [num threads]\n");
		}

	}

	// if(notJump)
	// {
	// 	printf("size: %d\n========\n", size);
	// 	double totalTime = seqThree();


	// 	printf("Sequential Time (avg of 3 runs): %.6f sec\n", (totalTime/3));

	// 	mmm_freeup();
	// }

	return 0;
}
double seqThree()
{
	double clockstart, clockend;
	double totalTime = 0;

	mmm_init();
	clockstart = rtclock(); // start clocking
	mmm_seq();
	mmm_reset(array1);
	mmm_reset(array2);
	mmm_reset(sMatrix);
	clockend = rtclock(); // stop clocking
	totalTime += clockend-clockstart;

	mmm_init();
	clockstart = rtclock(); // start clocking
	mmm_seq();
	mmm_reset(array1);
	mmm_reset(array2);
	mmm_reset(sMatrix);
	clockend = rtclock(); // stop clocking
	totalTime += clockend-clockstart;

	mmm_init();
	clockstart = rtclock(); // start clocking
	mmm_seq();	
	clockend = rtclock(); // stop clocking
	totalTime += clockend-clockstart;

	return totalTime;

}


// void *partialSum(void *args) {
//   // cast input as struct thread_args
//   thread_args *params = (thread_args*) args;

//   	int b = 0;
// 	//double temp = 0;
// 	for(int i = params->begin; i < params->end; i++)
// 	{
// 		for(int j = params->begin; j < params->end; j++)
// 		{
// 			//printf("Mutiply: %f\n",  array1[i][j] * array2[j][i]);
// 			matrix[i][b] += array1[i][j] * array2[j][i];
// 			b++;

// 		}
// 		//printf("matrix = %f\n", matrix[i][b]);
// 		//temp = 0;
// 		b=0;
// 	}
//   return NULL;
// }
