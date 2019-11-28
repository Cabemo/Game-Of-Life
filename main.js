		/***********
	 * 	P5
	***********/
	function windowResized() {
		resizeCanvas(windowWidth, windowHeight);
	}

	let gol;

	function setup() {
		gol = new GOL();
		createCanvas(windowWidth, windowHeight);
		// frameRate(15);
	}
// document.addEventListener('load', () => {
// 	var spacing = document.getElementById('spacing');

// 	spacing.oninput = evt => {
// 		document.getElementById('tSpacing').innerHTML = this.value;
// 		gol.separation = Number(this.value);
// 		console.log(gol.separation);
// 	};

// 	var radius = document.getElementById('radius');
// 	gol.radius = radius.value;
// 	radius.oninput = evt => {
// 		gol.radius = Number(this.value);
// 		console.log(gol.separation);
// 	};
// })

	
	function draw() {
		background(175)
		gol.draw();
		gol.update();
	}

	class GOL {
		offset = 100;
		constructor() {
			this.x = windowWidth - this.offset;
			this.y = windowHeight - this.offset;
			this.separation = 20;
			this.radius = 20;
			this.matrix = []
			this.copy = []
			for (let x = this.offset; x < this.x; x += this.separation) {
				this.matrix[x] = []
				this.copy[x] = []
				for (let y = this.offset; y < this.y; y += this.separation) {
					this.copy[x][y] = 0;
					this.matrix[x][y] = Math.round(random(0, 1));
				}
			}
		}

		draw() {
			for(let row = this.offset; row < this.x; row+=this.separation) {
				for(let col = this.offset; col < this.y; col+=this.separation) {		
					// console.log(matrix);
					if (
						mouseX > row - this.radius + this.separation &&
						mouseX < row + this.radius + this.separation &&
						mouseY > col - this.radius + this.separation &&
						mouseY < col + this.radius + this.separation
					) {
						this.matrix[row][col] = 1;
					}
					fill(this.matrix[row][col] == 1 ? 0 : 255);
					ellipse(row, col, this.radius)
				}
			}
		}

		update() {
			let n;
			for (let row = this.offset; row < this.x; row+=this.separation) {
				for (let col = this.offset; col < this.y; col+=this.separation) {
					n = this.checkNeighbours(row, col);
					this.copy[row][col] = this.checkStatus(this.matrix[row][col], n);
				}
				
			}
			for (let row = this.offset; row < this.x; row += this.separation) {
				for (let col = this.offset; col < this.y; col += this.separation) {
					this.matrix[row][col] = this.copy[row][col];
				}
			}
		}

		checkNeighbours(row, col) {
			let live_neighbours = 0;
			// console.log(row, col);
			if (row > this.offset && col > this.offset && row < this.x - this.separation && col < this.y - this.separation) {
				live_neighbours += this.matrix[row][col - this.separation]; //Left
				live_neighbours += this.matrix[row - this.separation][col]; //Up
				live_neighbours += this.matrix[row][col + this.separation]; //Right
				live_neighbours += this.matrix[row + this.separation][col]; //Down

				live_neighbours += this.matrix[row + this.separation][col + this.separation]; //RT
				live_neighbours += this.matrix[row - this.separation][col + this.separation]; //RB
				live_neighbours += this.matrix[row - this.separation][col - this.separation]; //LB
				live_neighbours += this.matrix[row + this.separation][col - this.separation]; //LT
			}
			//Top
			else if (row == this.offset && col > this.offset && col < this.x - this.separation) {
				live_neighbours += this.matrix[row][col - this.separation]; //Left
				live_neighbours += this.matrix[this.y - this.separation][col]; //Up
				live_neighbours += this.matrix[row][col + this.separation]; //Right
				live_neighbours += this.matrix[row + this.separation][col]; //Down

				live_neighbours += this.matrix[this.y - this.separation][col - this.separation]; //Left Top
				live_neighbours += this.matrix[this.y - this.separation][col + this.separation]; //Right Top
				live_neighbours += this.matrix[row + this.separation][col + this.separation]; //Right Down
				// live_neighbours += this.matrix[row - this.separation][col - this.separation]; //Left Down
			}
			//Right
			else if (col == this.x - this.separation && row > this.offset && row < this.y - this.separation) {
				live_neighbours += this.matrix[row][col - this.separation]; //Left
				live_neighbours += this.matrix[row - this.separation][col]; //Up
				live_neighbours += this.matrix[row][this.offset]; //Right
				live_neighbours += this.matrix[row + this.separation][col]; //Down

				live_neighbours += this.matrix[row + this.separation][col - this.separation]; //Left Top
				live_neighbours += this.matrix[row + this.separation][this.offset]; //Right Top
				live_neighbours += this.matrix[row - this.separation][this.offset]; //Right Down
				live_neighbours += this.matrix[row - this.separation][col - this.separation]; //Left Down
			}
			//Bottom
			else if (row == this.y - this.separation && col > this.offset && col < this.x - this.separation) {
				live_neighbours += this.matrix[row][col - this.separation]; //Left
				live_neighbours += this.matrix[row - this.separation][col]; //Up
				live_neighbours += this.matrix[row][col + this.separation]; //Right
				live_neighbours += this.matrix[this.offset][col]; //Down

				live_neighbours += this.matrix[row - this.separation][col - this.separation]; //Left Top
				live_neighbours += this.matrix[row - this.separation][col + this.separation]; //Right Top
				live_neighbours += this.matrix[this.offset][col + this.separation]; //Right Down
				live_neighbours += this.matrix[this.offset][col - this.separation]; //Left Down
			}
			//Left
			else if (col == this.offset && row > this.offset && row < this.y - this.separation) {
				live_neighbours += this.matrix[row][this.x - this.separation]; //Left
				live_neighbours += this.matrix[row - this.separation][col]; //Up
				live_neighbours += this.matrix[row][col + this.separation]; //Right
				live_neighbours += this.matrix[row + this.separation][col]; //Down

				live_neighbours += this.matrix[row + this.separation][this.x - this.separation]; //Left Down
				live_neighbours += this.matrix[row + this.separation][col + this.separation]; //Down right
				live_neighbours += this.matrix[row - this.separation][this.x - this.separation]; //Left Top
				live_neighbours += this.matrix[row - this.separation][col + this.separation]; //Right Top
			}
			//Upper Left Corner
			else if (row == this.offset && col == this.offset) {
				live_neighbours += this.matrix[row][this.x - this.separation]; //Left
				live_neighbours += this.matrix[this.y - this.separation][col]; //Up
				live_neighbours += this.matrix[row][col + this.separation]; //Right
				live_neighbours += this.matrix[row + this.separation][col]; //Down

				live_neighbours += this.matrix[this.y - this.separation][this.x - this.separation]; //Left Top
				live_neighbours += this.matrix[this.y - this.separation][col + this.separation]; //Right Top
				live_neighbours += this.matrix[row + this.separation][col + this.separation]; //Right Down
				live_neighbours += this.matrix[row + this.separation][col - this.separation]; //Left Down
			}
			//Upper Right Corner
			else if (col == this.x - this.separation && row == this.offset) {
				live_neighbours += this.matrix[row][col - this.separation]; //Left
				live_neighbours += this.matrix[this.y - this.separation][col]; //Up
				live_neighbours += this.matrix[row][this.offset]; //Right
				live_neighbours += this.matrix[row + this.separation][col]; //Down

				live_neighbours += this.matrix[row + this.separation][col - this.separation]; //Left Down
				live_neighbours += this.matrix[row + this.separation][col + this.separation]; //Right Down
				live_neighbours += this.matrix[this.y - this.separation][col - this.separation]; //Left Top
				live_neighbours += this.matrix[this.y - this.separation][this.offset]; //Right Top
			}
			//Lower Left Corner
			else if (col == this.offset && row == this.y - this.separation) {
				live_neighbours += this.matrix[row][this.x - this.separation]; //Left
				live_neighbours += this.matrix[row - this.separation][col]; //Up
				live_neighbours += this.matrix[row][col + this.separation]; //Right
				live_neighbours += this.matrix[this.offset][this.x - this.separation]; //Down

				live_neighbours += this.matrix[this.offset][this.x - this.separation]; //Left Down
				live_neighbours += this.matrix[this.offset][this.x + this.separation]; //Right Down
				live_neighbours += this.matrix[row - this.separation][col - this.separation]; //Left Up
				// live_neighbours += this.matrix[row - 11][col + this.separation]; //Right Up
			}
			//Lower Right Corner
			else {
				live_neighbours += this.matrix[row][col - this.separation]; //Left
				// live_neighbours += this.matrix[row - this.separation][col];//Up
				live_neighbours += this.matrix[row][this.offset]; //Right
				live_neighbours += this.matrix[this.offset][col]; //Down

				// live_neighbours += this.matrix[row + this.separation][col - this.separation]; //Left top
				// live_neighbours += this.matrix[row - this.separation][col + this.separation];//Up
				live_neighbours += this.matrix[this.offset][this.offset]; //Right Down
				live_neighbours += this.matrix[this.offset][col - this.separation]; //Left Down
			}
			return live_neighbours;
		}

		checkStatus(current, neighbors) {
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
	}