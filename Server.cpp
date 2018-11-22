#include "ServerSocket.h"
#include "SocketException.h"
#include <bitset>
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
      //data to send with parity bit
     	std::string data;
      //current_index is the index of the frame to send
      int current_index = -1;
      //stop errors if there has already received a NAK
      bool createdError = false;
      //parity bit for the current frame
      int countForEvenParityBit = 0;
      //data string
      std::string str = "Hello";
      //The ack from the cilent
      std::string ack = "ACK";
	    while (true){
          if(ack == "ACK"){
            current_index++;
            //TODO str = frame_data[current_index]

            int sumOf = 0;
            //create sum of 64ish characters
            for(int x = 0; x < str.length(); x++) {
                sumOf += str[x];
            }
            //bit represent to count 1's for even parity
            std::bitset<32> sumOfInBits(sumOf);
            countForEvenParityBit = 0;
            for(int x =0; x < sumOfInBits.size() -1; x++){
                if(sumOfInBits[x] == 1){
                  countForEvenParityBit++;
                }
            }
          }
          data = str;
          // for creating the error every 5 but not repeating the error if
          // one was already sent
          int conditionParity = 0;
          if(current_index == 4 && !createdError){
            if(conditionParity == 0){
              conditionParity = 1;
            }else{
              conditionParity = 0;
            }
            createdError = true;
          }else{
            createdError = false;
          }
          //adding the parity bit
          if(countForEvenParityBit%2 == conditionParity){
            //check bit of zero creates a NULL char this is why it is a string for now
            char addChar = '0';
            data += addChar;
          } else {
            char addChar = '1';
            data += addChar;
          }
          // send data
	       data_sock << data;
         // get ack
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
