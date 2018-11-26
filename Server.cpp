#include "ServerSocket.h"
#include "SocketException.h"
#include <bitset>
#include <string>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

string readFile();
vector<string> createFrames(string&);



int main()
{
  //reads text from file into LONG string.
  string data_string = readFile();
  //cout<<"Loading:\n"<< data_string;
  vector<string> frames = createFrames(data_string);

  for(int i; i < frames.size(); i++){
    cout<<"Frame "<< i << ": "<< frames[i]<<endl;
  }

   cout << "running....\n";
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
     	string data;
      //current_index is the index of the frame to send
      int current_index = -1;
      //stop errors if there has already received a NAK
      bool createdError = false;
      //parity bit for the current frame
      int countForEvenParityBit = 0;
      //data string
      string str = "Hello";
      //The ack from the cilent
      string ack = "ACK";
	    while (true){
          if(ack == "ACK"){
            current_index++;
            //TODO str = frame_data[current_index]
            str = frames[current_index];

            int sumOf = 0;
            //create sum of 64ish characters
            for(int x = 0; x < str.length(); x++) {
                sumOf += str[x];
            }
            //bit represent to count 1's for even parity
            bitset<32> sumOfInBits(sumOf);
            countForEvenParityBit = 0;
            for(int x =0; x < sumOfInBits.size(); x++){
                if(sumOfInBits[x] == 1){
                  countForEvenParityBit++;
                }
            }
          }
          data = str;
          // for creating the error every 5 but not repeating the error if
          // one was already sent
          int conditionParity = 0;
          if(current_index%5 == 0 && !createdError){
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
      cout << "Exception was caught:" << e.description() << "\nExiting.\n";
   }
  return 0;
}

//reads all contents in file to a LONG string
string readFile(){
  string word = "";
  ifstream fin;
  string tempString = "";
  fin.open("testtext.txt");
  if (!fin) {
    cout << "Unable to open file";
    return "";
  }
  while(fin >> word){
    tempString = tempString +" "+ word;
  }
  return tempString;
}

//creates a vector of strings which contain up to 64 chars each
vector<string> createFrames(string& fileString){
  vector<string> frames;
  //Each step is multiplied to the string size to get the next frame EX.frameStart = step * 64
  for(int step = 0; step < fileString.length()/64 +1; step++){

    if(step * 64 > fileString.length()){
      int frameStart = step * 64;
      string substring = fileString.substr(frameStart, fileString.length() - frameStart);
      if(substring.length() == 0){
        break;
      }
      frames.push_back(substring);
    }
    //else we are in bounds with the next step, add 64 chars to the frame vector
    else{
      int frameStart = step * 64;
      string substring = fileString.substr(frameStart, 64);
      if(substring.length() == 0){
        break;
      }
      frames.push_back(substring);
    }
  }
  return frames;
    //if step is larger than the string length, we're at the end so take the rest
}
