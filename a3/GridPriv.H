int width;
int height;
int * map;

// used to cache floodfill for connected part
bool * floodMap;

// Check if unit can fit at coord
bool canFit(int size, int x1, int y1) const {
	// Check for out of bounds
	if (x1 < 0 || y1 < 0 || x1 >= width || y1 >= height)
		return false;

	bool canFit = true;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (getTile(x1+i, y1+i) != getTile(x1, y1) || getTile(x1+i, y1+i) == BLOCKED){
				canFit = false;
			}
		}
	}
	if (!canFit) 
		return false;
	return true;
};

// Check if unit can move 1 square in direction
// If moving diagonally we can recursively check the 2 non-diag dirs
// When we check non-diag dirs, we also check for tile type
bool canMove(int size, int x1, int y1, Direction dir) const {
	switch(dir) {
		case N:
			return canFit(size, x1, y1-1) && tilesAreSame(size, x1, y1, x1, y1-1);
			break;
		case S:
			return canFit(size, x1, y1+1) && tilesAreSame(size, x1, y1, x1, y1+1);;
			break;
		case E:
			return canFit(size, x1+1, y1) && tilesAreSame(size, x1, y1, x1+1, y1);;
			break;
		case W:
			return canFit(size, x1-1, y1) && tilesAreSame(size, x1, y1, x1-1, y1);;
			break;
		case NE:
			return (canMove(size, x1, y1, N) && canMove(size, x1, y1, E));
			break;
		case NW:
			return (canMove(size, x1, y1, N) && canMove(size, x1, y1, W));
			break;
		case SE:
			return (canMove(size, x1, y1, S) && canMove(size, x1, y1, E));
			break;
		case SW:
			return (canMove(size, x1, y1, S) && canMove(size, x1, y1, W));
			break;
		default:
			return false;
			break;
	}
	return false;
};

// Check if the tiles are all the same at 
// points covered by unit at x1,y1 and x2,y2
bool tilesAreSame(int size, int x1, int y1, int x2, int y2) const {
	return (canFit(size, x1, y1) && canFit(size, x2, y2) && getTile(x1, y1) == getTile(x2, y2));
};

// Check flood cache with bounds checking
bool getFloodMap(int x, int y) const {
	if (x < 0 || y < 0 || x >= width || y >= height)
		return false;
	return floodMap[x + y*width];
};

void setReachable(int x, int y) const {
	floodMap[x + y*width] = true;
};

// Recursively set all adjacent points that can be moved to
// as moveable.
// Checks if node has already been expanded before expanding
// To stop infinite flooding
void flood(int size, int x, int y) const {
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