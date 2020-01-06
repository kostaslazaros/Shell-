all: shell client server

shell: shell.o
	gcc -o shell shell.o -s

client: client.o
	gcc -o client client.o -s

server: server.o
	gcc -o server server.o -s

shell.o: shell.c
	gcc -c shell.c -o shell.o

client.o: client.c
	gcc -c client.c -o client.o

server.o: server.c
	gcc -c server.c -o server.o

clean:
	rm -f shell client server shell.o client.o server.o