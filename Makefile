# Makefile for the socket programming tutorial
#

Server = ServerSocket.o Socket.o Server.o ServerSocket.h Socket.h SocketException.h
Client = ClientSocket.o Socket.o Client.o ServerSocket.h Socket.h SocketException.h


all : Server Client

Server: $(Server)
	g++ -o Server $(Server)


Client: $(Client)
	g++ -o Client $(Client)


Socket: Socket.cpp
ServerSocket: ServerSocket.cpp
ClientSocket: ClientSocket.cpp
Server: Server.cpp
Client: Client.cpp


clean:
	rm -f *.o simple_server simple_client
