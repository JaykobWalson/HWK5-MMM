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
	double seqTime = 0;
	if(strcmp(argv[0], "./mmm") == 0){
		if(strcmp(argv[1], "S") == 0){
			size = atoi(argv[2]);
			mmm_init();
			mmm_seq();
			mmm_reset(matrix1);
			mmm_reset(matrix2);
			mmm_reset(matrix3);
			for(int i = 0 ; i < 3; i++){
				mmm_init();
				clockstart = rtclock();
				mmm_seq();
				clockend = rtclock();
				time += (clockend - clockstart);
				mmm_reset(matrix1);
				mmm_reset(matrix2);
				mmm_reset(matrix3);
			}
			
			mmm_freeup();
			printf("=================\n");
			printf("Mode: Sequential\n");
			printf("Threads: 1\n");
			printf("Size: %d\n", size);
			printf("Time taken: %.6f sec\n", time/3);
			printf("=================\n");
		}
		else if(strcmp(argv[1], "P") == 0){
			size = atoi(argv[3]);
			numThreads = atoi(argv[2]);
			mmm_init();
			thread_args *args = (thread_args*) malloc (numThreads * sizeof(thread_args));
			for(int i = 0; i < numThreads; i++){
				args[i].tid = i;
				args[i].row_start = i * size/numThreads;
				args[i].row_end = (i + 1) * size/numThreads -1;
			}
			pthread_t *threads = (pthread_t*) malloc(numThreads * sizeof(pthread_t));
			for(int i = 0; i < numThreads; i++){
				pthread_create(&threads[i], NULL, mmm_par, &args[i]);
			}

			for(int i = 0; i < numThreads; i++){
				pthread_join(threads[i], NULL);
			}
				for(int i = 0; i < 3; i++){
					clockstart = rtclock();
					thread_args *args = (thread_args*) malloc (numThreads * sizeof(thread_args));
					for(int i = 0; i < numThreads; i++){
						args[i].tid = i;
						args[i].row_start = i * size/numThreads;
						args[i].row_end = (i + 1) * size/numThreads -1;
					}		
					pthread_t *threads = (pthread_t*) malloc(numThreads * sizeof(pthread_t));
					for(int i = 0; i < numThreads; i++){
						pthread_create(&threads[i], NULL, mmm_par, &args[i]);
					}

					for(int i = 0; i < numThreads; i++){
						pthread_join(threads[i], NULL);
					}
					clockend = rtclock();
					time += clockend-clockstart;
					mmm_reset(matrix1);
					mmm_reset(matrix2);
					mmm_reset(matrix4);

				}
				mmm_init();
				mmm_seq();
				mmm_reset(matrix1);
				mmm_reset(matrix2);
				mmm_reset(matrix3);
				for(int i = 0; i < 3; i++){
					mmm_init();
					clockstart = rtclock();
					mmm_seq();
					clockend = rtclock();
					seqTime += (clockend - clockstart);
					mmm_reset(matrix1);
					mmm_reset(matrix2);
					mmm_reset(matrix3);
				}
				

			double store = mmm_verify();
			printf("=================\n");
			printf("Mode: Parallel\n");
			printf("Threads: %d\n", numThreads);
			printf("Size: %d\n", size);
			printf("Time taken: %.6f sec\n", time/3);
			printf("Sequential Time: %.6f\n", seqTime/3);
			printf("Parallel Time: %.6f\n", time/3);
			printf("Largest Error: %.6f\n", store);
			printf("=================\n");
			mmm_freeup();
			
		}
		clockend = rtclock(); // stop clocking
	}
	

	// end: stuff I want to clock
	return 0;
}
