                                //*****VERIFIED AND PROVED CORRECT*******//

#include "../inc/header.h"

void * recv_data(void * args)
{

        int check ;                             //USED FOR RECHECKING WHETHER THE CONNECTION IS PROPERLY ESTABLISHED OR NOT        
        struct recv_thread_init * pass_data = (struct recv_thread_init *)(args);                //ARGUMENTS GIVEN ON THREAD INITATION
               
        int new_fd = pass_data -> new_fd ;         
        struct ser_client_trans_header  transfer_header ;
        struct ser_user_db user_info;
        struct book_info  book_data ;                
          FILE * fp_user_req_file ;                                     
          FILE * fp_server_users_db ;
        int data_code;
                                                        //BIG MAIN LOOP RECEIVING THE DATA AND THEN PROCESSING IT
        while(1)
        {
                    check = recv(new_fd,&transfer_header,sizeof(struct ser_client_trans_header),0);
                                  
                if(check <  0)
                {
                             perror("Sending failed");
                             exit(0);
                }
                
                    data_code  = transfer_header.data_code ;
                                    
                switch(data_code)
                {
                        case NOT_REG_USER :
                        { 
                                             user_info.user_id = allocate_user_id();                                       //ALLOCATING THE USER ID TO THE USER
                                             user_info.num_requests = INITIALISED_TO_ZERO;                     
                                                                                                                                                                                                                        
                                             fp_server_users_db = fopen("server_users_db","r+b");                                             
                                             flock(fileno(fp_server_users_db),LOCK_EX);                                             
                                             fseek(fp_server_users_db,0,SEEK_END);                                      //WRITING ABOUT THE USER INFO AT THE END OF THE FILE
                                             fwrite(&user_info, sizeof(ser_user_db), 1,fp_server_users_db);
                                             flock(fileno(fp_server_users_db),LOCK_UN);
                                             fclose(fp_server_users_db);                                                                                          
                                                                                          
                                             transfer_header.user_id = user_info.user_id;                               //TRANSFERRING THE USER HIS USER ID                                             
                                             check = send(new_fd,&transfer_header,sizeof(struct ser_client_trans_header),0);                                             
                                    if(check <  0)
                                    {
                                                        perror("Sending failed");
                                                        exit(0);
                                    }   
                
                                             fp_user_req_file = fopen(filename((char *)"user",user_info.user_id),"wb");         //FINALLY CREATING THE FILE FOR THE NON REGISTERED USER
                                             fclose(fp_user_req_file);
         
                                              
                                    break ;
                        }
                        case REG_USER :
                        {                                                                                                                                 
                                     bool user_id_exist = does_exist(transfer_header.user_id,fp_server_users_db);                //SEARCHING FOR THE USER  ID IN THE DATABASE 
                
                                     if(user_id_exist == true)
                                     {
                                                                 transfer_header.data_code = USER_ID_EXISTS;                       
                                                                 check = send(new_fd,&transfer_header,sizeof(struct ser_client_trans_header),0);
                         
                                                    if(check <  0)
                                                    {
                                                                     perror("Sending failed");                                          //USER ID EXISTS AND INFORMING THE SAME TO THE CLIENT
                                                                     exit(0);
                                                    }
                                                    
                                                                 user_info.user_id  = transfer_header.user_id;                //STORING THE USER ID TO WHICH WE ARE COMMUNICATING                                                  
                                     }
                
                                     else
                                     {
                                                             transfer_header.data_code = USER_ID_NOT_EXISTS;                                                                
                                                             check = send(new_fd,&transfer_header,sizeof(struct ser_client_trans_header),0);
                         
                                                 if(check <  0)
                                                 {
                                                                     perror("Sending failed");                                          //USER ID DOES NOT EXIST AND INFORMING THE SAME TO THE CLIENT
                                                                     exit(0);
                                                 }      
                                     }                            
           
                                    
                                    break;
                        }        
                        case TYPICAL_BOOK:
                        {
                                        check   = recv(new_fd,&book_data,sizeof(struct book_info),0);                     //DOING THE BOOKING HERE
                                                                     
                                if(check <  0)
                                {
                                                 perror("Sending failed");
                                                 exit(0);
                                }
                                                            
                                        ps_typical_req(new_fd,&book_data,&user_info,pass_data->rooms_llist);    //PROCESSING THE BOOKING HERE
                                                     
                                break ;                                 
                        }       
                        case CANCEL:
                        {
                                int request_id ;                                                                                                        
                                        check = recv(new_fd,&request_id,sizeof(int),0);                                 //TAKING THE REQUEST ID WHICH WE HAVE TO CANCEL
                                        
                                if(check <  0)
                                {
                                                 perror("Sending failed");
                                                 exit(0);
                                }
                                                            
                                        ps_cancel_request(new_fd,&user_info,pass_data->rooms_llist,request_id);  
                                        
                                break ;   
                        }        
                        case REQ_STATUS:
                        {
                                int request_id ;
                                        check = recv(new_fd,&request_id,sizeof(int),0);                                                         //TAKING THE REQUEST ID WHICH WE HAVE TO TELL THE REQUEST STATUS
                                        
                                if(check <  0)
                
                                {
                                                 perror("Sending failed");
                                                 exit(0);
                                }

                                        ps_req_status(new_fd,&user_info,request_id);  
                                        
                                break ;
                        }        
                        case HIGH_PRIOR_BOOK:
                        {
                                        check   = recv(new_fd,&book_data,sizeof(struct book_info),0);                   //DOING THE HIGH_PRIOR BOOKING HERE
                                                                                
                                if(check <  0)
                                {
                                                 perror("Sending failed");
                                                 exit(0);
                                }
                                
                                        ps_hprior_req(new_fd,&book_data,&user_info,pass_data->rooms_llist);     //PROCESSING THE HIGH_PRIOR_REQUEST
                                          
                                break ;                                                                   
                         }               
                }                 
        }
}       
        
       
