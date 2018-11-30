#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <fstream>

using namespace std;
const string flagstr="0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0";
bool checkParity(string);

int main()
{
   int j=0;
   vector<string> fullmsg;
   string content = "";
   bool correctParity,flag=true;
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
      for(int i=0;i<100000;i++){
         try {

           if(i == 0){
          client_data_socket >> reply;
          cout<<reply<<endl;
          string request;
          cin>>request;
          client_data_socket << request;
          client_data_socket >> reply;
          //cout<<reply<<endl;
        }
          else{
            client_data_socket >> reply;
            //cout<<reply<<endl;
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
            //this is just formatting the lines
            if(reply==flagstr){

               j=(static_cast<int>(content.size())-1);
               while(flag)
               {
                  if(content[j]==' ')
                  {
                     j--;
                  }
                  else
                  {
                     flag=false;
                     content=content.substr(0,j+1);
                  }
               }
               flag=true;
               fullmsg.push_back(content);
               content="";
            }
            else{
               content=content+reply.substr(0,64);
               //cout<<content<<endl;
            }
          }
          else {
            cout<<"Sending NAK on "<<i<<"th frame received:"<<reply <<endl;
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
   for(int j=0;j<fullmsg.size();j++)
   {
      cout<<fullmsg[j]<<" //Line Number: "<<j+1<<"\n\n";
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
