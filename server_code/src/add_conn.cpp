

                                                //***************VERIFIED AND ESTABLISHED CORRECT**********

#include "../inc/header.h"

void * add_connection(void * args)
{

        //ACCEPT CONNECTIONS ONLY AND INITIATE A RECEIVE THREAD FOR THEM WHICH WILL PROCESS THE RECEIVED DATA
        
        struct resource_node * rooms_llist_head = (struct resource_node *)(args);        
          pthread_t recv_conn_thread ;                                
        int new_socket_fd;        
        struct sockaddr_in server_addr,client_addr;
          socklen_t clilen;          
        int server_fd;							//SERVER SOCKET FILE DESCRIPTOR
	server_fd = socket(AF_INET,SOCK_STREAM,0);        //CREATE A NEW SOCKET FD SERVER
	
	if(server_fd < 0)
	{
		perror("Server-Socket");
		exit(0);                                                      //SERVER_SOCKET IS NOT CREATED
	}
	
	bzero((char*)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;			//PORT ADDRESSED TO SOCKET
	server_addr.sin_port = htons(PORT);
	
	if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <  0)
	{
		perror("Error Binding");			//BINDING PORT TO SOCKET AND CHECKING IF BINDED CORRECTLY
		exit(0);
	}
	
	int check = listen(server_fd,MAX_CONN_PROCESS); 				//LISTENING FOR NEW CONNECTIONS
	
        if(check < 0)
        {
                	perror("Error Listening");                                      //CHECKING FOR ERROR IN LISTENING
                	exit(0);
        }
	


                                //*********BIG LOOP********************//
	while(1)
	{               
                    new_socket_fd = accept(server_fd,(struct sockaddr *)&client_addr,&clilen);     //ACCEPTED THE CONNECTION					        
                                                                                                                                                                	         
                struct recv_thread_init pass_data;                	
                	pass_data.new_fd  = new_socket_fd ;                                                           //PASSING DATA ON CONNECTION
                	pass_data.rooms_llist = rooms_llist_head;
                	
        	          pthread_create(&recv_conn_thread ,NULL,recv_data,(void *)(&pass_data));	//CREATING A THREAD CORRESPONDING TO THAT CONNECTION WHO WILL CATER TO ITS REQUESTS
        }
        
        return NULL ;
            
}


