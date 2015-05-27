                                                        //**VERIFIED AND ESTABLISHED CORRECT*******
                
#include "../inc/header.h"

int send_ser_ps(int new_fd,struct book_info * booking_data,struct ser_user_db * user_info,struct resource_node * pass_data)  
{

         //SENDING TO THE ROOMS DATABASE IF ACCEPTED
         
         struct resource_node * traverse = pass_data->next;                     //PASS DATA IS THE HEAD OF THE LINKED LIST  CONTAINING THE ROOM RESOURCES DATABASE       
         struct resources req_resource = booking_data ->resource_data ;                                
         int return_val = REQ_REJECT;                              //INITIALISED TO THE REQ_REJECT SO THAT WE HAVE TO CHECK FOR THE CASE WHEN WE NEED TO ACCEPT THE REQUEST
                                 
   
         bool start = true;
         int req_room_num;
                    
         while((traverse->next)!=NULL)
         {
             if(does_resources_met(req_resource,traverse->resource_data)==true)
             {
                              FILE * fp = fopen(filename((char *)"room",(traverse->resource_data).room_num),"r+b");                
                              flock(fileno(fp),LOCK_EX);             
                        
                        if(does_timeline_inter(fp,booking_data)==false)
                        {                                                                                                                               
                                            req_room_num = update_room_num(req_resource,traverse->resource_data,start,(traverse->resource_data).room_num); 
                                            return_val = REQ_ACCEPT;                                    //now it is pssible to accept request now the only work to be done in this loop which room
                                            start = false;
                        }
                        
                              flock(fileno(fp),LOCK_UN);                                     
                              fclose(fp);
             }
             
                traverse = traverse->next ;
         }
         
         if(return_val==REQ_REJECT)
         {
                int check = send(new_fd,&return_val,sizeof(int),0);
                                
                if(check < 0)
                {
                                perror("SENDING FAILED");
                                exit(0);              
                }                                
                
                return REQ_REJECT;       
         }
        
        //NOW ESTABLISHED THAT THE REQUEST IS ACCEPTED         
        
        //WRITING IN THE "REQ_ROOM_NUM" FILE THE REQUEST 

          FILE  * fp_server_users_db = fopen("server_users_db","r+b");                                        
          flock(fileno(fp_server_users_db),LOCK_EX);          
          FILE * fp = fopen(filename((char *)"room",req_room_num),"r+b");
          flock(fileno(fp),LOCK_EX);                       
          
        struct room_status_node req_data_room;                
          req_data_room.timeline_init = (*booking_data).timeline_init;
          req_data_room.timeline_fin = (*booking_data).timeline_fin;
          req_data_room.user_id = (*user_info).user_id;
          req_data_room.request_id  = alloc_req_id((*user_info).user_id,fp_server_users_db);     //SERVER_USERS_DB USED HER
          req_data_room.req_time = DUMMY;                                                                 //GET_SYSTEM_TIME HERE
          req_data_room.book_data = *booking_data;
          req_data_room.req_status = PENDING_CODE;                                       //INITIALISED TO PENDING CODE
          req_data_room.req_type =  TYPICAL_BOOK;
          
          fseek(fp,0,SEEK_END);                                                                                                                    //writing at the end of the file  
          fwrite(&req_data_room,sizeof(room_status_node),1,fp); 
          flock(fileno(fp),LOCK_UN);                               
          fclose(fp);  
          flock(fileno(fp_server_users_db),LOCK_UN);  
          fclose(fp_server_users_db);
 
            fp = fopen(filename((char *)"user",(*user_info).user_id),"r+b");                    //WRITING THE REQUEST AT THE END OF THE USER FILE                      
            flock(fileno(fp),LOCK_EX);                       

          struct request request_data_user;
                        
            request_data_user.user_id = (*user_info).user_id;
            request_data_user.request_id  = req_data_room.request_id;
            request_data_user.req_time = req_data_room.req_time;                                                                 //GET SYSTEM_TIME
            request_data_user.book_data = (*booking_data);
            request_data_user.req_status = PENDING_CODE;                                                                            //INTIALISED TO PENDING CODE
            request_data_user.req_type = TYPICAL_BOOK; 
            request_data_user.room_num = req_room_num;
             
            fseek(fp,0,SEEK_END);            
            fwrite(&request_data_user,sizeof(request),1,fp);                                            //WRITING AT THE END OF THE FILE
            flock(fileno(fp),LOCK_UN);                         
            fclose(fp);                         
                               
          int check = send(new_fd,&return_val,sizeof(int),0);                                       //SENDING THAT THE REQUEST IS ACCEPTED
                                
          if(check < 0)
          {
                                        perror("SENDING FAILED");
                                        exit(0);              
           }  
      
              sleep(SLEEP_CONS_SEND);                                                                        //SLEEPING BETWEEN CONSECUTIVE SENDS                                               
              check = send(new_fd,&request_data_user.request_id,sizeof(int),0);                        //SENDING THE REQUEST_ID OF THE REQUEST ACCEPTED
                                
           if(check < 0)
           {
                                        perror("SENDING FAILED");
                                        exit(0);              
           }                           
                                                  
            return return_val;              
         
}
