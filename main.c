/*
    Simple program to use the pgm library

    Gilberto Echeverria
    17/11/2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "gol.h"


int main(int argc, char const *argv[])
{
	int n_threads, n_generations;;
	char *in_filename;
	pgm_t grid, copy;

	if(argc != 4) {
		printf("Usage ./main [input_file.pgm] [number of threads] [number of generations]");
	}

	in_filename = malloc(4096);
	// out_filename = malloc(4096);

	sscanf(argv[1], "%s", in_filename);
	sscanf(argv[2], "%d", &n_threads);
	sscanf(argv[3], "%d", &n_generations);
	printf("Running GOL on %s with %d threads and %d generations", in_filename, n_threads, n_generations);

	//Read the filename into the grid
	readPGMFile(in_filename, &grid);
	
	//Set the copy's values for width and height
	copy.image.width = grid.image.width;
	copy.image.height = grid.image.height;
	//Allocate the copy
	allocateImage(&copy.image);
	/*
		Validate number threads. It must be a 
		multiple of grid.height
	*/
	if(grid.image.height % n_threads != 0){
		printf("Number of threads must have to divide the image exactly\n");
		return 1;
	}
	//Initialize pthreads array;
	pthread_t threads[n_threads];
	
	char *point, path_filename_w_extension[4096];
	point = strchr(in_filename, '.');
	strncpy(path_filename_w_extension, in_filename, point - in_filename);
	printf("%s\n", path_filename_w_extension);
	initializeThreads(threads, &grid.image, &copy.image, n_threads, n_generations, path_filename_w_extension);
	return 0;
}