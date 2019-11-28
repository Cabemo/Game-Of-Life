#include <stdio.h>
#include <omp.h>

int main() {
	// #pragma omp parallel 
	for (size_t i = 0; i < 4; i++)
	{
		
		printf("Hello\n");
	}
	

	return 0;
}