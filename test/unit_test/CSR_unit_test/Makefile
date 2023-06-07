CC = g++

LIBDIR = /home/sylab/PunchShadow/ParMBE/include
TBBLIBDIR = /usr/include/tbb/
SRCDIR = /home/sylab/PunchShadow/ParMBE/src
LIB = -I$(LIBDIR) -I$(TBBLIBDIR)


CFLAGS = -Wall -Werror
CFLAGS += $(LIB)
CFLAGS += -ltbb
#CFLAGS += -g
#CFLAGS += -O0
CFLAGS += -O3

#############################################
CFLAGS += -DNDEBUG
CFLAGS += -DNSTATUS
#############################################


SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(SRC:.cpp=.o)


TARGET = csr_main

$(TARGET): $(OBJ) csr_test.o
	$(CC) $(CFLAGS) -o $@ $^ -ltbb

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

csr_test.o: csr_test.cpp
	$(CC) $(CFLAGS) -c csr_test.cpp

# csr.o: $(LIBDIR)/csr.cpp
# 	$(CC) -c $(LIBDIR)/csr.cpp -ltbb

clean:
	rm -f $(TARGET) *.o
	rm -f $(SRCDIR)/*.o