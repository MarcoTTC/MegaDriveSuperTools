TARGET = MegaDriveSuperPatcher
CC = g++
CFLAGS = -Wall -O2
LIBS = -lm

HEADERS = ArgumentParser.hpp ips.hpp
OBJECTS = main.o ArgumentParser.o ips.o

.PHONY: default all clean

default: $(TARGET)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
