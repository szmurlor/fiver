TARGET=vol2surf
TARGET_PATH=/usr/local/bin
OBJECTS=main.o 
INCPATH=-I. -I../../coremesh2/src -I../../meshreader2 
INSTALL=/usr/bin/install

$(TARGET): $(OBJECTS)
	g++ -o $(TARGET) $(OBJECTS) -L../../coremesh2/lib -L../../meshreader2/lib -lmeshreader2 -lcoremesh2

$(OBJECTS):%.o:%.cpp
	g++ -c $(INCPATH) $<

install: $(TARGET)
	$(INSTALL) $(TARGET) $(TARGET_PATH)

clean:
	rm -f $(TARGET)
	rm -f *.o

test: $(TARGET)
	./$(TARGET) ../grids/cube.grid cube_3.grid -s 10 0.5 0.5 0 0.15 1 0 -l 10 2 11 0.3

