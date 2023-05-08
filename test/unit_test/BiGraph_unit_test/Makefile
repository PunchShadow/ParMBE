# Makefile to test BiGraph.hpp


CC = g++

LIBDIR = /home/sylab/PunchShadow/ParMBE/include
TBBLIBDIR = /usr/include/tbb/
SRCDIR = /home/sylab/PunchShadow/ParMBE/src
LIB = -I$(LIBDIR) -I$(TBBLIBDIR)


CFLAGS = -Wall -Werror
CFLAGS += $(LIB)
CFLAGS += -ltbb
CFLAGS += -g
CFLAGS += -DDEBUG

SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(SRC:.cpp=.o)


TARGET = BiGraph_test

$(TARGET): $(OBJ) BiGraph_unit_test.o
	$(CC) $(CFLAGS) -o $@ $^ -ltbb

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

VertexSet_test.o: BiGraph_unit_test.cpp
	$(CC) $(CFLAGS) -c BiGraph_unit_test.cpp

clean:
	rm -f $(TARGET) *.o
	rm -f $(SRCDIR)/*.o
