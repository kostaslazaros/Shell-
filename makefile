all: shell client server

shell: shell.c shellfunc.c
	gcc -o shell shell.c shellfunc.c

client: client.c
	gcc -o client client.c

server: server.c
	gcc -o server server.c shellfunc.c

clean:
	rm -f shell client server