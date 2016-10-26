#include "Grid.H"
	
Grid::Grid(int width, int height) {
	// Create width/height grid in memory
	// each entry will be an int representing a tile in 
	// the tile type enum
	width = width;
	height = height;
	map = Tile[width*height];

}
	

Grid::~Grid(){
	// destroy the created 2 dimensional array, as well as
	// any data members on the heap
}

int Grid::getWidth() {
	return width;
}

int Grid::getHeight() {
	return height;
}

Tile Grid::getTile(int x, int y) const {
	return map[x + y*width];
}