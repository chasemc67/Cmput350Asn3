#include "Grid.H"
#include <iostream>

using namespace std;

int main() {
	Grid * grid = new Grid(500, 250);

	cout << grid->getWidth() << endl;
	cout << grid->getHeight() << endl;

	delete grid;
	return 0;
}