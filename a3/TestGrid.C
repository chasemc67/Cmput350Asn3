/*

  OpenGL program for testing the Grid class

  use middle mouse to toggle unit size

  left click on location, move pointer while button pressed
  finds path

  right click shows connected component

  key press => exit

*/

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <memory>
#include <GL/freeglut.h>
#include "Grid.H"

using namespace std;

using GridPtr = std::unique_ptr<Grid>;

GridPtr	mymap;
int     windowWidth(640);
int     windowHeight(640);
bool    mouseLeft,mouseRight;
int     mouseX0,mouseY0;
int     mouseX1,mouseY1;
int     objectSize;

void    LoadMap(const char *filename);
void    OnDisplay();
void    OnMouse(int button, int state, int x, int y);
void    OnMotion(int x, int y);

// key pressed => exit
void keyfunc(unsigned char /*k*/, int /*x*/, int /*y*/)
{
  cout << "key pressed => exit" << endl;
  glutLeaveMainLoop();
}

int main(int argc, char *argv[])
{
  LoadMap("map.bmp");

  if (!mymap.get()) {
    return -1;
  }
    
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(windowWidth,windowHeight);
  glutCreateWindow("CMPUT 350 - Assignment Example App");
    
  glutDisplayFunc(OnDisplay);
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION); // exit main loop
  glutKeyboardFunc(keyfunc);
  glutMouseFunc(OnMouse);
  glutMotionFunc(OnMotion);
  glutPassiveMotionFunc(OnMotion);
  glutMainLoop();

  cout << "end program" << endl;
  return 0;
}

static int MouseToCoordX(int x)
{
  return std::max(0, std::min(x * mymap->getWidth() / windowWidth,
                              mymap->getWidth()-objectSize-1));
}

static int MouseToCoordY(int y)
{
  return std::max(0, std::min(y * mymap->getHeight() / windowHeight,
                              mymap->getHeight()-objectSize-1));
}

void OnMouse(int button, int state, int x, int y)
{
  const bool pressed(state == GLUT_DOWN);
  
  switch(button) {
    case GLUT_LEFT_BUTTON:   mouseLeft  = pressed; break;
    case GLUT_RIGHT_BUTTON:  mouseRight = pressed; break;
    case GLUT_MIDDLE_BUTTON:
      if (pressed) {
        objectSize = (objectSize+1)%3;
      }
      break;
  }
  mouseX0 = mouseX1 = MouseToCoordX(x);
  mouseY0 = mouseY1 = MouseToCoordY(y);
  glutPostRedisplay();
}

void OnMotion(int x, int y)
{
  mouseX1 = MouseToCoordX(x);
  mouseY1 = MouseToCoordY(y);
  glutPostRedisplay();
}

void DrawPixel(int x, int y, int border, const float *color)
{
  const float xPixel(1.0f / windowWidth);
  const float yPixel(1.0f / windowHeight);
  const float xScale(1.0f / mymap->getWidth());
  const float yScale(1.0f / mymap->getHeight());
  const float x0(x*xScale + xPixel*border);
  const float x1((x+1)*xScale - xPixel*(border+1));
  const float y0(y*yScale + yPixel*border);
  const float y1((y+1)*yScale - yPixel*(border+1));
  
  glColor4fv(color);
  glVertex2f(x0,y0);
  glVertex2f(x1,y0);
  glVertex2f(x1,y1);
  glVertex2f(x0,y1);
}

void DrawBlock(int x, int y, int border, int size, const float *color)
{
  for (int i(0); i <= size; ++i) {
    for (int j(0); j <= size; ++j) {
      DrawPixel(x+j, y+i, border, color);
    }
  }
}

void OnDisplay()
{
  using Path = std::vector<Grid::Direction>;
	
  Path path;

  if (mouseLeft && 
      mymap->isConnected(objectSize, mouseX0, mouseY0, mouseX1, mouseY1)) {
    mymap->findShortestPath(objectSize, mouseX0, mouseY0, mouseX1, mouseY1, path);
  }
  
  static const float colors[][3] = { 
    { 0, 0.5f, 0 }, { 0, 0, 0.5f }, { 0.25f, 0.25f, 0.25f }
  };
  static const float white [3] = { 1, 1, 1 };
  static const float yellow[3] = { 1, 1, 0 };
  static const float red   [3] = { 1, 0, 0 };
    
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glOrtho(0,1,1,0,-1,1);
    
  glClear(GL_COLOR_BUFFER_BIT);

  glBegin(GL_QUADS);

  for (int y(0); y < mymap->getHeight(); ++y) {
    for (int x(0); x < mymap->getWidth(); ++x) {
      DrawPixel(x, y, 0, colors[mymap->getTile(x, y)]);
    }
  }
  
  if (mouseLeft) {
    
    int px(mouseX0), py(mouseY0);

    for (const auto &p : path) {
      switch (p) {
        case Grid::N:        --py; break;
        case Grid::NE: ++px; --py; break;
        case Grid::E:  ++px;       break;
        case Grid::SE: ++px; ++py; break;
        case Grid::S:        ++py; break;
        case Grid::SW: --px; ++py; break;
        case Grid::W:  --px;       break;
        case Grid::NW: --px; --py; break;
      }
      DrawBlock(px, py, 1, objectSize, yellow);
    }

    const float *color(path.empty() ? red : white);

    DrawBlock(mouseX0, mouseY0, 1, objectSize, color);
    DrawBlock(mouseX1, mouseY1, 1, objectSize, color);

  } else if (mouseRight) {
    
    for (int y(0); y+objectSize < mymap->getHeight(); ++y) {
      for (int x(0); x+objectSize < mymap->getWidth(); ++x) {
        if (mymap->isConnected(objectSize, mouseX1, mouseY1, x, y)) {
          DrawBlock(x, y, 1, objectSize, yellow);
        }
      }
    }

    DrawBlock(mouseX1, mouseY1, 1, objectSize, white);

  } else {

    DrawBlock(mouseX1, mouseY1, 2, objectSize, white);
  }

  glEnd();
    
  glPopMatrix();
  glPopAttrib();
    
  glutSwapBuffers();
}

// Load an uncompressed 24-bit .bmp file into a grid
void LoadMap(const char * filename)
{
  // Read the map file
  std::ifstream in(filename,std::ios::binary);

  if (!in) {
    std::cout << "Cannot read " << filename << std::endl;
    return;
  }
  
  // Read the map dimensions
  int width, height;
  assert(sizeof(width) == 4);
  in.seekg(18);
  in.read(reinterpret_cast<char*>(&width), 4); // beware byte ordering!
  in.read(reinterpret_cast<char*>(&height), 4);
  mymap = GridPtr(new Grid(width, height));

  cout << "width=" << width << " height=" << height << endl;
    
  // Initialise the map
  in.seekg(54);
  
  for (int y(0); y < height; ++y) {
    for (int x(0); x < width; ++x) {
      // Load the image data
      unsigned char r, g, b;

      in.read(reinterpret_cast<char*>(&b), 1);
      in.read(reinterpret_cast<char*>(&g), 1);
      in.read(reinterpret_cast<char*>(&r), 1);
        
      // Store the tile data
      Grid::Tile tile(Grid::BLOCKED);
      if (g > 96) {
        tile = Grid::GROUND;
      }
      if (b > 96) {
        tile = Grid::WATER;
      }
      mymap->setTile(x, height-y-1, tile);
    }
  }
}
