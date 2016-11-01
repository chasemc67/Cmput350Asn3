// #include <memory>

int width;
int height;
int * map;

// used to cache floodfill for connected part
bool * floodMap;
int * cachedSize;

class Node {
public:
	Node(int x_, int y_);
	~Node(){};

	bool operator>(const Node &rhs);
	bool operator<(const Node &rhs);
	bool operator==(const Node &rhs);
	// Get the euclidian distance from one node to another
	int getHeuristicDistance(const Node & to);

	int x;
	int y;
	Direction cameFrom;
	Node * parent;
	int gScore;
	int fScore;
};

// Check if unit can fit at coord
bool canFit(int size, int x1, int y1) const;

// Check if unit can move 1 square in direction
// If moving diagonally we can recursively check the 2 non-diag dirs
// When we check non-diag dirs, we also check for tile type
bool canMove(int size, int x1, int y1, Direction dir) const;

// Check if the tiles are all the same at 
// points covered by unit at x1,y1 and x2,y2
bool tilesAreSame(int size, int x1, int y1, int x2, int y2) const;

// Check flood cache with bounds checking
bool getFloodMap(int x, int y) const;

void setReachable(int x, int y) const;

// Recursively set all adjacent points that can be moved to
// as moveable.
// Checks if node has already been expanded before expanding
// To stop infinite flooding
void flood(int size, int x, int y) const;

int getXinDir(Direction dir) const;
int getYinDir(Direction dir) const;
int moveDistance(Direction dir) const;
void reconstruct_path(const Node & cameFrom, std::vector<Direction> &path) const;