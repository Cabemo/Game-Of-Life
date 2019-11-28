/**************************
 * Author: Emilio E. G. Cantón Bermúdez
 * Date: 28/11/2019
**************************/


#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <string.h>

#include "gol.h"

int main(int argc, char const *argv[])
{
	int n_threads, n_generations;;
	char in_filename[4096];
	pgm_t grid, copy;

	if(argc != 4) {
		printf("Usage ./omp [input_file.pgm] [number of threads] [number of generations]");
	}

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
	sscanf("P2", "%s", copy.magic_number);
	/*
		Validate number threads. It must be a 
		multiple of grid.height
	*/
	if (grid.image.height % n_threads != 0) {
		printf("Number of threads must have to divide the image exactly\n");
		return 1;
	}

	//Get filename without extension
	char *point, out_filename[4096];
	point = strchr(in_filename, '.');
	strncpy(out_filename, in_filename, point - in_filename);

	omp_lock_t omp_lock;
	int offset_counter = 0;

	omp_init_lock(&omp_lock);
	omp_set_num_threads(n_threads);
	
	#pragma omp parallel
	{
		omp_set_lock(&omp_lock);
		int offset = (grid.image.height / n_threads) * offset_counter++;
		omp_unset_lock(&omp_lock);
		int live_neighbors = 0, counter = 0;
		int n_rows = (grid.image.height / n_threads);

		while (counter++ < n_generations)
		{
			for (size_t i = offset; i < offset + n_rows; i++)
			{
				for (size_t j = 0; j < grid.image.width; j++)
				{
					live_neighbors = checkNeighbors(&grid.image, i, j);
					copy.image.pixels[i][j].value = checkStatus(grid.image.pixels[i][j].value, live_neighbors);
				}
			}
			#pragma omp barrier
			for (size_t i = offset; i < offset + n_rows; i++)
			{
				for (size_t j = 0; j < grid.image.width; j++)
				{
					grid.image.pixels[i][j].value = copy.image.pixels[i][j].value;
				}
			}
			if (!offset)
			{
				char output_filename[4096], cn[6];

				sprintf(cn, "%d", counter);
				strcpy(output_filename, out_filename);
				strcat(output_filename, cn);
				strcat(output_filename, ".pgm");
				printf("Writing to %s\n", output_filename);
				writePGMFile(output_filename, &copy);
			}
		}
	}

	return 0;
}