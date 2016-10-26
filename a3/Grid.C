#include "Grid.H"
#include <iostream>
	
Grid::Grid(int width, int height) {
	// Create width/height grid in memory
	// each entry will be an int representing a tile in 
	// the tile type enum
	this->width = width;
	this->height = height;
	int size = width*height;
	map = new int[size];
	floodMap = new bool[size];
	std::fill(floodMap, floodMap+size, false);
}
	

Grid::~Grid(){
	// destroy the created 2 dimensional array, as well as
	// any data members on the heap
	delete [] floodMap;
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
	std::cout << "Checking for connection" << std::endl;
	if (!canFit(size, x1, y1)) {
		std::cout << "Cant fit at current spot" << std::endl;
		return false;
	}

	// if floodmap is not accurate, then re-create it
	if (!getFloodMap(x1, y1)) {
		// zero floodmap, and set current point as reachable
		std::cout << "zeroing" << std::endl;
		std::fill(floodMap, floodMap+size, false);
		flood(size, x1, y1);
	}
	// check cache. Assumes that only units of same
	// size are at play. otherwise need different caches
	// for each different size or something
	if (getFloodMap(x1, y1) || getFloodMap(x2, y2)) {
		std::cout<< "spot is in cache" << std::endl;
		return true;
	}
	std::cout << "default" << std::endl;
	return false;
}

int Grid::findShortestPath(int size, int x1, int y1, int x2, int y2, 
                       std::vector<Direction> &path) const {
	return 1;
}

void Grid::setTile(int x, int y, Tile tile) {
	map[x + y*width] = tile;
}

bool Grid::canFit(int size, int x1, int y1) const {
	// Check for out of bounds
	std::cout << "Width: " << width << std::endl;
	if (x1 < 0 || y1 < 0 || x1 >= this->width || y1 >= this->height)
		return false;

	bool canFit = true;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (getTile(x1+i, y1+i) != getTile(x1, y1) || getTile(x1+i, y1+i) == BLOCKED){
				canFit = false;
			}
		}
	}
	return canFit;
};