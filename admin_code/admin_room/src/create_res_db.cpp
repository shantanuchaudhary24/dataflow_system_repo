#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>

#include "../inc/header.h"


int main(int argc,char * args[])
{
        
        struct resources room;
        int num_rooms = atoi(args[1]);
        int debug = atoi(args[2]);      
       
        if ((chdir("/home/chain/Database")) < 0)     // Change the current working directory 
        {
               //directory is changed in order to make the process run completely in the background and to do so the best always available directory is root 
               //write the failure event in the log .we ideally cannot interact with the terminal in ths case so we have to store the information in the log files
                  
                   exit(EXIT_FAILURE);

        }
        
           FILE * fp = fopen("resources_db","wb");                      //first clearing the components
           fclose(fp);
         
           fp = fopen("resources_db","r+b");
               
         int counter ;
                 
         for(counter = 0 ; counter < num_rooms ; counter ++)
         {
                room.room_num = counter + 1 ;
                room.capacity_room = 10*(room.room_num) ;               
                
                fwrite(&room,sizeof(struct resources),1,fp);
         }
        
            fclose(fp);
            fp = fopen("resources_db","r+b");
      
        
          while(feof(fp)!=true)
          {
                fread(&room,sizeof(struct resources),1,fp);
             if(debug == 1)
             {
                        printf("ROOM_NUM : %d ROOM_CAPACITY : %d",room.room_num,room.capacity_room);                
             }
          }
          
          
            fclose(fp);         
          return 0;
}
