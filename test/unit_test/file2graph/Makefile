CC = g++

LIB = -I../../../src -I/usr/include/tbb/


CFLAGS = -Wall -Werror
CFLAGS += $(LIB)
#CFLAGS += $(LD)
CFLAGS += -ltbb

TARGET = file2graph

$(TARGET): file2graph.o
	$(CC) -o $(TARGET) file2graph.o -ltbb

file2graph.o: file2graph.cpp
	$(CC) $(CFLAGS) -c file2graph.cpp

clean:
	rm -f $(TARGET) *.o