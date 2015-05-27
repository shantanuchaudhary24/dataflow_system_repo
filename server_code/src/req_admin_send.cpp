
                                                                //********VERIFIED AND ESTABLISHED CORRECT*********

#include "../inc/header.h"

int send_admin(int new_fd , struct book_info * booking_data ,struct ser_user_db * user_info , struct resource_node * pass_data,int TYPE_BOOK)
{

                     //SENDING TO THE ADMIN PENDING REQUESTS FILE IF ACCEPTED
                        
                     struct resource_node * traverse = pass_data->next;                     //PASS DATA IS THE HEAD OF THE LINKED LIST  CONTAINING THE ROOM RESOURCES DATABASE       
                     struct resources req_resource = booking_data ->resource_data ;                                
                     int return_val = REQ_REJECT;                              //INITIALISED TO THE REQ_REJECT SO THAT WE HAVE TO CHECK FOR THE CASE WHEN WE NEED TO ACCEPT THE REQUEST
                                                                                                              
                     while((traverse->next)!=NULL)
                     {
                             if(does_resources_met(req_resource,traverse->resource_data)==true)
                             {                                                                                    
                                                      return_val = REQ_ACCEPT;                                    //EVEN IF ONE ROOM MET THE DEMAND THEN WE ACCEPT THE REQUEST
                                           break;
                             }
                                     traverse = traverse->next ;
                     }
                                
                      if(return_val == REQ_REJECT)
                      {                                                                                       
                                int check = send(new_fd,&return_val,sizeof(int),0);
                                
                                if(check < 0)
                                {
                                                perror("SENDING FAILED");
                                                exit(0);              
                                }                                                                                                                                                 
                                return REQ_REJECT;
                                
                      }

                      struct request request_data;                                                      //STORING THE REQUEST DATA TO BE WRITTEN INTO THE FILE
                     
                           FILE * fp ;                                                      
                           FILE  * fp_server_users_db = fopen("server_users_db","r+b");                                        
                           flock(fileno(fp_server_users_db),LOCK_EX);                                                            
                           fp = fopen("admin_req_db","r+b");                                        
                           flock(fileno(fp),LOCK_EX);               
                        
                           request_data.user_id = (*user_info).user_id;
                           request_data.request_id  = alloc_req_id((*user_info).user_id,fp_server_users_db);           //ALLOCATES THE REQUEST_ID AND UPDATES IN THE FILE  **BUG HERE
                           request_data.req_time = DUMMY;                                                                                  //GET SYSTEM TIME
                           request_data.book_data = (*booking_data);
                           request_data.req_status = PENDING_CODE;                                                                     //INITIALISED  TO PENDING_CODE
                           request_data.req_type = TYPE_BOOK; 
                      
                            fseek(fp,0,SEEK_END);                                                                                                                    
                            fwrite(&request_data,sizeof(request),1,fp);                                               //WRITING THE REQUEST AT THE END OF THE ADMIN PENDING REQUESTS FILE
                                                
                            flock(fileno(fp_server_users_db),LOCK_UN);                                                 
                            fclose(fp_server_users_db);
                                                          
                            flock(fileno(fp),LOCK_UN);                       
                            fclose(fp);
                                                                          
                            fp = fopen(filename((char *)"user",(*user_info).user_id),"r+b");                    //WRITING THE REQUEST AT THE END OF THE USER FILE                      
                            flock(fileno(fp),LOCK_EX);                       
                            fseek(fp,0,SEEK_END);                                                                               //WRITING AT THE END OF THE FILE  
                            fwrite(&request_data,sizeof(request),1,fp);                       
                            flock(fileno(fp),LOCK_UN);                                               
                            fclose(fp);                     
                                
                      int check = send(new_fd,&return_val,sizeof(int),0);                                       //SENDING THAT THE REQUEST IS ACCEPTED
                                
                      if(check < 0)
                      {
                                        perror("SENDING FAILED");
                                        exit(0);              
                      }  
      
                           sleep(SLEEP_CONS_SEND);                                                                        //SLEEPING BETWEEN CONSECUTIVE SENDS                                               
                           check = send(new_fd,&request_data.request_id,sizeof(int),0);                        //SENDING THE REQUEST_ID OF THE REQUEST ACCEPTED
                                
                      if(check < 0)
                      {
                                        perror("SENDING FAILED");
                                        exit(0);              
                      }                           
                                                  
                      return return_val;              

}
