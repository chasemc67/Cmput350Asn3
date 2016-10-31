#include "Grid.H"
#include <iostream>

// Shared pointers
#include <memory>
// Formated strings
#include "boost/format.hpp"
// infinity for a* alg
#include <limits>
	
Grid::Grid(int width, int height) {
	// Create width/height grid in memory
	// each entry will be an int representing a tile in 
	// the tile type enum
	this->width = width;
	this->height = height;
	int size = width*height;
	map = new int[size];
	floodMap = new bool[size];
	cachedSize = new int;
	std::fill(floodMap, floodMap+size, false);
}
	

Grid::~Grid(){
	// destroy the created 2 dimensional array, as well as
	// any data members on the heap
	delete cachedSize;
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
	// std::cout << "Checking for connection" << std::endl;
	if (!canFit(size, x1, y1)) {
		// std::cout << "Cant fit at current spot" << std::endl;
		return false;
	}

	// if floodmap is not accurate, then re-create it
	if (!getFloodMap(x1, y1) || size != *cachedSize) {
		*cachedSize = size;
		// zero floodmap, and set current point as reachable
		std::fill(floodMap, floodMap+(this->width * this->height), false);
		flood(size, x1, y1);
	}
	// check cache. Assumes that only units of same
	// size are at play. otherwise need different caches
	// for each different size or something
	if (getFloodMap(x2, y2)) {
		// std::cout<< "spot is in cache" << std::endl;
		return true;
	}
	// std::cout << "Spot is not reachable" << std::endl;
	return false;
}

int Grid::findShortestPath(int size, int x1, int y1, int x2, int y2, 
                       std::vector<Direction> &path) const {

	
	std::shared_ptr<Node> startNode(new Node(x1, y1));
	std::shared_ptr<Node> endNode(new Node(x2, y2));

	// cost nothing to get from start node to this node
	startNode->gScore = 0;
	// cost to get from start node to goal is entirely heuristic
	//startNode->fScore = getHeuristicDistance(startNode, endNode);


	// std::shared_ptr<Node> t1(new Node(0, 0));
	// std::shared_ptr<Node> t2(new Node(3, 5));
	Node * t1 = new Node(0,0);
	Node * t2 = new Node(3,5);
	std::cout << boost::format("Testing heurstic distance for 0,0 to 3,5: %d\n") % getHeuristicDistance(t1, t2);
	delete t1;
	delete t2;

	// Set of nodes already evaluated
	std::vector<Node> closedSet;
	// Set of discovered nodes to be evaluated
	// initially contains only the start node
	// Open set must be sorted, with lowest fScore at the top.
	std::vector<Node> openSet;

	std::cout << std::endl;
	std::cout << boost::format("Finding shortest path from (%d, %d) to (%d, %d)\n") % n1->x % n1->y % n2->x % n2->y;
	std::cout << "n1.x > n2.x checked with operator overlading : " << (*n1 > *n2) << std::endl;
	std::cout << "n1.x > n2.x checked with element compare : " << (n1->x > n2->x) << std::endl;
	std::cout << std::endl;

	std::shared_ptr<Node> t1(new Node(0, 0));
	std::shared_ptr<Node> t2(new Node(3, 5));
	std::cout << boost::format("Testing heurstic distance for 0,0 to 3,5: %d\n") % getHeuristic(t1, t2);
	
	std::cout << std::endl;
	std::cout << "======================================" << std::endl;	

	return 1;
}

// int getHeuristic(Node & startNode, Node & endNode) {
// 	std::cout << "Getting euclidian distance from " << startNode->x ", " << startNode->y << " to " << endNode->x ", " << endNode->y << std::endl;
// 	return 1;
// }

void Grid::setTile(int x, int y, Tile tile) {
	map[x + y*width] = tile;
}

// Check if unit can fit at coord
bool Grid::canFit(int size, int x1, int y1) const {
	// Check for out of bounds
	if (x1 < 0 || y1 < 0 || x1 >= width || y1 >= height)
		return false;

	bool canFit = true;
	for (int i = 0; i <= size; i++) {
		for (int j = 0; j <= size; j++) {
			if ( (getTile(x1+i, y1+i) != getTile(x1, y1)) || (getTile(x1+i, y1+i) == BLOCKED)){
				canFit = false;
			}
		}
	}
	return canFit;
};

// Check if unit can move 1 square in direction
// If moving diagonally we can recursively check the 2 non-diag dirs
// When we check non-diag dirs, we also check for tile type
bool Grid::canMove(int size, int x1, int y1, Direction dir) const {
	switch(dir) {
		case N:
			return (canFit(size, x1, y1-1) && tilesAreSame(size, x1, y1, x1, y1-1));
			break;
		case S:
			return (canFit(size, x1, y1+1) && tilesAreSame(size, x1, y1, x1, y1+1));
			break;
		case E:
			return (canFit(size, x1+1, y1) && tilesAreSame(size, x1, y1, x1+1, y1));
			break;
		case W:
			return (canFit(size, x1-1, y1) && tilesAreSame(size, x1, y1, x1-1, y1));
			break;
		case NE:
			return (canMove(size, x1, y1, N) && canMove(size, x1, y1, E) && tilesAreSame(size, x1, y1, x1+1, y1-1));
			break;
		case NW:
			return (canMove(size, x1, y1, N) && canMove(size, x1, y1, W) && tilesAreSame(size, x1, y1, x1-1, y1-1));
			break;
		case SE:
			return (canMove(size, x1, y1, S) && canMove(size, x1, y1, E) && tilesAreSame(size, x1, y1, x1+1, y1+1));
			break;
		case SW:
			return (canMove(size, x1, y1, S) && canMove(size, x1, y1, W) && tilesAreSame(size, x1, y1, x1-1, y1+1));
			break;
		default:
			return false;
			break;
	}
	return false;
};

// Check if the tiles are all the same at 
// points covered by unit at x1,y1 and x2,y2
bool Grid::tilesAreSame(int size, int x1, int y1, int x2, int y2) const {
	return ((getTile(x1, y1) == getTile(x2, y2)) && canFit(size, x1, y1) && canFit(size, x2, y2));
};

// Check flood cache with bounds checking
bool Grid::getFloodMap(int x, int y) const {
	if (x < 0 || y < 0 || x >= width || y >= height)
		return false;
	return floodMap[x + y*width];
};

void Grid::setReachable(int x, int y) const {
	floodMap[x + y*width] = true;
};

// Recursively set all adjacent points that can be moved to
// as moveable.
// Checks if node has already been expanded before expanding
// To stop infinite flooding
void Grid::flood(int size, int x, int y) const {
	setReachable(x, y);
	if(canMove(size, x, y, N) && !getFloodMap(x, y-1))
		flood(size, x, y-1);
	if(canMove(size, x, y, S) && !getFloodMap(x, y+1))
		flood(size, x, y+1);
	if(canMove(size, x, y, E) && !getFloodMap(x+1, y))
		flood(size, x+1, y);
	if(canMove(size, x, y, W) && !getFloodMap(x-1, y))
		flood(size, x-1, y);
	if(canMove(size, x, y, NE) && !getFloodMap(x+1, y-1))
		flood(size, x+1, y-1);
	if(canMove(size, x, y, NW) && !getFloodMap(x-1, y-1))
		flood(size, x-1, y-1);
	if(canMove(size, x, y, SW) && !getFloodMap(x-1, y+1))
		flood(size, x-1, y+1);
	if(canMove(size, x, y, SE) && !getFloodMap(x+1, y+1))
		flood(size, x+1, y+1);
};

void Grid::getHeuristicDistance(const Node & from, const Node & to) {
	sqrt(((from->x - to->x)^2) + ((from->y - to->y)^2));
}

// Node implementation

Grid::Node::Node(int x_, int y_) {
		x = x_;
		y = y_;
		gScore = std::numeric_limits<int>::max();
		fScore = std::numeric_limits<int>::max();
};

bool Grid::Node::operator>(const Node &rhs) {
	return ((this->x > rhs.x));
}

bool Grid::Node::operator<(const Node &rhs) {
	return ((this->x < rhs.x));
}