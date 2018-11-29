#include "ServerSocket.h"
#include "SocketException.h"
#include <bitset>
#include <string>
#include <fstream>
#include <string>
#include <vector>
#include <pthread.h>
using namespace std;


//prototypes
const string flagstr="0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0";
vector<string> framer(string fileName);

int main()
{
   //declare arguments
   vector<vector<string> > fileList;
   bool flag=true;
   vector<string> faker;
   vector<string> frames;
   string request ="";

   cout<<"Loading testtext...\n";
   fileList.push_back(framer("testtext.txt"));
   cout<<"Loading LifeOfDogs...\n";
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
         //create sockets
         ServerSocket data_socket;
         ServerSocket ack_socket;
         server_data.accept(data_socket);
         server_ack.accept(ack_socket);
         try{
            //first communication to determine which file to download
            data_socket << "What do you want to download?\n";
            cout<<request;
            data_socket>>request;

            if(request == "testtext.txt")
               frames = fileList[0];

            else if(request == "LifeOfDogs.txt")
               frames = fileList[1];
            else
               frames = fileList[1];
            for(int i=0;i<frames.size();i++)
            {
               faker.push_back(frames[i]);
               faker[i][3]++;
            }

            //--start to download files--

            //current_index is the index of the frame to send
            int current_index = -1;
            //data string
            string str = "";
            //The ack from the cilent
            string ack = "ACK";
            while (true){
               if(ack == "ACK"){
               //update current frame index
                  current_index++;
               }
               //update current frame, this is outside of the if statement above for the purpose of error spoofing
               str = frames[current_index];
               if(current_index%5==0&&flag==true){
                  str=faker[current_index];
                  flag=false;
               }
               if(current_index%5==2){flag=true;}
               data_socket << str;
               // get ack
               ack_socket >> ack;
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
