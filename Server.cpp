#include "ServerSocket.h"
#include "SocketException.h"
#include <bitset>
#include <string>
#include <fstream>
#include <string>
#include <vector>
#include <pthread.h>
using namespace std;

struct Arguments{
  vector<string>* frames_ptr;
  ServerSocket* data_socket_ptr;
  ServerSocket* ack_socket_ptr;

};

vector<string> framer();


//TODO function that sends the data from the frame_pointer
void *sendData(void* arg_ptr);

int main()
{
  Arguments arg;
  Arguments* arg_ptr = &arg;

  vector<string> frames = framer();
  arg.frames_ptr = &frames;

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


   cout << "running....\n";

   try{
      // Create the socket
      ServerSocket server_data(30000);
      ServerSocket server_ack(29999);

      while (true){
	       ServerSocket data_socket;
         ServerSocket ack_socket;

         arg.data_socket_ptr = &data_socket;
         arg.ack_socket_ptr = &ack_socket;


         //create the threads
          for(int thread = 0; thread < NTHREADS; thread++){
            //for each thread, server must accept a socket
            server_data.accept(data_socket);
            server_ack.accept(ack_socket);
             //create a thread to sendData, using sockets and vector as arguments
             pthread_create( &thread_id[thread], NULL, &sendData ,arg_ptr);
          }
           //join each thread
           for(int thread  = 0; thread < NTHREADS; thread ++){
             pthread_join( thread_id[thread],NULL);
           }

         }//END WHILE TRUE
       }//END TRY
       catch (SocketException& e){
         cout << "Exception was caught:" << e.description() << "\nExiting.\n";
       }
  return 0;
}

//TODO find a way to pass pointers to sockets as well as frameData
//NOTE *arg ... to pass multiple arguments, pass a structure.
//we need a pointer to a structure which contains frames as well as sockets
void *sendData(void* arg_ptr){
  //convert pointer to Arguments type
  Arguments* arg_convert = (Arguments*)arg_ptr;
  //arg is dereferenced converted pointer (same as arg in main)
  Arguments arg = *arg_convert;

  //dereference to concrete versions of each a
  vector<string> frames = *arg.frames_ptr;
  ServerSocket data_socket = *arg.data_socket_ptr;
  ServerSocket ack_socket = *arg.ack_socket_ptr;

  try{
     //current_index is the index of the frame to send
     int current_index = -1;
     //data string
     string str = "";
     //The ack from the cilent
     string ack = "ACK";
     while (true){
         if(ack == "ACK"){
           current_index++;
           //string is current frame
           str = frames[current_index];
         }
         data_socket << str;
         // get ack
         ack_socket >> ack;
     }//END WHILE
  }//END OUTER MOST TRY
  catch(SocketException&){
  }
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
   cout<<temp.size()<<"\n\n";
   return temp;
}
