CXXF=-I../../coremesh2/src -I../../meshreader2
all: main.o
	g++ -g -o fieldinfo main.o -lcoremesh2 -lmeshreader2 -ltrivlog -L../../bin -L../../meshreader2

main.o: main.cpp
	g++ -g -c main.cpp -I. $(CXXF)

install: all
	/usr/bin/install fieldinfo /usr/local/bin
	ln -s -f /usr/local/bin/fieldinfo /usr/local/bin/finfo

clean:
	rm -f *.o
	rm -f fieldinfo

es_neck: all
	./fieldinfo -gn ~/src/nerw/es/.neck_2.grid -f ~/src/nerw/es/.neck_2.field 3 2 3 4 -mmax 1 100

SW=/home/poy/sawickib/tmp/AmiSzyja

mg_neck: all
	./fieldinfo -gn $(SW)/dobra.grid -f $(SW)/.J.field 3 1 2 3 -mmax 3 100
