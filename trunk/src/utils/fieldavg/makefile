TARGET=fieldavg
TARGET_PATH=/usr/local/bin
TARGET_MAN=/usr/local/man/man1/
CXXF=-I../../coremesh2/src -I../../meshreader2

all: main.o 
	g++ -o $(TARGET) main.o -L../../bin -lmeshreader2 -lcoremesh2 -ltrivlog

main.o: main.cpp
	g++ -c main.cpp -I. $(CXXF)

install: all #man
	/usr/bin/install $(TARGET) $(TARGET_PATH)
	ln -f -s $(TARGET_PATH)/$(TARGET) $(TARGET_PATH)/fenergy
	#/usr/bin/install $(TARGET).1 $(TARGET_MAN)

clean:
	rm -f *.o
	rm -f $(TARGET)
	rm -f $(TARGET).1

for_jstar:
	./fieldavg -f /home/gaus/jstar/tmp/roboczy/.SIMULATION.field 3 38 39 40 -gn /home/gaus/jstar/tmp/roboczy/.SIMULATION.grid -avg
