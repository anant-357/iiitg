CC=gcc

ff: ff.c utils.c
	$(CC) -o ../bin/ff.run ff.c utils.c

pt: pt.c ff.c utils.c
	$(CC) -o ../bin/pt.run pt.c ff.c utils.c

clean:
	rm ../bin/*.run
