#include "ServerSocket.h"
#include "SocketException.h"
#include <string>

int main(int argc, int argv[])
{
   std::cout << "running....\n";
   try{
      // Create the socket
      ServerSocket server(30000);

      while (true){
	 ServerSocket new_sock;
	 server.accept(new_sock);

	 // For multiple threading, you need to create
	 // a new thread here and pass new_sock to it.
	 // The thread will use new_sock to communicate
	 // with the client.
	 try{
	    while (true){
	       std::string data;
	       new_sock >> data;
	       new_sock << data;
	    }
	 }
	 catch(SocketException&){
	 }
      }
   }
   catch (SocketException& e){
      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
   }
  return 0;
}
