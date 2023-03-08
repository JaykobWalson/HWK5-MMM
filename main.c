#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "rtclock.h"
#include "mmm.h"
int size;
int numThreads;
double** matrix1;
double** matrix2;
double** matrix3;
double** matrix4;
int main(int argc, char *argv[]) {
	double clockstart, clockend;	
	double time = 0;
	if(strcmp(argv[0], "./mmm") == 0){
		if(strcmp(argv[1], "S") == 0){
			size = atoi(argv[2]);
			mmm_init();
			mmm_seq();
			mmm_reset(matrix1);
			mmm_reset(matrix2);
			mmm_reset(matrix3);
			
				mmm_init();
				clockstart = rtclock();
				mmm_seq();
				clockend = rtclock();
				time += (clockend - clockstart);
				mmm_reset(matrix1);
				mmm_reset(matrix2);
				mmm_reset(matrix3);
			
			mmm_freeup();
			printf("mode: sequential\n");
			printf("threads: 1\n");
			printf("size: %d\n", size);
			printf("Time taken: %.6f sec\n", time/3);
		}
		else if(strcmp(argv[1], "P") == 0){
			printf("1 \n");
			//mmm_init();
			size = atoi(argv[3]);
			numThreads = atoi(argv[2]);
			thread_args *args = (thread_args*) malloc (numThreads * sizeof(thread_args));
			for(int i = 0; i < numThreads; i++){
				args[i].tid = i;
				args[i].row_start = i * size/numThreads + 1;
				args[i].row_end = (i + 1) * size/numThreads;
			}
			printf("2 \n");
			pthread_t *threads = (pthread_t*) malloc(numThreads * sizeof(pthread_t));
			for(int i = 0; i < numThreads; i++){
				pthread_create(&threads[i], NULL, mmm_par, &args[i]);
			}
			printf("3 \n");
				// mmm_init();
				// clockstart = rtclock();
				// clockend = rtclock();
				// mmm_reset(matrix1);
				// mmm_reset(matrix2);
				// mmm_reset(matrix4);
				// time += (clockend - clockstart);
		
			double store = mmm_verify();
			printf("mode: parallel\n");
			// printf("threads: %d\n", numThreads);
			printf("size: %d\n", size);
			//printf("Time taken: %.6f sec\n", time/3);
			printf("===========\n");
			// printf("Sequential Time: %.6f", time/3);
			printf("Largest Error: %.6f", store);
			//mmm_freeup();
			printf("4 \n");
		}
	}
	

	// end: stuff I want to clock

	clockend = rtclock(); // stop clocking
	return 0;
}
