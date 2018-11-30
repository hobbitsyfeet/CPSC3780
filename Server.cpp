#include "ServerSocket.h"
#include "SocketException.h"
#include <bitset>
#include <string>
#include <fstream>
#include <string>
#include <vector>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>


using namespace std;


//prototypes
const string flagstr="0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0";
vector<string> framer(string fileName);

int main()
{
  srand(time(NULL));   // Initialization, should only be called once.
   //declare arguments
   vector<vector<string> > fileList;
   vector<string> faker;
   vector<string> frames;


   cout<<"Loading testtext...\n";
   fileList.push_back(framer("testtext.txt"));
   cout<<"Loading lod...\n";
   fileList.push_back(framer("LifeOfDogs.txt"));

  //display information loaded
  /*for(int i; i < fileList[1].size(); i++){
    cout<<"Frame "<< i << ": "<< fileList[1][i]<<endl;
  }*/

  //can handle up to 5 threads
   int NTHREADS = 5;
   pthread_t thread_id[NTHREADS];
   cout << "running....\n";

   try{
      // Create the socket
      ServerSocket server_data(30000);
      ServerSocket server_ack(29999);

      //run the server and open sockets to designated ports
      while (true){
         string request ="";
         //create sockets
         ServerSocket data_socket;
         ServerSocket ack_socket;
         server_data.accept(data_socket);
         server_ack.accept(ack_socket);
         try{

          //bool inputflag = true;
          //do{
              //first communication to determine which file to download
              data_socket << "What do you want to download?\n";

              data_socket>>request;
              cout<<"asking for: "<<request<<endl;
              if(request == "test"){
                 frames = fileList[0];
                 //inputflag = false;
               }

              else if(request == "lod"){
                 frames = fileList[1];
                 //inputflag = false;
               }

               faker=frames;
          //}while(inputflag);
            for(int i=0;i<faker.size();i++)
            {           
               faker[i][3]++;
            }

            //--start to download files--
            //current_index is the index of the frame to send
            int current_index = 0;
            int error_index = -1;
            //data string
            string str = "";
            //The ack from the cilent
            string ack = "";
            bool errorSent=false;


            while (true){

               /*else if(ack == "NAK"){
               //update current frame index
                  errorSent = true;
               }*/
               //update current frame, this is outside of the if statement above for the purpose of error spoofing
              str = frames[current_index];

               if(current_index%5 == 0&&errorSent==false){
                  error_index = rand()%5 + current_index;
               }
               if(error_index == current_index && errorSent==false){
                 //str = "this is dumb";
                  str=faker[current_index];
                  errorSent = true;
               }
               else //if(error_index == current_index && errorSent){
                 {errorSent=false;
               }

               data_socket << str;
               ack_socket >> ack;
               //std::cout<<ack<<endl;
               if(ack == "ACK"){
               //update current frame index
                  current_index++;
               }
            }//END WHILE
         }//END INNER TRY
         catch(SocketException&){}
      }//END WHILE TRUE
   }//END TRY
   catch (SocketException& e){
      cout << "Exception was caught:" << e.description() << "\nExiting.\n";
   }
   return 0;
}

//starts communication with the client, sending data and receiving it



vector<string> framer(string fileName)
{
  char thing[fileName.size()];
  for(int i = 0; i < fileName.size(); i++){
    thing[i] = fileName[i];
 }
 int i,j,pcounter=0;
 vector<string> temp;
 string ftemp;
 ifstream file(thing);
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
      if(pcounter%2==1){
        ftemp.push_back('1');
     }
     else{
        ftemp.push_back('0');
     }
     temp.push_back(ftemp);
     ftemp.erase(0,ftemp.size());
  }
  temp.push_back(flagstr);
}
cout<<fileName <<":";
cout<<temp.size()<<"\n\n";
temp.push_back("__EOF__");
return temp;
}
