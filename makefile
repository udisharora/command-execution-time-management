CC = gcc
CFLAGS = -Wall -g

all: time time_shared time_pipe

time: time.c
	$(CC) $(CFLAGS) -o time time.c

time_shared: time_shared.c
	$(CC) $(CFLAGS) -o time_shared time_shared.c

time_pipe: time_pipe.c
	$(CC) $(CFLAGS) -o time_pipe time_pipe.c

clean:
	rm -f time time_shared time_pipe
