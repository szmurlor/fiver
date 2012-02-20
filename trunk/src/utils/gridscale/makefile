CXXF=-I../../coremesh2/src -I../../meshreader2
all: main.o
	g++ -g -o gridscale main.o -lcoremesh2 -lmeshreader2 -L../../coremesh2/lib -L../../meshreader2

main.o: main.cpp
	g++ -g -c main.cpp -I. $(CXXF)

install: all
	/usr/bin/install gridscale /usr/local/bin
	ln -s -f /usr/local/bin/gridscale /usr/local/bin/gridscale

clean:
	rm -f *.o
	rm -f gridscale

es_neck: all
	./gridscale -gn ~/src/nerw/es/.neck_2.grid -f ~/src/nerw/es/.neck_2.field 3 2 3 4 -mmax 1 100

SW=/home/poy/sawickib/tmp/AmiSzyja

mg_neck: all
	./fieldinfo -gn $(SW)/dobra.grid -f $(SW)/.J.field 3 1 2 3 -mmax 3 100
