int width;
int height;
int * map;

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
bool canMove(int size, int x1, int y1, Direction dir) {
	switch(dir) {
		case N:
			return canFit(size, x1, y1-1);
			break;
		case S:
			return canFit(size, x1, y1+1);
			break;
		case E:
			return canFit(size, x1+1, y1);
			break;
		case W:
			return canFit(size, x1-1, y1);
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