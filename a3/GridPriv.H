// list private Grid members here

// ...

int width;
int height;

int * map;

bool canFit(int size, int x1, int y1) {
	bool canFit = true;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (getTile(x1+i, y1+i) != getTile(x1, y1) || getTile(x1+i, y1+i) == blocked){
				canFit = false;
			}
		}
	}
	if (!canFit) 
		return false;
	return true;
}