/**************************
 * Author: Emilio E. G. Cantón Bermúdez
 * Date: 28/11/2019
**************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gol.h"

int main(int argc, char const *argv[])
{
	int n_generations;
	char *in_filename;
	pgm_t grid, copy;

	if(argc != 3) {
		printf("Usage ./simple [input_file.pgm] [number of generations]");
	}

	in_filename = malloc(4096);
	// out_filename = malloc(4096);

	sscanf(argv[1], "%s", in_filename);
	sscanf(argv[2], "%d", &n_generations);
	printf("Running GOL on %s with %d generations", in_filename, n_generations);

	readPGMFile(in_filename, &grid);

	copy.image.width = grid.image.width;
	copy.image.height = grid.image.height;
	allocateImage(&copy.image);

	char *point, out_filename[4096];
	t_info info;

	point = strchr(in_filename, '.');
	strncpy(out_filename, in_filename, point - in_filename);

	info.grid = &grid.image;
	info.copy = &copy.image;
	info.output_file = out_filename;
	info.n_generations = n_generations;
	info.n_threads = 1;
	info.offset = 0;


	evolveArea(&info);
	return 0;
}