#include "Grid.H"
#include <iostream>

// Shared pointers
#include <memory>
// Formated strings
#include "boost/format.hpp"
// infinity for a* alg
#include <limits>
// square numbers
#include <cmath>

#include <map>
// #include <unordered_map>
	
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

/*
bool value_comparer(M::value_type &i1, M::value_type &i2)
{
		return i1.second<i2.second;
} */

int Grid::findShortestPath(int size, int x1, int y1, int x2, int y2, 
                       std::vector<Direction> &path) const {

	
	std::shared_ptr<Node> startNode(new Node(x1, y1));
	std::shared_ptr<Node> endNode(new Node(x2, y2));

	// cost nothing to get from start node to this node
	startNode->gScore = 0;
	// cost to get from start node to goal is entirely heuristic
	startNode->fScore = startNode->getHeuristicDistance(*endNode);

	// Set of nodes already evaluated
	std::map<std::pair<int, int>, std::shared_ptr<Node>> closedSet;
	// Set of discovered nodes to be evaluated
	// initially contains only the start node
	// Open set must be sorted, with lowest fScore at the top.
	// F is sorted by fScore, n is sorted by node
	std::multimap<int, std::shared_ptr<Node>> openSetF;
	// std::unordered_map<std::pair<int, int>, std::shared_ptr<Node>> openSetN;

	std::cout << std::endl;
	std::cout << boost::format("Finding shortest path from (%d, %d) to (%d, %d)\n") % startNode->x % startNode->y % endNode->x % endNode->y;
	std::cout << std::endl;

	std::shared_ptr<Node> t1(new Node(5, 5));
	t1->fScore = 100;
	std::shared_ptr<Node> t2(new Node(10, 10));
	t2->fScore = 120;
	std::shared_ptr<Node> t3(new Node(12, 12));
	t3->fScore = 80;
	std::shared_ptr<Node> t4(new Node(11, 11));
	t4->fScore = 110;
	std::shared_ptr<Node> t5(new Node(8, 8));
	t5->fScore = 50;
	
	openSetF.insert(std::pair<int, std::shared_ptr<Node>>(t1->fScore, t1));
	openSetF.insert(std::pair<int, std::shared_ptr<Node>>(t2->fScore, t2));
	openSetF.insert(std::pair<int, std::shared_ptr<Node>>(t3->fScore, t3));
	openSetF.insert(std::pair<int, std::shared_ptr<Node>>(t4->fScore, t4));
	openSetF.insert(std::pair<int, std::shared_ptr<Node>>(t5->fScore, t5));


	std::multimap<int, std::shared_ptr<Node>>::iterator it = openSetF.begin();
	auto it_closed = closedSet.begin();

	std::cout << "Testing map" << std::endl; 
	while (it != openSetF.end()) {
		std::cout << boost::format("%d, %d, f:%d\n") % it->second->x % it->second->y % it->second->fScore;
		it++;
	}

	/*
	std::cout<< "Testing itor" << std::endl;
	openSetF[t5->fScore] = t5;
	std::cout << boost::format("min is: %d, %d, f:%d\n") % openSetF.begin()->second->x % openSet.begin()->second->y % openSet.begin()->second->fScore;
	it = openSetF.begin();
	while (it != openSetF.end()) {
		std::cout << boost::format("%d, %d, f:%d\n") % it->second->x % it->second->y % it->second->fScore;
		it++;
	}

	// openSet.add(startNode)
	openSetF[startNode->fScore] = startNode;
	std::shared_ptr<Node> current;
	while(!openSetF.empty()) { 

		std::cout << std::endl;
		std::cout << "open set: " << std::endl;
		it = openSetF.begin();
		while (it != openSetF.end()) {
			std::cout << boost::format("%d, %d, f:%d\n") % it->second->x % it->second->y % it->second->fScore;
			it++;
		}

		std::cout << "closed set: " << std::endl;
		it_closed = closedSet.begin();
		while (it_closed != closedSet.end()) {
			std::cout << boost::format("%d, %d, f:%d\n") % it_closed->second->x % it_closed->second->y % it_closed->second->fScore;
			it_closed++;
		}

		current = openSetF.begin()->second;
		
		if (*current == *endNode) {
			// return reconstruct_path(cameFrom, current);
			std::cout << "Found path" << std::endl;
			return 1;
		}

		openSetF.erase(current.fScore);
		closedSet[std::make_pair(current.x, current.y)] = current;

		/*

		for (neighbor : all 8 reachable neighbors for current) {
			if (closedSet.contains(neighbor))
				continue;

			tentative_gScore = current.gScore() + distToNeighbor(neighbor)

			if (!openSet.contains(neighbor))
				openSet.add(neighbor)
			else if (tentative_gScore >= neighbor.gScore)
				continue

			neighbor.cameFrom = current;
			neighbor.gScore = tentative_gScore;
			neighbor.fScore = neighbor.gScore + neighbor.getHeuristicDistance(*endNode)
		}

		*/

	// }
	return 1;
}

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

int Grid::Node::getHeuristicDistance(const Node & to) {
	std::cout << boost::format("heuristic x diff:%d y diff:%d \n") % (this->x - to.x) % (this->y - to.y);
	std::cout << boost::format("%d %d \n") % pow((this->x - to.x), 2) % pow((this->y - to.y),2);
	return (sqrt((pow((this->x - to.x), 2) + pow((this->y - to.y),2))));
}

// Node implementation

Grid::Node::Node(int x_, int y_) {
		x = x_;
		y = y_;
		gScore = std::numeric_limits<int>::max();
		fScore = std::numeric_limits<int>::max();
};

bool Grid::Node::operator>(const Node &rhs) {
	return ((this->fScore > rhs.fScore));
}

bool Grid::Node::operator<(const Node &rhs) {
	return ((this->fScore < rhs.fScore));
}

bool Grid::Node::operator==(const Node &rhs) {
	return ((this->x == rhs.x && this->y == rhs.y));
}