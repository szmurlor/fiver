gridbnd: main.o

main.o: main.cpp
	g++ -o grid_sub -I../../coremesh2/src -L../../coremesh2/lib -L../../meshreader2/ -I../../meshreader2 -lmeshreader2 -lcoremesh2 main.cpp

clean:
	rm -f *.o grid_sub
