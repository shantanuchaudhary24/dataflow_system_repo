#include <iostream>
using namespace std;
#include <time.h>
#include <sys/file.h>
#include <list>         
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>                                              //LIBRARY HEADER FILES INCLUDED 
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>

#define  MAX_CONN_PROCESS 7
#define NOT_REG_USER            0
#define REG_USER                      1
#define USER_ID_EXISTS 2
#define USER_ID_NOT_EXISTS 3
#define TYPICAL_BOOK 4
#define HIGH_PRIOR_BOOK 5
#define REQ_STATUS 6
#define CANCEL 7
#define INITIALISED_TO_ZERO 0
#define PENDING_CODE 0
#define APPROVED_CODE 1
#define CANCEL_CODE 2
#define ROOM_BOOKED 0
#define UNDR_CONS 1
#define IDLE 2
#define FIRSTGSEC 0
#define FIRSTESEC 1
#define FIRSTLSEC 2
#define REQ_ACCEPT 0
#define REQ_REJECT 1
#define SOUND_PRIORITY 4
#define  LCD_PRIORITY 3
#define  PROJECTOR_PRIORITY 2
#define   PROJ_AUDIO_PRIORITY 1
#define ROOM_PRIORITY 5                        //1 POINT FOR EACH ROOM_PRIORITY DIFFERENCE IN SITTING  
#define START_OFFICE 9
#define END_OFFICE 17
#define  NOW_CANCELLED 1
#define ALREADY_CANCELLED 2
#define DUMMY 1
#define PORT 7102
#define SERVER_PS_TIME_HOURS 3
#define SERVER_PS_TIME_MINUTES  10
#define SER_PROCESS_SLEEP 2
#define NO_MIKES_POOL 10
#define SLEEP_CONS_SEND 1
#define REQ_ID_VALID 0
#define REQ_ID_INVALID 1
#define FAILURE 0
#define MAX_FILENAME_SIZE 30

struct ser_client_trans_header 
{
        int data_code ;
        int  user_id;                                     
};

struct ser_user_db
{
        int user_id;
        int num_requests;
};

struct timeline
{
        int day_num;
        int month_num;
        int year_num;
        
        int time_hours;
        int time_minutes;
                
};


struct resources 
{
        int room_num;
        int capacity_room ;
        
        bool white_board;
        bool AC;
        
        bool projector;
        bool proj_with_audio;        
        bool LCD_Display;                               //same structure for user resource struct and room corresonding struct in which case you have to neglect microphones 
        
        bool sound_system;
        bool microphones ;
        bool audio_recording;
        bool video_recording;       
};

struct resource_node
{
      struct resource_node   * previous;
      struct resource_node * next;     
      struct resources  resource_data;      
};


struct recv_thread_init
{
        int new_fd ;
        struct resource_node * rooms_llist;
};

struct book_info
{ 
        struct timeline timeline_init ;
        
        struct timeline timeline_fin;
        
        struct resources resource_data;     
};


struct request
{
        int user_id;
        int request_id;
        int req_time;
        struct book_info book_data;
        int req_status;
        int req_type;
        int room_num;                           //applicable if approved        
};

struct room_status_node
{
        struct timeline timeline_init;
        struct timeline timeline_fin;
        int user_id;
        int request_id;
        int req_time;
        struct book_info book_data;
        int req_status;                         //Approved or under considertion
        int req_type;                            //typical or high priority
        int room_num;        
};




                //functions
                                //main                                

                                        //aux.c
int  allocate_user_id();
bool does_exist(int userid ,FILE * server_users_db );
bool out_of_time(struct book_info * booking_data);
bool does_resources_met(struct resources req_resource,struct resources avail_resource);
bool does_timeline_inter(FILE * fp,struct book_info * booking_data);
int update_room_num(struct resources req_res, struct resources avail_res,bool start,int req_room_num );
int resource_difference(struct resources req_res , struct resources avail_res);
int compare_timeline(struct timeline tl1,struct timeline tl2);
int alloc_req_id(int user_id,FILE * server_users_db);
char * filename(char * string, int num);
void delete_record_room(FILE * fp,char * filename);
int does_request_id_exists(int request_id,int user_id);
void  delete_record_admin(FILE * fp,char * filename);

                                        //create_db.c
struct resource_node * create_linked_list(FILE * rooms_db);
void create_data_base(struct resource_node * head);

                                        //ps_end_day.c
void * ps_end_day(void * args);
void update_room_file(int user_id,int request_id,int room_num);
void  delete_record(FILE * fp,int size);        
                                        //del_connection.c
void * del_connection(void * args) ;

                                        //ps_recv_data.c
int ps_req_status(int new_fd,struct ser_user_db * user_info,int request_id);
int ps_cancel_request(int new_fd,struct ser_user_db * user_info,struct resource_node *pass_data, int request_id);
int ps_typical_req(int new_fd,struct book_info * booking_data,struct ser_user_db * user_info,struct resource_node * pass_data);
int ps_hprior_req(int new_fd , struct book_info * booking_data ,struct ser_user_db * user_info , struct resource_node * pass_data );
int send_admin(int new_fd , struct book_info * booking_data ,struct ser_user_db * user_info , struct resource_node * pass_data,int TYPE_BOOK);
int send_ser_ps(int new_fd,struct book_info * booking_data,struct ser_user_db * user_info,struct resource_node * pass_data);


                                        //add_conn.c
void * add_connection(void * args);

                                        //recv_data.c
void * recv_data(void * args);
                                                                                
                                                                                  
void debug();                                        
