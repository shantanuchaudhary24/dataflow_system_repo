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

void main_threads();

int main(void)
{
                   //I am assuming that resources_db file will remain static
       //file locking and microphones is not done yet             
 
          /*************Daemon specific initialization is started***************/
              
               pid_t pid, sid;              // Our process ID and Session ID 
            
               pid = fork();                 // Fork off the parent process 
               
            if (pid < 0) 
            {
                    exit(EXIT_FAILURE);   //in this case we are not able to create a new process 
            }
            
            if (pid > 0)
            {
                    exit(EXIT_SUCCESS); // If we got a good PID, then  we can exit the parent process.             
            }
        
            //umask(0);                                                   //Here we will set the file permssions to other people to a particular value having a particular meaning
                    
            /* Open any logs here */                             // we should open the logs here so that we can have debug info in them though they solves much greater purpose than this
                   
            
               sid = setsid();               // Create a new SID for the child process  in order to give it more privilige
            if (sid < 0) 
            {
                //write theailure event in the log .we ideally cannot interact with the terminal in ths case so we have to store the information in the log files
                    exit(EXIT_FAILURE);
            }                                             

               
               close(STDIN_FILENO);
               close(STDOUT_FILENO);                                // Close out the standard file descriptors these files are redundant since there is no possible input from the standard terminal 
               close(STDERR_FILENO);       
            if ((chdir("/home/Shantanu/Database")) < 0)     // Change the current working directory 
            {
               //directory is changed in order to make the process run completely in the background and to do so the best always available directory is root 
               //write the failure event in the log .we ideally cannot interact with the terminal in ths case so we have to store the information in the log files
                //   debug();
                   exit(EXIT_FAILURE);
                  // debug();
            }
                
               main_threads();            
               
            return 0;
}


void main_threads()
{
                FILE  * fp ;      
                fp = fopen("resources_db","r+b");             // resources_db file is assumed to be already initiated
        
                struct resource_node * rooms_llist_head = create_linked_list(fp);
      
                fclose(fp);
      
      
                create_data_base(rooms_llist_head);                 
 
                pthread_t add_conn_thread ;
                pthread_t del_conn_thread ;
                pthread_t ps_day ;
                pthread_create(&add_conn_thread,NULL,add_connection,(void *)(rooms_llist_head));                      //New thread for new connection
                pthread_create(&del_conn_thread,NULL,del_connection,NULL);                     //Delete thread for each disconnection
                pthread_create(&ps_day,NULL,ps_end_day,NULL);
                pthread_join(add_conn_thread,NULL);                                                              //Defensive programming
                pthread_join(del_conn_thread,NULL);                                                             //Defensive programming
                pthread_join(ps_day,NULL);                                                                              //Defensive programming*/
}

