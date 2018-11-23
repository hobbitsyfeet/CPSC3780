#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <fstream>

using namespace std;
typedef bitset<529> block;//512 for data, 1 for parity, 16 for seq #

//TODO class Frame>> extraction fromthis block
struct frame{
   const static int size = 64;
   char message[64];
   bitset<1> parity; //needs to be even parity
   unsigned int seq;

   //
};

vector<block>* framer();
vector<frame>* framer2();


int main()
{
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
          //get last char for parity check
          char lastbyte = reply[reply.length() -1];

          cout << lastbyte;
          cout << '\n';
          //remove last char from message
          string str = reply.substr(0, reply.length()-1);
          int sumOf = 0;
          //sumof the chars of the message
          for(int x = 0; x < str.length(); x++) {
              sumOf += str[x];
              std::bitset<32> temp(str[x]);

              //prints out each character and it's binary value
              /*cout << str[x];
              cout << " to bitset is ";
              cout << temp;
              cout << "\n";
              */
          }
          // bit representation of the sum
          std::bitset<32> sumOfInBits(sumOf);
          int countOneValueBit = 0;
          for(int x =0; x < sumOfInBits.size(); x++){
              if(sumOfInBits[x] == 1){
                countOneValueBit++;
              }
          }
          //check if the calculated parity is the same as that sent
          bool correctParity = countOneValueBit%2 == 0 && lastbyte == '0' || countOneValueBit%2 == 1 && lastbyte == '1';
          //check parity here
          cout << "We received this response from the server:\n\"" << reply << "\"\n";
          cout << "SumOf ";
          cout << sumOf;
          cout << " to bitset is ";
          cout << sumOfInBits;
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

//*******************************************************
// Helper function to take a text file and turn it into
// binary frames with a parity bit and sequence number
//*******************************************************
vector<block>* framer()
{
   //initilize shit
   int i,j,k;
   unsigned short count=0,parity;
   ifstream file("testtext.txt");
   string input;
   block temp;
   bitset<8> btemp;//byte temp
   bitset<16> ctemp;//count temp
   vector<block> frames;
   //extract data from file and break it down into frames
   while(!file.eof())
   {
      //get line of text
      getline(file,input);
      //pad line
      while(input.size()%64!=0)
      {
         input.append(" ");
      }
      //split it into frames
      for(i=0;i<input.size();i+=64)
      {
         //convert characters into binary and copy to frame
         for(j=0;j<64;j++)
         {
            btemp=bitset<8>(static_cast<char>(input[i+j]));
            for(k=0;k<8;k++)
            {
               temp[(8*j)+7-k]=btemp[k];
            }
         }
         parity=0;
         //determine and set parity bit
         for(j=0;j<512;j++)
         {
            if(temp[j]==1)
            {
               parity++;
            }
         }
         if(parity%2==1)
         {
            temp[512]=1;
         }
         else
         {
            temp[512]=0;
         }
         //set the seq #
         ctemp=bitset<16>(count);
         for(j=0;j<16;j++)
         {
            temp[528-j]=ctemp[j];
         }
         //update seq # for next frame
         count++;
         //push the frames onto the vector
         frames.push_back(temp);
      }
   }
   return &frames;
}

vector<frame>* framer2()
{
   int i,j,k,pcounter,scount=0;
   bitset<8> btemp;
   vector<frame> temp;
   frame ftemp;
   ifstream file("testtext.txt");
   string input;
   while(!file.eof())
   {
      getline(file,input);
      while(input.size()%64!=0)
      {
         input.append(" ");
      }
      for(i=0;i<input.size();i+=64)
      {
         pcounter=0;
         for(j=0;j<64;j++)
         {
            ftemp.message[j]=input[i+j];
            btemp=bitset<8>(ftemp.message[j]);
            for(k=0;k<8;k++)
            {
               if(btemp[j]==1){pcounter++;}
            }
         }
         if(pcounter%2==1){ftemp.parity[0]=1;}
         else{ftemp.parity[0]=0;}
         ftemp.seq=scount;
         scount++;
         temp.push_back(ftemp);
      }
   }
   cout<<temp.size()<<"\n\n";
   return &temp;
}
