CC = gcc
CFLAGS = -Wall -g

q1: server.c client.c frame.c window.c
	$(CC) $(CFLAGS) -o ../bin/server server.c frame.c window.c
	$(CC) $(CFLAGS) -o ../bin/client client.c frame.c

q2: channel.c receiver.c sender.c frame.c window.c
	$(CC) $(CFLAGS) -o ../bin/channel channel.c 
	$(CC) $(CFLAGS) -o ../bin/sender sender.c frame.c window.c
	$(CC) $(CFLAGS) -o ../bin/receiver receiver.c frame.c 

clean:
	rm ../bin/*
