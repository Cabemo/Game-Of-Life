/*
    Simple program to use the pgm library

    Gilberto Echeverria
    17/11/2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gol.h"

int main(int argc, char const *argv[])
{
	int n_threads;
	//sscanf(argv[1], "%d", &n_threads);
	//printf("%d", n_threads);

	char *in_filename = "Boards/pulsar.pgm";
	char *out_filename = "sample_2.pgm";
	pgm_t grid, copy;

	readPGMFile(in_filename, &grid);

	copy.image.width = grid.image.width;
	copy.image.height = grid.image.height;
	allocateImage(&copy.image);
	copyPGM(&grid, &copy);

	n_threads = 1;
	//validate threads
	if (grid.image.height % n_threads != 0)
	{
		printf("Number of threads must have to divide the image exactly\n");
		//return -1;
	}

	t_info info;

	info.grid = &grid.image;
	info.copy = &copy.image;
	info.n_threads = 1;
	info.offset = 0;

	evolveArea((void *)&info);

	// Convert the ASCII format into Binary, to generate smaller images
	// Change the P2 format to P5
	sscanf("P2", "%s", copy.magic_number);
	//negativePGM(&grid);
	writePGMFile(out_filename, &copy);

	return 0;
}