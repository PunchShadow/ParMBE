CC = g++

LIBDIR = ../../include
TBBLIBDIR = /usr/include/tbb/
SRCDIR = ../../src
LIB = -I$(LIBDIR) -I$(TBBLIBDIR)


CFLAGS = -Wall -std=c++17
CFLAGS += $(LIB)
CFLAGS += -ltbb
# CFLAGS += -g
# CFLAGS += -O0
# CFLAGS += -fsanitize=thread -fPIE -pie -g -fno-omit-frame-pointer
CFLAGS += -O3

#############################################
CFLAGS += -DNDEBUG
CFLAGS += -DSTATUS
#############################################


SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(SRC:.cpp=.o)


TARGET = parmbe_main

$(TARGET): $(OBJ) parmbe_test.o
	$(CC) $(CFLAGS) -o $@ $^ -ltbb

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

minelmbc_test.o: parmbe_test.cpp
	$(CC) $(CFLAGS) -c parmbe_test.cpp

# minelmbc.o: $(LIBDIR)/minelmbc.cpp
# 	$(CC) -c $(LIBDIR)/minelmbc.cpp -ltbb

clean:
	rm -f $(TARGET) *.o
	rm -f $(SRCDIR)/*.o