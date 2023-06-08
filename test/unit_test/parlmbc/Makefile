CC = g++

LIBDIR = /home/sylab/PunchShadow/ParMBE/include
TBBLIBDIR = /usr/include/tbb/
SRCDIR = /home/sylab/PunchShadow/ParMBE/src
LIB = -I$(LIBDIR) -I$(TBBLIBDIR)


CFLAGS = -Wall -Werror
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


TARGET = parlmbc_main

$(TARGET): $(OBJ) parlmbc_test.o
	$(CC) $(CFLAGS) -o $@ $^ -ltbb

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

minelmbc_test.o: parlmbc_test.cpp
	$(CC) $(CFLAGS) -c parlmbc_test.cpp

# minelmbc.o: $(LIBDIR)/minelmbc.cpp
# 	$(CC) -c $(LIBDIR)/minelmbc.cpp -ltbb

clean:
	rm -f $(TARGET) *.o
	rm -f $(SRCDIR)/*.o