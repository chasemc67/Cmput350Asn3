#include "Grid.H"
#include <iostream>

#include <memory>
	
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

	//std::cout << "Finding shortest path from " << x1 << ", " << y1 << " to " << x2 << ", " << y2 << std::endl;


	// ** Testing 
	std::shared_ptr<Node> n1(new Node(12, 12));
	std::shared_ptr<Node> n2(new Node(10, 10));
	// Node * n1 = new Node(12, 12);
	// Node * n2 = new Node(10, 10);

	std::cout << std::endl;
	std::cout << "n1: " << n1->x << ", " << n1->y << std::endl;
	std::cout << "n2: " << n2->x << ", " << n2->y << std::endl;
	std::cout << "n1.x > n2.x checked with operator overlading : " << (*n1 > *n2) << std::endl;
	std::cout << "n1.x > n2.x checked with element compare : " << (n1->x > n2->x) << std::endl;
	std::cout << std::endl;

	delete n1;
	delete n2;
	// std::cout << boost::format("node n1: %d, %d") % n1->x % %n2->y;
	// ** Testing 

	// std::vector<Node> closedSet;
	// std::vector<Node> openSet;
	// Node * endNode = new Node(x2, y2);

	// Node * startNode = new Node(x1, y1);
	//startNode -> cameFrom = startNode;
	// startNode -> gScore = 0;
	//startNode -> fScore = getHeuristic(&startNode, &endNode);


	// openSet.push_back(startNode);

	// closedSet   	// The set of nodes already evaluated
	// openSet 		// The set of discovered nodes. currently only start
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


bool Grid::Node::operator>(const Node &rhs) {
	return ((this->x > rhs.x));
}

bool Grid::Node::operator<(const Node &rhs) {
	return ((this->x < rhs.x));
}