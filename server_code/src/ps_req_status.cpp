                        //****VERIFIED AND ESTABLISHED CORRECT********
#include "../inc/header.h"

int ps_req_status(int new_fd,struct ser_user_db * user_info,int request_id)
{

       //PROCESS THE VALUE OF THE REQUEST STATUS  OF THE REQUEST_ID IF IT EXISTS
       
       int req_id_validity = does_request_id_exists(request_id,(*user_info).user_id);      
       int check = send(new_fd,&req_id_validity,sizeof(int),0);
       
       if(check < 0)
       {
                perror("sending failed");
                exit(0);                
       }       
       if(req_id_validity == REQ_ID_INVALID)
       {
             return FAILURE;                                                                    //IF REQUEST_ID IS INVALID SEND THE SAME AND EXIT
       }
       
         sleep(SLEEP_CONS_SEND);
         
       struct request req_request;              
        
         FILE * fp;                    
         fp = fopen(filename((char *)"user",(*user_info).user_id),"r+b");       
         flock(fileno(fp),LOCK_EX);              
         fseek(fp,sizeof(request)*(request_id - 1),SEEK_SET);
         fread(&req_request,sizeof(struct request),1,fp); 
         flock(fileno(fp),LOCK_UN);       
         fclose(fp);               
       
         check = send(new_fd,&req_request.req_status,sizeof(int),0);
       if(check < 0)
       {
                perror("sending failed");
                exit(0);                
       }      
       
       return req_request.req_status;
}

