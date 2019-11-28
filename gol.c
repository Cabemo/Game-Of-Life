/**************************
 * Author: Emilio E. G. Cantón Bermúdez
 * Date: 28/11/2019
**************************/

#include "gol.h"


mybarrier barrier;

//Function to start all threads
void initializeThreads(pthread_t threads[], image_t *grid, image_t *copy, int n_threads, int n_generations, char* output_file){
	barrier_init(&barrier, n_threads);

	t_info info[n_threads];
	for (size_t i = 0; i < n_threads; i++)
	{

		info[i].grid = grid;
		info[i].copy = copy;
		info[i].n_threads = n_threads;
		info[i].n_generations = n_generations;
		info[i].output_file = output_file;
		// strcpy(output_file, info.output_file);
		info[i].offset = (grid->height / n_threads) * i;
	}
	for (size_t i = 0; i < n_threads; i++)
	{
		info[i].offset = (grid->height / n_threads) * i;
		pthread_create(&threads[i], NULL, &evolveArea, (void *)&info[i]);
	}

	barrier_destroy(&barrier);

	for (size_t i = 0; i < n_threads; i++)
	{
		pthread_join(threads[i], NULL);
	}
	
}
//Every thread will execute this function applying the rules
void* evolveArea(void *args) {
	t_info *info = (t_info *) args;
	int live_neighbors = 0, counter = 0;
	int n_rows = (info->grid->height / info->n_threads);

	while(counter++ < info->n_generations) {
		for (size_t i = info->offset; i < info->offset + n_rows; i++)
		{
			for (size_t j = 0; j < info->grid->width; j++)
			{
				live_neighbors = checkNeighbors(info->grid, i, j);
				info->copy->pixels[i][j].value = checkStatus(info->grid->pixels[i][j].value, live_neighbors);
			}
		}
		if(info->n_threads > 1) {
			barrier_wait(&barrier);
		}
		for (size_t i = info->offset; i < info->offset + n_rows; i++)
		{
			for (size_t j = 0; j < info->grid->width; j++)
			{
				info->grid->pixels[i][j].value = info->copy->pixels[i][j].value;
			}
		}
		if(!info->offset) {
			pgm_t c;
			char output_filename[4096], cn[6];
			c.image = *(info->copy);
			sscanf("P2", "%s", c.magic_number);
			
			sprintf(cn, "%d", counter);
			strcpy(output_filename, info->output_file);
			strcat(output_filename, cn);
			strcat(output_filename, ".pgm");
			printf("Writing to %s\n", output_filename);
			writePGMFile(output_filename, &c);
		}
	}
	if(info->n_threads > 1) {
		pthread_exit(NULL);
	}
}
int checkNeighbors(image_t *grid, int row, int col) {
	int live_neighbours = 0;

	//Center
	if (row > 0 && col > 0 && row < grid->width - 1 && col < grid->height - 1)
	{
		live_neighbours += grid->pixels[row][col - 1].value; //Left
		live_neighbours += grid->pixels[row - 1][col].value; //Up
		live_neighbours += grid->pixels[row][col + 1].value; //Right
		live_neighbours += grid->pixels[row + 1][col].value; //Down

		live_neighbours += grid->pixels[row + 1][col - 1].value; //Left Down
		live_neighbours += grid->pixels[row - 1][col + 1].value; //Right Up
		live_neighbours += grid->pixels[row + 1][col + 1].value; //Right Down
		live_neighbours += grid->pixels[row + 1][col - 1].value; //Left Down
	}
	//Top
	else if (row == 0 && col > 0 && col < grid->width - 1)
	{
		live_neighbours += grid->pixels[row][col - 1].value;		  //Left
		live_neighbours += grid->pixels[grid->height - 1][col].value; //Up
		live_neighbours += grid->pixels[row][col + 1].value;		  //Right
		live_neighbours += grid->pixels[row + 1][col].value;		  //Down

		live_neighbours += grid->pixels[row + 1][col - 1].value;		  //Left Down
		live_neighbours += grid->pixels[grid->height - 1][col + 1].value; // Right Up
		live_neighbours += grid->pixels[row + 1][col + 1].value;		  //Right Down
		live_neighbours += grid->pixels[grid->height - 1][col].value;	 //Left Down
	}
	//Right
	else if (col == grid->width - 1 && row > 0 && row < grid->height - 1)
	{
		live_neighbours += grid->pixels[row][col - 1].value; //Left
		live_neighbours += grid->pixels[row - 1][col].value; //Up
		live_neighbours += grid->pixels[row][0].value;		 //Right
		live_neighbours += grid->pixels[row + 1][col].value; //Down

		live_neighbours += grid->pixels[row - 1][col - 1].value; //Left Up
		live_neighbours += grid->pixels[row - 1][0].value;		 //Right Up
		live_neighbours += grid->pixels[row + 1][0].value;		 //Right Down
		live_neighbours += grid->pixels[row + 1][col - 1].value; //Left Down
	}
	//Bottom
	else if (row == grid->height - 1 && col > 0 && col < grid->width - 1)
	{
		live_neighbours += grid->pixels[row][col - 1].value; //Left
		live_neighbours += grid->pixels[row - 1][col].value; //Up
		live_neighbours += grid->pixels[row][col + 1].value; //Right
		live_neighbours += grid->pixels[0][col].value;		 //Down

		live_neighbours += grid->pixels[row - 1][col - 1].value; //Left Up
		live_neighbours += grid->pixels[row - 1][col + 1].value; //Right Up
		live_neighbours += grid->pixels[0][col + 1].value;		 //Right Down
		live_neighbours += grid->pixels[0][col - 1].value;		 //Left Down
	}
	//Left
	else if (col == 0 && row > 0 && row < grid->height - 1)
	{
		live_neighbours += grid->pixels[row][grid->width - 1].value; //Left
		live_neighbours += grid->pixels[row - 1][col].value;		 //Up
		live_neighbours += grid->pixels[row][col + 1].value;		 //Right
		live_neighbours += grid->pixels[row + 1][col].value;		 //Down

		live_neighbours += grid->pixels[row - 1][grid->width - 1].value; //Left Up
		live_neighbours += grid->pixels[row - 1][col + 1].value;		 //Right Up
		live_neighbours += grid->pixels[row + 1][col + 1].value;		 //Right Down
		live_neighbours += grid->pixels[row + 1][grid->width - 1].value; //Left Down
	}
	//Upper Left Corner
	else if (row == 0 && col == 0)
	{
		live_neighbours += grid->pixels[row][grid->width - 1].value;  //Left
		live_neighbours += grid->pixels[grid->height - 1][col].value; //Up
		live_neighbours += grid->pixels[row][col + 1].value;		  //Right
		live_neighbours += grid->pixels[row + 1][col].value;		  //Down

		live_neighbours += grid->pixels[grid->height - 1][grid->width - 1].value; //Left Up
		live_neighbours += grid->pixels[grid->height - 1][col + 1].value;		  //Right Up
		live_neighbours += grid->pixels[row + 1][col + 1].value;				  //Right Down
		live_neighbours += grid->pixels[row + 1][col - 1].value;				  //Left Down
	}
	//Upper Right Corner
	else if (col == grid->width - 1 && row == 0)
	{
		live_neighbours += grid->pixels[row][col - 1].value;		  //Left
		live_neighbours += grid->pixels[grid->height - 1][col].value; //Up
		live_neighbours += grid->pixels[row][0].value;				  //Right
		live_neighbours += grid->pixels[row + 1][col].value;		  //Down

		live_neighbours += grid->pixels[grid->height - 1][col - 1].value;		  //Left Up
		live_neighbours += grid->pixels[grid->height - 1][0].value; //Right Up
		live_neighbours += grid->pixels[row + 1][0].value;				  //Right Down
		live_neighbours += grid->pixels[row + 1][col - 1].value;		  //Left Down
	}
	//Lower Left Corner
	else if (col == 0 && row == grid->height - 1)
	{		
		live_neighbours += grid->pixels[row][grid->width - 1].value;//Left
		live_neighbours += grid->pixels[row - 1][col].value;//Up
		live_neighbours += grid->pixels[row][col + 1].value;//Right
		live_neighbours += grid->pixels[0][col].value;//Down

		live_neighbours += grid->pixels[row - 1][grid->width - 1].value; //Left Up
		live_neighbours += grid->pixels[row - 1][col + 1].value;		 //Right Up
		live_neighbours += grid->pixels[0][col + 1].value;		 //Right Down
		live_neighbours += grid->pixels[0][col - 1].value;				 //Left Down
	}
	//Lower Right Corner
	else
	{
		live_neighbours += grid->pixels[row][col - 1].value; //Left
		live_neighbours += grid->pixels[row - 1][col].value; //Up
		live_neighbours += grid->pixels[row][0].value;		 //Right
		live_neighbours += grid->pixels[0][col].value;		 //Down

		live_neighbours += grid->pixels[row - 1][col - 1].value; //Left Up
		live_neighbours += grid->pixels[row - 1][col + 1].value; //Right Up
		live_neighbours += grid->pixels[0][0].value;		 //Right Down
		live_neighbours += grid->pixels[0][0].value;		 //Left Down
	}

	return live_neighbours;
}
//After every thread has finished, update the original grid
void updateArea(t_info *info){
	int n_rows = (info->grid->height / info->n_threads);
	int initial_index = n_rows * info->offset;

	for (size_t i = initial_index; i < initial_index + n_rows; i++)
	{
		for (size_t j = 0; j < info->grid->width; j++)
		{
			info->grid->pixels[i][j].value = info->copy->pixels[i][j].value;
		}
	}
}
//Rules to determine next generation
int checkStatus(int current, int neighbors) {
	//1. Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
	if(current == 1 && neighbors < 2)
		return 0;
	//2. Any live cell with two or three live neighbours lives on to the next generation.
	if(current == 1 && (neighbors == 2 || neighbors == 3))
		return 1;
	//3. Any live cell with more than three live neighbours dies, as if by overpopulation.
	if(current == 1 && neighbors > 3)
		return 0;
	//4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
	if(current == 0 && neighbors == 3)
		return 1;
	return 0;
}