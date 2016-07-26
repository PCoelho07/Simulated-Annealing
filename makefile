
CC    = g++
CFLAGS= -std=c++11 -I. -I/usr/include
LIBS  = -L/usr/lib

OBJ   = main.o Dgraph.o task.o sa.o

%.o: %.cpp
	$(CC) -g -c -o $@ $< $(CFLAGS) -pg

sa_jsp: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) -pg

.PHONY: clean

clean:
	rm -f *.o *~ core
