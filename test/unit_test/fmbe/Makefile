# Makefile to test FMBE.hpp


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


TARGET = fmbe_test

$(TARGET): $(OBJ) fmbe_test.o
	$(CC) $(CFLAGS) -o $@ $^ -ltbb

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

fmbe_test.o: fmbe_test.cpp
	$(CC) $(CFLAGS) -c fmbe_test.cpp

clean:
	rm -f $(TARGET) *.o
	rm -f $(SRCDIR)/*.o
