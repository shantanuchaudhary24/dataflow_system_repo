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
#define WINDOW_POSITION_Y 		200
#define WINDOW_SIZE_X 			400
#define WINDOW_SIZE_Y 			400
#define MAX_ROOM_SIZE 			1000
#define INITIALIZED 			0
#define ERROR_CHECK			0
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
#define SET_CAPACITY 			12
#define SET_DATABASE			13
#define SET_INVALID_CAPACITY		14
#define SET_START_TIME_HOURS 		15
#define SET_START_TIME_MINS 		16
#define SET_END_TIME_HOURS 		17
#define SET_END_TIME_MINS 		18
#define SET_NUM_OPERATORS		19
#define SET_NUM_MICS			20
#define SET_NUM_ROOMS			21
#define SET_POOL_DATABASE		22


struct timeline
{
        int day_num;
        int month_num;
        int year_num;
        
        int time_hours;
        int time_minutes;
                
};

struct general_pool
{
	int num_rooms;
	int num_operators;
	int num_microphones;
	int start_time_hours;
	int start_time_mins;
	int end_time_hours;
	int end_time_mins;
};

struct resources 
{
        int room_ID;
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

