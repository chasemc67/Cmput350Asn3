#include "Grid.H"
	
Grid::Grid(int width, int height) {
	// Create width/height grid in memory
	// each entry will be an int representing a tile in 
	// the tile type enum
	this->width = width;
	this->height = height;
	map = new int[width*height];
}
	

Grid::~Grid(){
	// destroy the created 2 dimensional array, as well as
	// any data members on the heap
	delete [] map;
}

int Grid::getWidth() const {
	return width;
}

int Grid::getHeight() const {
	return height;
}

Grid::Tile Grid::getTile(int x, int y) const {
	return static_cast<Tile>(map[x + y*width]);
}

bool Grid::isConnected(int size, int x1, int y1, int x2, int y2) const {
	// check if object with size can reside on x1, y1
	if (!canFit(size, x1, y1))
		return false;
	// Check if object can move in direction


	// For each direction object can move, recursively call isConnected.
	// Base case is if object can move from x1, y1 to x2, y2 in 1 move

	// Cache result in some data member
	// Return the OR of all the results

	return true;
}

int Grid::findShortestPath(int size, int x1, int y1, int x2, int y2, 
                       std::vector<Direction> &path) const {
	return 1;
}

void Grid::setTile(int x, int y, Tile tile) {
	map[x + y*width] = tile;
}