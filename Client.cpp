#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <fstream>

using namespace std;
typedef bitset<529> block;//512 for data, 1 for parity, 16 for seq #

vector<block>* framer();

int main()
{
   try{
      // Replace "localhost" with the hostname
      // that you're running your server.
      ClientSocket client_socket("localhost", 30000);
      string reply;
      // Usually in real applications, the following
      // will be put into a loop.
      for(int i=0;i<5;i++){
         try {
	     client_socket << "Test message.";
	     client_socket >> reply;
         }
         catch(SocketException&){
         }
         cout << "We received this response from the server:\n\"" << reply << "\"\n";;
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