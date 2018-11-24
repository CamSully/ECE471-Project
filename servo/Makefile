CC = gcc
CFLAGS = -O2 -Wall
LFLAGS = 

all: servo

servo: servo.o
	$(CC) $(LFLAGS) -o servo servo.o

servo.o: servo.c
	$(CC) $(CFLAGS) -c servo.c

clean:
	rm -f *~ *.o servo
