CC = gcc
CFLAGS = -Wall -g

hw3: hw3.c
	$(CC) $(CFLAGS) -o hw3 hw3.c

clean:
	rm -f hw3
	