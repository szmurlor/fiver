TARGET=fieldenergy
TARGET_PATH=/usr/local/bin
TARGET_MAN=/usr/local/man/man1/
CXXF=-I../../coremesh2/src -I../../meshreader2

all: main.o fieldenergy.o
	g++ -o $(TARGET) main.o fieldenergy.o -L/usr/local/lib -L../../bin -lmeshreader2 -lcoremesh2

main.o: main.cpp
	g++ -c main.cpp -I. $(CXXF)

fieldenergy.o: fieldenergy.cpp
	g++ -c fieldenergy.cpp -I. $(CXXF)

test: all
	 ./$(TARGET) /home/poy/sawickib/simres/headreal/mh-0/head.grid /home/poy/sawickib/simres/headreal/mh-0/.head.field /home/poy/sawickib/simres/headreal/mh-0/head.data -f 1 21 -s 2 1 2 > w.txt

installhome: all
	/usr/bin/install $(TARGET) ~/bin

install: all #man
	/usr/bin/install $(TARGET) $(TARGET_PATH)
	ln -f -s $(TARGET_PATH)/$(TARGET) $(TARGET_PATH)/fenergy
	#/usr/bin/install $(TARGET).1 $(TARGET_MAN)

man:
	pod2man -c "Mesh Tools" man.pod $(TARGET).1

uninstall:
	rm -f $(TARGET_PATH)/$(TARGET)
	rm -f $(TARGET_PATH)/fenergy

clean:
	rm -f *.o
	rm -f $(TARGET)
	rm -f $(TARGET).1


es_neck: all
	 ./$(TARGET) ~/src/nerw/es/.neck_2.grid ~/src/nerw/es/.neck_2.field ~/src/nerw/es/neck.dat -f 3 2 3 4 -d -m 1 -I

SW=/home/poy/sawickib/tmp/AmiSzyja
mg_neck: all
	 ./$(TARGET) $(SW)/dobra.grid $(SW)/.J.field $(SW)/siatglo.dat -f 3 1 2 3 -d 
