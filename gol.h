/**************************
 * Author: Emilio E. G. Cantón Bermúdez
 * Date: 28/11/2019
**************************/

#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "pgm_image.h"
#include "mybarrier.h"

/*
	Struct that each thread will have
	to determine which area of the grid
	he needs to process
*/
typedef struct{
	int n_threads;
	int n_generations;
	int offset;
	char *output_file;
	image_t *grid;
	image_t *copy;
} t_info;

//Function to start all threads
void initializeThreads(pthread_t threads[], image_t *grid, image_t *copy, int n_threads, int n_generations, char* output_file);
//Every thread will excecute this function applying the rules
void* evolveArea(void * args);
//Possible cases applied within evolveArea
int checkNeighbors(image_t *grid, int x, int y);
//Rules to determine next generation
int checkStatus(int current, int neighbors);
//After every thread has finished, update the original grid
void updateArea(t_info *info);