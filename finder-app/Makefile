CC= aarch64-none-linux-gnu-gcc



all: writer

writer: writer.o
	$(CC) -o writer writer.o -lm
writer.o: writer.c
	$(CC) -Wall -g -c writer.c
clean:
	rm -rf writer *.o
