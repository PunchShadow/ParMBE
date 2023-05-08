CC = g++

LIBDIR = /home/sylab/PunchShadow/ParMBE/include
TBBLIBDIR = /usr/include/tbb/
SRCDIR = /home/sylab/PunchShadow/ParMBE/src
LIB = -I$(LIBDIR) -I$(TBBLIBDIR)


CFLAGS = -Wall -Werror
CFLAGS += $(LIB)
CFLAGS += -ltbb
# CFLAGS += -g
CFLAGS += -DNDEBUG
CFLAGS += -O3

SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(SRC:.cpp=.o)


TARGET = minelmbc_main

$(TARGET): $(OBJ) minelmbc_test.o
	$(CC) $(CFLAGS) -o $@ $^ -ltbb

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

minelmbc_test.o: minelmbc_test.cpp
	$(CC) $(CFLAGS) -c minelmbc_test.cpp

# minelmbc.o: $(LIBDIR)/minelmbc.cpp
# 	$(CC) -c $(LIBDIR)/minelmbc.cpp -ltbb

clean:
	rm -f $(TARGET) *.o
	rm -f $(SRCDIR)/*.o