CC= gcc



all: aesdsocket

aesdsocket: aesdsocket.o
	$(CC) -o aesdsocket aesdsocket.o -lm
aesdsocket.o: aesdsocket.c
	$(CC) -Wall -g -c aesdsocket.c
clean:
	rm -rf aesdsocket *.o
