CC = gcc
CFLAGS = -Wall -g

q1: server.c client.c frame.c window.c
	$(CC) $(CFLAGS) -o ../bin/server server.c frame.c window.c
	$(CC) $(CFLAGS) -o ../bin/client client.c frame.c

q2: server.c client.c frame.c window.c
	$(CC) $(CFLAGS) -o ../bin/server n_server.c frame.c window.c
	$(CC) $(CFLAGS) -o ../bin/client client.c frame.c

clean:
	rm ../bin/*
