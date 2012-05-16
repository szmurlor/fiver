gridbnd: main.o

main.o: main.cpp
	g++ -o grid_sub -I../../coremesh2/src -I../../meshreader2 -L../../bin -lmeshreader2 -lcoremesh2 main.cpp

clean:
	rm -f *.o grid_sub
