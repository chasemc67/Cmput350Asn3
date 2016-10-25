CC=g++ -O3 -Wall -Wextra -std=c++11

#CC=g++ -std=c++11 -O3 -Wall -Wextra -fsanitize=undefined -fno-sanitize=object-size -fsanitize=address -fsanitize=leak -fsanitize=signed-integer-overflow -fsanitize=bounds-strict

all: testGrid

test: testGrid
	./testGrid

clean:
	rm -f ./testGrid

testGrid: TestGrid.C Grid.C Grid.H
	$(CC) TestGrid.C Grid.C -lglut -lGLEW -lGL -lGLU -lX11 -L/usr/lib/nvidia-340 -Wno-write-strings -o ./testGrid

testg: Test.C Grid.C Grid.H
	$(CC) Test.C Grid.C -lglut -lGLEW -lGL -lGLU -lX11 -L/usr/lib/nvidia-340 -Wno-write-strings -o ./testg

tar:
	rm -f *~ a2.tar
	tar cf a2.tar *

submit:
	astep -c c350 -p assign2 README Grid.C Grid.H
