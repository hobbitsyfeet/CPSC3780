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
  string content = "";
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
      //TODO while NOT EOF
      for(int i=0;i<1000000;i++){
         try {

           if(i == 0){
          client_data_socket >> reply;
          cout<<reply<<endl;
          string request;
          cin>>request;
          client_data_socket << request;
          client_data_socket >> reply;
          cout<<reply<<endl;
        }
          else{
            client_data_socket >> reply;
            cout<<reply<<endl;
          }
          if(reply == "__EOF__")
          {break;}
          //check if the calculated parity is the same as that sent
          correctParity=checkParity(reply);
          //check parity here
          //cout << "We received this response from the server:\n\"" << reply << "\"\n";
          //cout <<"="<<correctParity;
          //cout << "\n";

          if (correctParity){
            //cout<<"Sending ACK\n";
            client_ack_socket << "ACK";
            if(content.length() < 64)
              content = content + reply;
            else{content = content + reply + '\n';}
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
   cout<<content;
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
