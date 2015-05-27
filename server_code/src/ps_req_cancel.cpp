#include "../inc/header.h"

int ps_cancel_request(int new_fd,struct ser_user_db * user_info,struct resource_node *pass_data, int request_id)
{

       int req_id_validity = does_request_id_exists(request_id,(*user_info).user_id);
      
       int check = send(new_fd,&req_id_validity,sizeof(int),0);
       if(check < 0)
       {
                perror("sending failed");
                exit(0);                
       }
       
       if(req_id_validity == REQ_ID_INVALID)
       {
             return FAILURE;           
       }
       
       struct request user_request ;       
       struct room_status_node room_node;       
       int user_id = (*user_info).user_id;
       
         FILE * fp;         
         fp = fopen(filename((char *)"user",(*user_info).user_id),"r+b");       
       flock(fileno(fp),LOCK_EX);                    
         fseek(fp,sizeof(struct request)*request_id,SEEK_SET);         
         fread(&user_request,sizeof(struct request),1,fp);       
       flock(fileno(fp),LOCK_UN);                    
         fclose(fp);                                                                                                                            //cancelled     
         
       if(user_request.req_status == CANCEL_CODE)
       {
                return ALREADY_CANCELLED;        
       }  
       else
       {                
                if(user_request.req_status == APPROVED_CODE)
                {                                                        
                        int booked_room = user_request.room_num;                                    //if booked
                        
                              fp = fopen(filename((char *)"room",booked_room),"r+b");                        
                        flock(fileno(fp),LOCK_EX);                                 
                        
                        while(feof(fp)!=true)
                        {
                                        fread(&room_node,sizeof(struct room_status_node),1,fp);                                
                                if(room_node.user_id == user_id && room_node.request_id == request_id )
                                {
                                                      fseek(fp,-sizeof(room_status_node),SEEK_CUR);
                                                      delete_record_room(fp,filename((char *)"room",booked_room));                                                          //then delete this record and have the new file
                                           break;                                                      
                                }
                        }
                        
                        flock(fileno(fp),LOCK_UN);                                                         
                              fclose(fp);
                              
                              fp = fopen(filename((char *)"user",user_id),"r+b");                        
                        flock(fileno(fp),LOCK_EX);                                                         
                              fseek(fp,sizeof(struct request)*request_id,SEEK_SET);         
                              user_request.req_status = CANCEL_CODE;                               
                              fwrite(&user_request,sizeof(struct request),1,fp);                        
                        flock(fileno(fp),LOCK_UN);                                                         
                              fclose(fp);
                               
                        return NOW_CANCELLED;
                 }
                else
                {                        
                        struct resources req_resource = user_request.book_data.resource_data;
                        
                        if(req_resource.audio_recording==true||req_resource.video_recording==true||out_of_time(&(user_request.book_data))==true)               
                        {
                                struct request admin_req ;
                                                                
                                //it implies the data will be in the admin_req_db
                                        fp    = fopen("admin_req_db","r+b");                                
                                flock(fileno(fp),LOCK_EX);                                 
                                                
                                while(feof(fp)!=true)
                                {
                                                 fread(&admin_req,sizeof(struct request),1,fp);                                
                                       if(admin_req.user_id == user_id && admin_req.request_id == request_id )
                                       {
                                                      fseek(fp,-sizeof(request),SEEK_CUR);                                                                 
                                                      delete_record_admin(fp,(char *)"admin_req_db");                                                          //then delete this record and have the new file
                                           break;
                                        }
                                }
                                
                                flock(fileno(fp),LOCK_UN);                                 
                                        fclose(fp);
                                
                                        
                                        fp = fopen(filename((char *)"user",user_id),"r+b");       
                                flock(fileno(fp),LOCK_EX);                                                                 
                                        fseek(fp,sizeof(struct request)*request_id,SEEK_SET);         
                                        user_request.req_status = CANCEL;                               
                                        fwrite(&user_request,sizeof(struct request),1,fp);       
                                flock(fileno(fp),LOCK_UN);                                                                 
                                        fclose(fp);
                                
                                return NOW_CANCELLED;
                        }
                        
                        else
                        {
                                 int room_num  = user_request.room_num ;
                                
                                         fp = fopen(filename((char *)"room",room_num),"r+b");                              
                                 flock(fileno(fp),LOCK_EX);                                 
                                 
                                 while(feof(fp)!=true)
                                 {
                                                    fread(&room_node,sizeof(struct room_status_node),1,fp);
                                
                                          if(room_node.user_id == user_id && room_node.request_id == request_id )
                                          {
                                                                  delete_record_room(fp,filename((char *)"room",room_num));                                                          //then delete this record and have the new file
                                                     break;                                                      
                                          }
                                 }
                        
                                flock(fileno(fp),LOCK_UN);                                                                 
                                        fclose(fp);
                              
                                        fp = fopen(filename((char *)"user",user_id),"r+b");                                        
                                flock(fileno(fp),LOCK_EX);                                                                 
                                        fseek(fp,sizeof(struct request)*request_id,SEEK_SET);         
                                        user_request.req_status = CANCEL;                               
                                        fwrite(&user_request,sizeof(struct request),1,fp);       
                                flock(fileno(fp),LOCK_UN);                                                                 
                                        fclose(fp);                     
                                        
                                return NOW_CANCELLED;                                    
                        }                     
                }                      
       }
       
       return NOW_CANCELLED;                    //dummy                                      
}

