# Game-Of-Life
Simulator in C and visualization in p5.js of [https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life](Conway's Game of Life)

## Description

The project is divided in two parts. The javascript part is a simple webpage that simulates a grid and the Game of Life within it. The second part is a C implementation which, with help of [Gilberto Echeverría](https://github.com/gilecheverria), read and writes `pgm` files with the result of the matrix evolution. There are three parts to the `C` implementation:

1. Simple
2. Pthreaded
3. Omp implemented
