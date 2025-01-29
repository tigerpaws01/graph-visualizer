run:
	g++ ./src/node.cpp ./src/camera.cpp ./src/drawer.cpp ./src/main.cpp -o main -I ./src/include/ -I ./SDL/include/ -L ./SDL/build/ -l SDL3
