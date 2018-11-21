#include "ServerSocket.h"
#include "SocketException.h"
#include <string>

int main()
{
   std::cout << "running....\n";
   try{
      // Create the socket
      ServerSocket server_data(30000);
      ServerSocket server_ack(29999);

      while (true){
	       ServerSocket data_sock;
         ServerSocket ack_sock;

	       server_data.accept(data_sock);
         server_ack.accept(ack_sock);

	 // For multiple threading, you need to create
	 // a new thread here and pass new_sock to it.
	 // The thread will use new_sock to communicate
	 // with the client.
	 try{

     	std::string data;
      int current_index = 0;
      //frame = frame::getFrame(block, 2)
      std::string ack = "True";
	    while (true){
          if(ack == "True"){
            std::cout<<ack;
            data += "x";
            //data_to_send = current_frame.generateParityBit();
          }
	       data_sock << data;
         ack_sock >> ack;
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
