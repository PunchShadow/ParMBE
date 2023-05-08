# Makefile to auto generate the unit test for the utility library

CC = g++

LIBDIR = /home/sylab/PunchShadow/ParMBE/include
TBBLIBDIR = /usr/include/tbb/
SRCDIR = /home/sylab/PunchShadow/ParMBE/src

LIB = -I$(LIBDIR) -I$(TBBLIBDIR)

CFLAGS = -Wall -Werror

CFLAGS += $(LIB)
CFLAGS += -ltbb

CFLAGS += -g -DDEBUG

SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(SRC:.cpp=.o)

TARGET = utility_main

$(TARGET): $(OBJ) utility_test.o
	$(CC) $(CFLAGS) -o $@ $^ -ltbb

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

utility_test.o: utility_test.cpp
	$(CC) $(CFLAGS) -c utility_test.cpp

clean:
	rm -f $(TARGET) *.o
	rm -f $(SRCDIR)/*.o

# Path: test/unit_test/Makefile
# Compare this snippet from test/unit_test/minelmbc/Makefile:
# CC = g++