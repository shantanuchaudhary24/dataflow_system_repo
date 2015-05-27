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

struct resource_node * create_linked_list(FILE * rooms_db)
{
        rewind(rooms_db);                                       //setted to the start of the file
            
      struct resources record;                                        
       
      //creating the head and tail
      
      struct resource_node * head = (resource_node *)malloc(sizeof(resource_node));
      
      struct resource_node  * tail = (resource_node *)malloc(sizeof(resource_node));
      
       head ->previous =  NULL;
       head->next = tail;
       tail->previous = head;
       tail->next = NULL;
       
      
      while(feof(rooms_db)!=true)                                               //detecting the end of file and beware of empty file assuming that it is not
      {
                 fread(&record,sizeof(struct resources),1,rooms_db);
                
             //create a node before tail
             
             struct resource_node * node = (resource_node *)malloc(sizeof(resource_node));
      
                                        //inserting the node       
             (tail->previous)->next  = node ;
                 node->previous = (tail->previous) ;
                 tail  ->previous = node ;
                 node -> next = tail;   
                 
                 node->resource_data = record;          
                 
     }         
      
      return head ;
}

void create_data_base(struct resource_node * head)
{            
      
      struct resource_node * traverse = head->next;                     //pass_data is actually this head         
      
        FILE    * fp ;
        
      //in each iteration can corresponding room satisfy the booking conditions 
      while((traverse->next)!=NULL)
      {
              fp  = fopen(filename((char *)"room",(traverse->resource_data).room_num),"wb");                                    //just creating the files    
              fclose(fp);
              
              printf("ENTERED IN CREATING ROOM FILES");    
              traverse = traverse->next ;
      }
      
        fp = fopen("admin_req_db","a+b");
        fclose(fp);              
       
        fp  = fopen("server_users_db","a+b");             //server_users_db is created in order to store the user_id
        fclose(fp);
        
}

