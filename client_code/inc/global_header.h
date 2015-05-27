#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glui.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>
#include <pthread.h>
using namespace std;

#define WINDOW_POSITION_X 		500
#define WINDOW_POSITION_Y	 	200
#define WINDOW_SIZE_X 			400
#define WINDOW_SIZE_Y 			400
#define BACKGROUND_COLOR 		1.0,1.0,1.0,1.0
#define ARRAY_SIZE 			256
#define NOT_REG_USER 			0
#define REG_USER 			1
#define USER_ID_EXIST 			2
#define USER_ID_NOT_EXIST 		3
#define TYPICAL_BOOK 			4
#define HIGH_PRIOR_BOOK 		5
#define REQ_STATUS 			6
#define CANCEL 				7
#define BOOK_DATA 			8
#define INITIALIZED			0
#define ERROR_CHECK 			0
#define MINUTE 				60
#define MAX_ROOM_SIZE 			1000
#define DEFAULT_YEAR 			1900
#define REQ_ACCEPT 			0
#define REQ_REJECT 			1
#define REQ_ID_VALID 			0
#define REQ_ID_INVALID 			1
#define PENDING_CODE 			0 
#define APPROVED_CODE 			1
#define CANCEL_CODE 			2
#define SET_WHITE_BOARD 		1
#define SET_AC 				2
#define SET_PROJECTOR 			3
#define SET_PROJECTOR_AUDIO 		4
#define SET_PROJECTOR_LCD 		5
#define SET_SOUND_SYSTEM 		6
#define SET_AUDIENCE_SOUND 		7
#define SET_VIDEO_RECORDING 		8
#define SET_VIDEO_AUDIENCE 		9
#define SET_AUDIO_STATE 		10
#define SET_AUDIO_AUDIENCE 		11
#define SET_DATE 			12
#define SET_MONTH 			13
#define SET_YEAR 			14
#define SET_START_TIME_HOURS 		15
#define SET_START_TIME_MINS 		16
#define SET_DURATION_HOURS 		17
#define SET_DURATION_MINS 		18
#define SET_CAPACITY 			19
#define SET_SUBMIT 			20
#define SET_USER_ID 			21
#define SET_NEW_USER 			22
#define SET_OLD_USER 			23
#define SET_CALENDER_ENTRY 		24
#define SET_CAPACITY_ENTRY 		25
#define SET_FORM_REQ 			26
#define SET_REQUEST_ID 			27
#define SET_CANCEL_STATUS 		28
#define SET_OLD_REQUEST 		29
#define SET_SUBMIT_HIGH_PRIOR 		30
#define SET_CHECK_USER_ID 		31

class time
{
	public:	
		int hours;
		int minutes;
};

struct ser_client_trans_header
{
	int data_code;
	int user_id;
};

struct thread_data
{
	int argc;
	char **args;	
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
      struct resources resources_data;      
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
        
        struct resources resources_data;
        
        int duration_hours;
        int duration_minutes;     
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

