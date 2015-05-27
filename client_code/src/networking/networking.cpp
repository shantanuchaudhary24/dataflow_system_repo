#include "../../inc/global_header.h"
#include "../../inc/main_header.h"

extern struct book_info info_object;
int socketfd,port_num;
struct hostent *server;
struct sockaddr_in server_addr,client_addr;

void * send_server(int data_code)
{
	struct ser_client_trans_header transfer_header;
	switch(data_code)
	{	
		case NOT_REG_USER:
		{
			transfer_header.data_code = NOT_REG_USER;
			int check = send(socketfd, &transfer_header, sizeof(struct ser_client_trans_header) , ERROR_CHECK);
			if(check < ERROR_CHECK)
			{
				perror("Header Sending failed");
				exit(ERROR_CHECK);
			}
			break;
		}
		case TYPICAL_BOOK:
		{
			transfer_header.data_code = TYPICAL_BOOK;
			int check = send(socketfd, &transfer_header, sizeof(struct ser_client_trans_header) , ERROR_CHECK);
			if(check < ERROR_CHECK)
			{
				perror("Header Sending failed");
				exit(ERROR_CHECK);
			}
			break;
		}
		case BOOK_DATA:
		{
			int booking_accepted;
			int check = send(socketfd, &info_object, sizeof(struct book_info) , ERROR_CHECK);
			if(check < ERROR_CHECK)
			{
				perror("Header Sending failed");
				exit(ERROR_CHECK);
			}
			check = recv(socketfd, &booking_accepted, sizeof(int), ERROR_CHECK);
			if(check < ERROR_CHECK)
			{
				perror("receiving failed");
				exit(ERROR_CHECK);
			}
			
			if(booking_accepted == REQ_ACCEPT)
			{
				int request_id;
				check = recv(socketfd, &request_id, sizeof(int), ERROR_CHECK);
				if(check < ERROR_CHECK)
				{
					perror("RECEIVING FAILED");
					exit(ERROR_CHECK);
				}
				else
				{
					glui_accepted_request(REQ_ACCEPT);
				}
			}
			if(booking_accepted == REQ_REJECT)
			{
				glui_not_accepted();
			}
			break;
		
		}
	}
	return NULL;
}

void * recv_server(int data_code)
{
	static struct ser_client_trans_header transfer_header;
	switch(data_code)
	{	
		case NOT_REG_USER:
		{
			
			int check = recv(socketfd, &transfer_header, sizeof(struct ser_client_trans_header) , ERROR_CHECK);
			if(check < ERROR_CHECK)
			{
				perror("Header Sending failed");
				exit(ERROR_CHECK);
			}
			else
			{
				return &transfer_header;
			}
			break;
		}
	}
	return NULL;
}

void init_Client()
{                                    
        port_num = PORT;	
	socketfd = socket(AF_INET,SOCK_STREAM,ERROR_CHECK);	//CREATION OF SOCKET
	if(socketfd < ERROR_CHECK)
	{
		perror("Error Creating Socket.");	//CHECKING VALID SOCKET ESTABLISHMENT
		exit(ERROR_CHECK);
	}
	server = gethostbyname(ADDRESS);		//FETCHING SERVER ADDRESS AND PORT
	if(server == NULL)
	{
		fprintf(stderr,"ERROR,No such host.\n");	//CHECKING FOR THE SERVER AVAILABILITY
		exit(ERROR_CHECK);
	}
	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);
	server_addr.sin_port = htons(port_num);		//PORT NUMBER OF SERVER
	if (connect(socketfd,(struct sockaddr *) &server_addr,sizeof(server_addr)) < ERROR_CHECK)
		{
			perror("Error Connecting to Socket");	//CONNECTING TO SERVER
			exit(ERROR_CHECK);				
		}
	return ;					
}
