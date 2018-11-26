#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <fstream>

using namespace std;
bool checkParity(string);

int main()
{
   bool correctParity;
   try{
      // Replace "localhost" with the hostname
      // that you're running your server.
      ClientSocket client_data_socket("localhost", 30000);
      ClientSocket client_ack_socket("localhost",29999);

      string reply;
      string reply_ack;

      
      // Usually in real applications, the following
      // will be put into a loop.
      for(int i=0;i<11;i++){
         try {
          client_data_socket >> reply;
          cout<<reply<<endl;
          //get last char for parity check
          
          //sumof the chars of the message
          /*for(int x = 0; x < str.length(); x++) {
              sumOf += str[x];
              std::bitset<32> temp(str[x]);

              //prints out each character and it's binary value
              cout << str[x];
              cout << " to bitset is ";
              cout << temp;
              cout << "\n";
              
          }*/
          /*
          // bit representation of the sum
          std::bitset<32> sumOfInBits(sumOf);
          int countOneValueBit = 0;
          for(int x =0; x < sumOfInBits.size(); x++){
              if(sumOfInBits[x] == 1){
                countOneValueBit++;
              }
          }*/
          //check if the calculated parity is the same as that sent
          correctParity=checkParity(reply);
          //check parity here
          //cout << "We received this response from the server:\n\"" << reply << "\"\n";
          cout <<correctParity;
          cout << "\n";

          if (correctParity){
            cout<<"Sending ACK\n";
            client_ack_socket << "ACK";
          }
          else {
            cout<<"Sending NAK\n";
            client_ack_socket <<"NAK";
          }
        }
         catch(SocketException&){
         }


      }
}

   catch(SocketException& e){
      cout << "Exception was caught:" << e.description() << "\n";
   }
   return 0;
}

bool checkParity(string frame)
{
   int pcheck=0;
   for(int i=0;i<(frame.size()-1);i++)
   {
      pcheck+=frame[i];
   }
   if(pcheck%2==1&&frame[frame.size()-1]=='1'){return true;}
   else if(pcheck%2==0&&frame[frame.size()-1]=='0'){return true;}
   else{return false;}
}