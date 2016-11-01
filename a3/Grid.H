#pragma once

/*
 CMPUT 350 Assignment 3
 Due Wednesday Nov. 2, 22:00
  
 The objective of this assignment is to produce a class that can store a
 map-like structure and perform pathfinding and connectivity operations on it.

 Class Grid represents a rectangular tile map with octile topology, i.e. 8
 compass directions using Euclidean distance. There are blocked, ground, or
 water tiles. Moving objects are not represented in the grid.

 The moving object for pathfinding has a location x,y and size s.  It is
 considered to occupy the tiles from x to x+s in the x-axis, and from y to y+s
 in the y-axis. Thus, an object with size 0 occupies one tile, and object with
 size 1 occupies a 2x2 square, and an object with size 2 occupies a 3x3
 square. You are only required to support objects of sizes 0,1, and 2.

 Moves are permitted if and only if all tiles passed through during the move
 have the same tile type. For instance, if a 3x3 object is on ground tiles, it
 may only move diagonally if the 4x4 region it passes over is composed solely
 of ground tiles.

 Similarly, a 2x2 object on water tiles may move to the east if the 3x2 region
 it passes over is composed entirely of water tiles.

 Example:

   01234

 0 wwggg   A 3x3 object centered in this 5x5 map may move west, south, or
 1 ggggg   southwest, because it will pass only over ground tiles. However,
 2 ggggg   it may NOT move northeast, even though it would wind up on a 3x3
 3 ggggw   patch of ground tiles, because it would clip through some water
 4 ggggw   tiles when moving.

 Note that in the above example, the object's initial location would be (1,1)
 and its size would be 2, meaning it occupies from 1 to 3 on both the x and y
 axises.

 To test your implementation issue make which creates testGrid. See TestGrid.C
 for documentation.

 exampleGrid is our solution that should run on lab machines.


 Submission and Marking
 ----------------------
 
 Submit your solution using astep like so:
  
   astep -c c350 -p assign3 README GridPriv.H GridInclude.H Grid.C
  
 Your submissions will be marked considering their correctness (syntax +
 semantics), documentation, efficiency, and coding style. Add assert
 statements to check important pre/post-conditions, use all warning options
 (-Wall -Wextra). Your code must compile without warning or error messages on
 the lab machines. You may use any C/C++ standard libraries you want, such as
 STL, and Boost. Feel free to use C++11 features that g++ on the lab computers
 support. We will compile your code using -std=c++0x.

 Do not include a main() function in your submitted code. When testing, define
 your main() function in a separate .C file and link with the corresponding .o
 file.

 Also, private class members you might want to add to class Grid must be
 defined in file GridPriv.H, which you will submit together with Grid.C .

 Finally, before submitting all files complete the README file.
 
*/

#include <vector> // used for storing paths

#include "GridInclude.H" // add needed #include lines to this file

class Grid
{
public: 

  // no changes in the public interface!

  // data

  enum Tile { GROUND, WATER, BLOCKED };
    
  enum Direction { N, NE, E, SE, S, SW, W, NW };
    
  static const int CARDINAL_COST = 100; // Cost to move in cardinal direction
  static const int DIAGONAL_COST = 142; // Cost to move diagonally
  // (>100*sqrt(2) => air distance is admissible)

  // methods

  Grid(int width, int height);
  ~Grid();
    
  // Map properties
  int getWidth() const;
  int getHeight() const;
  Tile getTile(int x, int y) const;
    
  // Pathfinding operations

  // Return true iff object with a given size can reside on (x1, y1)
  // and move from there to (x2, y2) while staying at the same tile
  // type without colliding. 
  //
  // This should execute faster than calling findShortestPath().
  // 
  // Also, if the map hasn't changed, subsequent calls with the same
  // x1,y1,x2,y2 coordinates SHOULD BE MUCH FASTER. Hint: flood fill + caching
  bool isConnected(int size, int x1, int y1, int x2, int y2) const;

  // Compute a shortest path from (x1, y1) to (x2, y2) for an object with a
  // given size using A*. Store the shortest path into path, and return the
  // cost of that path (using CARDINAL_COST for the moves N, E, S, and W, and
  // DIAGONAL_COST for the moves NE, SE, SW, and NW) or -1 if there is no
  // path. Reduce initialization time by using the generation counter trick.
  int findShortestPath(int size, int x1, int y1, int x2, int y2, 
                       std::vector<Direction> &path) const;
    
  // sets tile type at location x y
  void setTile(int x, int y, Tile tile);

private:

  // add private members in file below

#include "GridPriv.H"
};
