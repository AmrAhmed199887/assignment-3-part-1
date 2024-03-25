CROSS_COMPILE= aarch64-none-linux-gnu-gcc
all: writer

writer: writer.o
	#$(CROSS_COMPILE) -o writer writer.o -lm
	gcc -o writer writer.o -lm
writer.o: writer.c
	#$(CROSS_COMPILE) -Wall -g -c writer.c
	gcc -Wall -g -c writer.c
clean:
	rm -r writer *.o
