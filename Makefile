LIBS = -lsfml-graphics -lsfml-window -lsfml-system


exec: raycaster
	./raycaster

raycaster: raycaster.o
	g++ raycaster.o -o raycaster $(LIBS)

raycaster.o: raycaster.cpp
	g++ -c raycaster.cpp

