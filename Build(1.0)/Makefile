CXX=g++
CFAGS=-W -Wall
TARGET=pushbox
LIBS=-lncurses
OBJECTS=game.o map.o

all:$(TARGET)

$(TARGET):$(OBJECTS)
	$(CXX) $(CFAGS) -o $@ $^ $(LIBS)

clean:
	rm *.o $(TARGET)