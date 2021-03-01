SH = bash
CFLAGS = -g
LDFLAGS = -lm
CC = gcc

all: server client

server: server.o
	$(CC) $(LDFLAGS) $^ -o $@
client: client.o
	$(CC) $(LDFLAGS) $^ -o $@
server.o: server.c header.h
	$(CC) -c $(CFLAGS) $< -o $@
client.o: client.c header.h
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf *.o
	rm -rf server
	rm -rf client

#ma0882_PA3
