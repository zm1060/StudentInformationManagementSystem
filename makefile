# change application name here (executable output name)
TARGET=sms

# compiler
CC=gcc
# debug
DEBUG=-g
# optimisation
OPT=-O0
# warnings
WARN=-Wall

PTHREAD=-pthread

CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe

GTKLIB=`pkg-config --cflags --libs gtk+-3.0`

# linker
LD=gcc
LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic -l sqlite3

OBJS=   widget.o  \
       		main.o

all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)
    
widget.o: src/widget.c src/widget.h
	$(CC) -c $(CCFLAGS) src/widget.c $(GTKLIB) -o widget.o -l sqlite3
main.o: src/main.c
	$(CC) -c $(CCFLAGS) src/main.c $(GTKLIB) -o main.o -l sqlite3
    
clean:
	rm -f *.o $(TARGET)
