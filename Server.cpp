#include "ServerSocket.h"
#include "SocketException.h"
#include <bitset>
#include <string>
#include <fstream>
#include <string>
#include <vector>
#include <pthread.h>
using namespace std;

vector<string> framer();


//TODO function that sends the data from the frame_pointer
void *sendData(void* frame_pointer);

int main()
{
  vector<string> frames = framer();

  vector<string>* frames_ptr = &frames;
  //display information loaded
  for(int i; i < frames.size(); i++){
    cout<<"Frame "<< i << ": "<< frames[i]<<endl;
  }



  //can handle up to 5 threads
  int NTHREADS = 5;
  pthread_t thread_id[NTHREADS];

  //TODO both for thread loops should exist in the loop which access each frame
  // for(frame in frames)
    //create threads
    //join threads
    //print threads
  //NEXT FRAME
  //create the threads
   for(int thread = 0; thread < NTHREADS; thread++){
      //TODO last argument (NULL) needs to be a pointer to vector of frames
      pthread_create( &thread_id[thread], NULL, &sendData ,frames_ptr);
   }
    //join each thread
    for(int thread  = 0; thread < NTHREADS; thread ++){
      //TODO last argument (NULL) needs to be a pointer to vector of frames
      pthread_join( thread_id[thread],NULL);
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
          /*
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
          }   */     
          // send data
	       data_sock << data;
         // get ack
         ack_sock >> ack;
      }//END WHILE
	 }//END OUTER MOST TRY
	 catch(SocketException&){
	 }
 }//END WHILE TRUE
   }
   catch (SocketException& e){
      cout << "Exception was caught:" << e.description() << "\nExiting.\n";
    }

  return 0;
}

//TODO find a way to pass pointers to sockets as well as frameData
//NOTE *arg ... to pass multiple arguments, pass a structure.
//we need a pointer to a structure which contains frames as well as sockets
void *sendData(void* frame_pointer){
  vector<string> *frames_convert;
  frames_convert = (vector<string> *) frame_pointer;
  vector<string> frames = *frames_convert;
/*
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
     }//END WHILE
  }//END OUTER MOST TRY
  catch(SocketException&){
  }
}//END WHILE TRUE
  }
  catch (SocketException& e){
     cout << "Exception was caught:" << e.description() << "\nExiting.\n";
   }*/

}


vector<string> framer()
{
   int i,j,pcounter=0;
   vector<string> temp;
   string ftemp;
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
            ftemp.push_back(input[i+j]);
            pcounter+=input[i+j];
         }
         if(pcounter%2==1){ftemp.push_back('1');}
         else{ftemp.push_back('0');}
         temp.push_back(ftemp);
         ftemp.erase(0,ftemp.size());
      }
   }
   for(i=0;i<1000;i++)
   {
      temp.push_back(temp[i%18]);
   }
   cout<<temp.size()<<"\n\n";
   return temp;
}