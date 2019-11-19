default: server.out client.out

client.out: Point.c StructClient.c DataMessage.c real_client.c
	gcc Point.c StructClient.c DataMessage.c real_client.c -o client.out -lm

server.out:  DataMessage.c StructClient.c  Server.c
	gcc DataMessage.c StructClient.c Server.c -o server.out -lm