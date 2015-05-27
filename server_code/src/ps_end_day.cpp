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

void * ps_end_day(void * args)
{
         time_t cur_time;
       char * strng;
          
      
       
       
    while(1)
    {
                    time(&cur_time);   
                    
                    strng = ctime(&cur_time);
                    
                int cur_time_hrs = ((((int)strng[11]) - 48)*10 + (((int)strng[12]) - 48));   
                int cur_time_mins = ((((int)strng[14]) - 48)*10 + (((int)strng[15]) - 48));
        
                if(cur_time_hrs == SERVER_PS_TIME_HOURS && cur_time_mins == SERVER_PS_TIME_MINUTES)          //only then enter the code
                {
                        FILE * fp1 = fopen("server_users_db","r+b");                                                             
                   
                        FILE * fp2;    
                        
                        flock(fileno(fp1),LOCK_EX);                          
                        
                   while(feof(fp1)!=true)
                   {
                          struct ser_user_db server_str;
                                 fread(&server_str,sizeof(ser_user_db),1,fp1);
               
                          int user_id = server_str.user_id;
                          int num_requests = server_str.num_requests;
                        
                                fp2 = fopen(filename((char *)"user",user_id),"r+b");
                          
                                flock(fileno(fp2),LOCK_EX);    
                               
                         struct request user_request;
                     
                         int loop_var ;
                         for(loop_var = 0 ; loop_var < num_requests ; loop_var++ )
                         {
                                               fread(&user_request,sizeof(request),1,fp2);
                                     if(user_request.req_status==PENDING_CODE)
                                     {
                                           struct book_info book_data = user_request.book_data;
                                           
                                                       /***********THIS IS ALREADY DECIDE IMPORTANT POINT***************/
                                           int room_num = user_request.room_num;                    
                                                                       
                                           if(!(book_data.resource_data.audio_recording==true||book_data.resource_data.video_recording==true||out_of_time(&book_data)==true))          //there is not !!!!!             !!!!!!!!!!!!!!!!!!!!!!
                                           {
                                          
                                                              fseek(fp2,-(sizeof(request)),SEEK_CUR);
                                                              user_request.req_status = APPROVED_CODE;                          //written into the user file
                                                              fwrite(&user_request,sizeof(request),1,fp2);
                                                    
                                                              update_room_file(user_id,loop_var+1,room_num);                       //done
                                                   
                                           }
                                        }                                                    
                           } 
                           
                                   flock(fileno(fp2),LOCK_UN);             
                                   fclose(fp2);                                                                          
                     }
                     
                           flock(fileno(fp1),LOCK_UN);                                                             
                           fclose(fp1);
                }  
        
                    sleep(SER_PROCESS_SLEEP);                
    }
        
    return NULL;
}

void update_room_file(int user_id,int request_id,int room_num)
{
        FILE * fp = fopen(filename((char *)"room",room_num),"r+b");

        flock(fileno(fp),LOCK_EX);  
       struct room_status_node room_node ;
      
       while(feof(fp)!=true)
       {
                    fread(&room_node,sizeof(room_status_node),1,fp);
                
                if(room_node.user_id == user_id && room_node.request_id == request_id)
                {
                          fseek(fp,-(sizeof(room_status_node)),SEEK_CUR);
                          
                          room_node.req_status = APPROVED_CODE;
                          
                          fwrite(&room_node,sizeof(room_status_node),1,fp);
                          
                          fclose(fp);
                     return ;
                }                                    
       }
       
       flock(fileno(fp),LOCK_UN);     
       fclose(fp);
       
      return ;
}


