CC = gcc
CFLAGS = -O2 -Wall
LFLAGS = 

all: servo servo0 servoNeg90 servo90

servo: servo0.o
	$(CC) $(LFLAGS) -o servo0 servo0.o

servo.o: servo0.c
	$(CC) $(CFLAGS) -c servo0.c

servo: servoNeg90.o
	$(CC) $(LFLAGS) -o servoNeg90 servoNeg90.o

servo.o: servoNeg90.c
	$(CC) $(CFLAGS) -c servoNeg90.c

servo: servo90.o
	$(CC) $(LFLAGS) -o servo90 servo90.o

servo.o: servo90.c
	$(CC) $(CFLAGS) -c servo90.c

servo: servo.o
	$(CC) $(LFLAGS) -o servo servo.o

servo.o: servo.c
	$(CC) $(CFLAGS) -c servo.c

clean:
	rm -f *~ *.o servo
