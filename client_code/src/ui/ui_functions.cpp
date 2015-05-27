//####################################### START###############################################################
#include "../../inc/global_header.h"
#include "../../inc/main_header.h"
//#######################################FUNCTIONS & HEADERS##################################################
void init_global_form();
void switcher_function(int value);
void init_checkstatus_menu();
void init_form();
void init_global_cancel();
//######################################VARIABLES GLOBAL TO THIS FILE#########################################
extern int socketfd;
struct book_info info_object;
GLUI_Panel *projector_panel, *sound_system_panel, *video_system_panel, *audio_panel, *lcd_panel ;
GLUI_EditText *date_box, *month_box, *year_box, *time_hours_box, *time_minutes_box, *duration_hours_box, *duration_minutes_box;
GLUI_EditText *user_id_box, *capacity_box, *request_id_box;
GLUI *invalid_window, *invalid_capacity_window;
char ID_string[ARRAY_SIZE];
int request_id_input;
int user_id;
//##################################### DEFAULT STATES FOR INTERFACE ##########################################
bool white_board_state			= false;
bool AC_state 				= false;
bool projector_state			= false;
bool sound_system_state 		= false;
bool audio_recording_state 		= false;
bool video_recording_state 		= false;
bool projector_with_LCD_state 		= true;
bool projector_with_audio_state 	= true;
bool sound_system_audience_state 	= true;
bool audio_recording_audience_state 	= true;
bool video_recording_audience_state 	= true;
bool cancel_status			= true;


void init_global_cancel()
{
	cancel_status = true;
}
//######################### FUNCTION RESPONSIBLE FOR INITIALIZING THE STATES ###################################
void init_global_form()
{
	 white_board_state			= false;
	 AC_state 				= false;
	 projector_state			= false;
	 sound_system_state	 		= false;
	 audio_recording_state 			= false;
	 video_recording_state 			= false;
	 projector_with_LCD_state 		= true;
	 projector_with_audio_state	 	= true;
	 sound_system_audience_state 		= true;
	 audio_recording_audience_state 	= true;
	 video_recording_audience_state 	= true;
}
//################################### FORM FOR TAKING CONFIGURATION FROM USER ####################################
void init_form()
{
	GLUI_Master.close_all();
	GLUI *glui_window = GLUI_Master.create_glui( "RMS v1.0");
	glui_window->add_statictext(ID_string);
	GLUI_Panel *time_slot_panel, *date_panel, *start_time_panel, *duration_panel;
	GLUI_RadioGroup *projector_panel_group, *sound_system_panel_group, *video_system_panel_group, * audio_panel_group, *lcd_panel_group;
	
	init_global_form();
	init_default();
//#################################### DISPLAY CHECKBOXES AND THE USER ID #########################################
	glui_window->add_statictext("SELECT YOUR REQUIREMENTS");
	glui_window->add_checkbox("WHITEBOARD",NULL,SET_WHITE_BOARD,switcher_function);
	glui_window->add_checkbox("AIR-CONDITIONING",NULL,SET_AC,switcher_function);
	glui_window->add_checkbox("PROJECTOR",NULL,SET_PROJECTOR,switcher_function);
//############################## ADDING PANEL WHICH INCLUDES PROJECTOR AUDIO CONFIGURATION ########################		
	projector_panel 	= glui_window->add_panel("CONFIGURATION");
	projector_panel_group 	= glui_window->add_radiogroup_to_panel(projector_panel,NULL,SET_PROJECTOR_AUDIO,switcher_function);
	glui_window->add_radiobutton_to_group(projector_panel_group,"WITH AUDIO");
	glui_window->add_radiobutton_to_group(projector_panel_group,"WITHOUT AUDIO");
	projector_panel->disable();	//DISABLE PANEL WHEN CHECK INACTIVE
//############################## ADDING PANEL WHICH INCLUDES PROJECTOR LCD CONFIGURATION ###########################
	lcd_panel 		= glui_window->add_panel("CONFIGURATION");
	lcd_panel_group 	= glui_window->add_radiogroup_to_panel(lcd_panel,NULL,SET_PROJECTOR_LCD,switcher_function);
	glui_window->add_radiobutton_to_group(lcd_panel_group,"WITH LCD");
	glui_window->add_radiobutton_to_group(lcd_panel_group,"WITHOUT LCD");
	lcd_panel->disable();		//DISABLE PANEL WHEN CHECK INACTIVE
//############################## ADDING PANEL WHICH INCLUDES SOUND SYSTEM CONFIGURATION ###########################
	glui_window->add_checkbox("SOUND SYSTEM",NULL,SET_SOUND_SYSTEM,switcher_function);
	sound_system_panel 	= glui_window->add_panel("CONFIGURATION");
	sound_system_panel_group= glui_window->add_radiogroup_to_panel(sound_system_panel,NULL,SET_AUDIENCE_SOUND,switcher_function);
	glui_window->add_radiobutton_to_group(sound_system_panel_group, "FOR AUDIENCE");
	glui_window->add_radiobutton_to_group(sound_system_panel_group, "NOT FOR AUDIENCE");
	sound_system_panel->disable();
//############################################ ADDING A SEPARATOR COLUMN ###########################################
	glui_window->add_column(true);
//##################################### CHECK BOX AND PANEL FOR VDEO REC. SYSTEM ###################################	
	glui_window->add_checkbox("VIDEO REC. SYSTEM",NULL,SET_VIDEO_RECORDING,switcher_function);
	video_system_panel 	= glui_window->add_panel("CONFIGURATION");
	video_system_panel_group= glui_window->add_radiogroup_to_panel(video_system_panel,NULL,SET_VIDEO_AUDIENCE,switcher_function);
	glui_window->add_radiobutton_to_group(video_system_panel_group, "FOR AUDIENCE");
	glui_window->add_radiobutton_to_group(video_system_panel_group, "NOT FOR AUDIENCE");
	video_system_panel->disable();  // DISABLE PANEL WHEN CHECK INACTIVE
//##################################### CHECK BOX AND PANEL FOR AUDIO REC. SYSTEM ################################## 
	glui_window->add_checkbox("AUDIO REC. SYSTEM",NULL,SET_AUDIO_STATE,switcher_function);
	audio_panel 	= glui_window->add_panel("CONFIGURATION");
	audio_panel_group= glui_window->add_radiogroup_to_panel(audio_panel,NULL,SET_AUDIO_AUDIENCE,switcher_function);
	glui_window->add_radiobutton_to_group(audio_panel_group, "FOR AUDIENCE");
	glui_window->add_radiobutton_to_group(audio_panel_group, "NOT FOR AUDIENCE");
	audio_panel->disable();
//################################# PANEL AND TEXTBOX FOR DATE/MONTH/YEAR/TIME/DURATION ############################
	glui_window->add_statictext("CONSULT ADMINISTRATOR FOR TIME SLOTS");
	date_panel = glui_window->add_panel("EVENT DATE",GLUI_PANEL_EMBOSSED);
	date_box = glui_window-> add_edittext_to_panel( date_panel,"DATE", GLUI_EDITTEXT_INT,NULL,SET_DATE, (GLUI_Update_CB)switcher_function);
	month_box= glui_window-> add_edittext_to_panel( date_panel,"MONTH", GLUI_EDITTEXT_INT, NULL, SET_MONTH,(GLUI_Update_CB)switcher_function);
	year_box = glui_window-> add_edittext_to_panel( date_panel,"YEAR", GLUI_EDITTEXT_INT, NULL, SET_YEAR,(GLUI_Update_CB)switcher_function);
//########################################### COLUMN SEPARATOR ######################################################
	glui_window->add_column(true);
//#################################### STARTING TIME AND DURATION ##################################################
	time_slot_panel = glui_window->add_panel("EVENT TIME", GLUI_PANEL_EMBOSSED);
	start_time_panel= glui_window-> add_panel_to_panel( time_slot_panel,"START TIME:",GLUI_PANEL_EMBOSSED);
	time_hours_box = glui_window-> add_edittext_to_panel( start_time_panel,"TIME(HRS):", GLUI_EDITTEXT_INT,NULL, SET_START_TIME_HOURS,(GLUI_Update_CB)switcher_function);
	time_minutes_box = glui_window-> add_edittext_to_panel( start_time_panel,"TIME(MINS):",GLUI_EDITTEXT_INT, NULL, SET_START_TIME_MINS,(GLUI_Update_CB)switcher_function);
	duration_panel  = glui_window-> add_panel_to_panel( time_slot_panel,"DURATION:",GLUI_PANEL_EMBOSSED);
	duration_hours_box = glui_window-> add_edittext_to_panel( duration_panel,"HOURS:", GLUI_EDITTEXT_INT, NULL, SET_DURATION_HOURS,(GLUI_Update_CB)switcher_function);
	duration_minutes_box = glui_window-> add_edittext_to_panel( duration_panel,"MINS:", GLUI_EDITTEXT_INT, NULL, SET_DURATION_MINS,(GLUI_Update_CB)switcher_function);
//######################################## ASKING FOR CAPACITY OF ROOM ##############################################	
	capacity_box = glui_window-> add_edittext( "CAPACITY", GLUI_EDITTEXT_INT, NULL, SET_CAPACITY,(GLUI_Update_CB)switcher_function);
	glui_window-> add_button( "SUBMIT",SET_SUBMIT,(GLUI_Update_CB)switcher_function);
	glui_window-> add_button( "SUBMIT (HIGH PRIORITY)",SET_SUBMIT_HIGH_PRIOR,(GLUI_Update_CB)switcher_function);
	glui_window-> add_button( "GO BACK",ERROR_CHECK,(GLUI_Update_CB)init_menu);
	glui_window-> add_button( "QUIT",ERROR_CHECK,(GLUI_Update_CB)exit);

}

void init_checkstatus_menu()
{
	GLUI_Master.close_all();
	GLUI *glui_window = GLUI_Master.create_glui( "RMS v1.0");
	glui_window->add_column(true);
	GLUI_Panel *glui = glui_window->add_panel("PLEASE ENTER THE ID ALLOTED TO YOU:", GLUI_PANEL_EMBOSSED);
	user_id_box = glui_window-> add_edittext_to_panel( glui,"USER ID", GLUI_EDITTEXT_INT, NULL, SET_USER_ID,(GLUI_Update_CB)switcher_function);
	glui_window-> add_button_to_panel( glui,"OK",SET_CHECK_USER_ID,(GLUI_Update_CB)switcher_function); //connection to server
	glui_window-> add_button_to_panel( glui,"GO BACK",ERROR_CHECK,(GLUI_Update_CB)init_menu);
	glui_window->add_column(true);
}



void switcher_function(int value)
{
	class time end_time ;

	switch(value)
	{
		case SET_WHITE_BOARD:// check for white_board
		{	if(white_board_state == false)
			{
				info_object.resources_data.white_board = true;
				white_board_state = true;
				break;
			}
			if(white_board_state == true)
			{
				info_object.resources_data.white_board = false;
				white_board_state = false;
				break;
			}
		}
		case SET_AC://check for AC
		{
			if(AC_state == false)
			{
				info_object.resources_data.AC = true;
				AC_state = true;
				break;
			}
			if(AC_state == true)
			{
				info_object.resources_data.AC = false;
				AC_state = false;
				break;
			}
		}
		case SET_PROJECTOR://check for projector
		{
			if(projector_state == false)
			{
				info_object.resources_data.projector = true;
				projector_state = true;
				projector_panel->enable();
				lcd_panel->enable();
				break;
			}
			if(projector_state == true)
			{
				info_object.resources_data.projector = false;
				projector_state = false;
				projector_panel->disable();
				lcd_panel->disable();
				break;
			}
		}
		case SET_PROJECTOR_AUDIO://check for projector with audio or not
		{
			if(projector_with_audio_state == false && projector_state == true)
			{
				info_object.resources_data.proj_with_audio = true;
				projector_with_audio_state = true;
				break;
			}
			if(projector_with_audio_state == true && projector_state == true)
			{
				info_object.resources_data.proj_with_audio = false;
				projector_with_audio_state = false;
				break;
			}
		}
		case SET_PROJECTOR_LCD://check for projector with lcd or not
		{
			if(projector_with_LCD_state == true && projector_state == true)
			{
				info_object.resources_data.LCD_Display = false;
				projector_with_LCD_state = false;
				break;
			}
			if(projector_with_LCD_state == false && projector_state == true)
			{
				info_object.resources_data.LCD_Display = true;
				projector_with_LCD_state = true;
				break;
			}
		}
		case SET_SOUND_SYSTEM://check for sound system
		{
			if(sound_system_state == false)
			{
				info_object.resources_data.sound_system = true;
				sound_system_state = true;
				sound_system_panel->enable();
				break;
			}
			if(sound_system_state == true)
			{
				info_object.resources_data.sound_system = false;
				sound_system_state = false;
				sound_system_panel->disable();
				break;
			}
		}
		case SET_AUDIENCE_SOUND://check for sound system for audience
		{
			if(sound_system_state == true && sound_system_audience_state == true)
			{
				info_object.resources_data.microphones = false;
				sound_system_audience_state = false;
				break;
			}
			if(sound_system_state == true && sound_system_audience_state == false)
			{
				info_object.resources_data.microphones = true;
				sound_system_audience_state = true;
				break;
			}
		}
		case SET_VIDEO_RECORDING://check for video system
		{
			if(video_recording_state == false)
			{
				info_object.resources_data.video_recording = true;
				video_recording_state = true;
				video_system_panel->enable();
				break;
			}
			if(video_recording_state == true)
			{
				info_object.resources_data.video_recording = false;
				video_recording_state = false;
				video_system_panel->disable();
				break;
			}
		}
		case SET_VIDEO_AUDIENCE://check for video system for audience
		{
			if(video_recording_state == true && video_recording_audience_state == true)
			{
				info_object.resources_data.microphones = false;
				video_recording_audience_state = false;
				break;
			}
			if(video_recording_state == true && video_recording_audience_state == false)
			{
				info_object.resources_data.microphones = true;
				video_recording_audience_state = true;
				break;
			}
		}
		case SET_AUDIO_STATE://check for audio system
		{
			if(audio_recording_state == false)
			{
				info_object.resources_data.audio_recording = true;
				audio_recording_state = true;
				audio_panel->enable();
				break;
			}
			if(audio_recording_state == true)
			{
				info_object.resources_data.audio_recording = false;
				audio_recording_state = false;
				audio_panel->disable();
				break;
			}
		}
		case SET_AUDIO_AUDIENCE://check for audio system for audience
		{
			if(audio_recording_state == true && audio_recording_audience_state == false)
			{
				info_object.resources_data.microphones = true;
				audio_recording_audience_state=true;
				break;
			}
			if(audio_recording_state == true && audio_recording_audience_state == true )
			{
				info_object.resources_data.microphones = false;
				audio_recording_audience_state=false;
				break;
			}
		}
		case SET_DATE:	// sets the day of event
		{
			info_object.timeline_init.day_num = date_box->get_int_val();
			break;
		}
		case SET_MONTH:	// sets the month of event
		{
			info_object.timeline_init.month_num = month_box->get_int_val();
			break;
		}
		case SET_YEAR:	// sets the year of the event 
		{
			info_object.timeline_init.year_num = year_box->get_int_val();
			break;
		}
		case SET_START_TIME_HOURS:	//sets the start time hours
		{
			info_object.timeline_init.time_hours = time_hours_box->get_int_val();
			break;
		}
		case SET_START_TIME_MINS:	// sets the start time mins
		{
			info_object.timeline_init.time_minutes = time_minutes_box->get_int_val();
			break;
		}
		case SET_DURATION_HOURS:	// sets the duration time hours
		{
			info_object.duration_hours = duration_hours_box->get_int_val();
			break;
		}
		case SET_DURATION_MINS:		// sets the duration time mins
		{
			info_object.duration_minutes = duration_minutes_box->get_int_val();
			break;
		}
		case SET_CAPACITY:		// sets the capacity
		{
			info_object.resources_data.capacity_room = capacity_box->get_int_val();
			break;
		}
		case SET_SUBMIT:		// sets the submit button parameters
		{
			end_time = final_time_calculator();
			alot_final_time(end_time);
			bool date_check = check_calender_entry(info_object.timeline_fin.day_num, info_object.timeline_fin.month_num, info_object.timeline_fin.year_num);
			bool capacity_check = check_capacity(info_object.resources_data.capacity_room);

			if( (date_check==true) && (capacity_check==true) )
			{
				send_server(TYPICAL_BOOK);		// successfully send the entry to server
				send_server(BOOK_DATA);
				
			}
			if( (date_check == false) && (capacity_check == true) )
			{
				invalid_calender_entry();		// invalid date entered
			}
			if( (date_check == true) && (capacity_check == false) )
			{
				invalid_capacity();			// invalid capacity entered
			}
			if( (date_check == false) && (capacity_check == false) )
			{
				invalid_capacity();			// invalid capacity as well as date entered
				invalid_calender_entry();
			}
			break;
		}
		case SET_USER_ID:
		{
			user_id = user_id_box->get_int_val(); 		// request ID
			break;
		}
		case SET_NEW_USER:
		{
			send_server(NOT_REG_USER);
			struct ser_client_trans_header * recv_data = (struct ser_client_trans_header *)recv_server(NOT_REG_USER);
			user_id = (*recv_data).user_id;
			sprintf(ID_string,"YOUR USER ID is:%d",user_id);
			init_form_request();				//code for new user.
			break;
		}
		case SET_OLD_USER:
		{
			init_checkstatus_menu();			// code for exising user ID sending.
			break;
		}
		case SET_CALENDER_ENTRY:
		{
			invalid_window->close();			// closes the invalid entry window
			break;
		}
		case SET_CAPACITY_ENTRY:
		{
			invalid_capacity_window->close();		// closes the invalid capacity window
			break;

		}
		case SET_FORM_REQ:
		{
			init_form();					// request for status form
			break;
		}
		case SET_REQUEST_ID:
		{
		 	request_id_input = request_id_box->get_int_val();
		 	break;
		}
		case SET_CANCEL_STATUS:
		{
			if(cancel_status == true)
			cancel_status = false;
			else cancel_status = true;
			break;
		}
		case SET_OLD_REQUEST:
		{
			if(cancel_status == true)
			{
				struct ser_client_trans_header object;
				object.data_code = CANCEL;
				int check = send(socketfd, &object, sizeof(struct ser_client_trans_header), ERROR_CHECK);
				if(check < ERROR_CHECK)
				{
					perror("cancel_status failure");
					exit(ERROR_CHECK);
				}
				else
				{
					int check = send( socketfd, &request_id_input, sizeof(int),ERROR_CHECK);
					if(check < ERROR_CHECK)
					{
						perror("cancel_status failure");
						exit(ERROR_CHECK);
					}
					else
					{
						int req_id_valid;
						check = recv( socketfd, &req_id_valid, sizeof(int), ERROR_CHECK);
						if(check < ERROR_CHECK)
						{
							perror("request_valid failure");
							exit(ERROR_CHECK);
						}
						else
						{
							if(req_id_valid == REQ_ID_VALID)
							glui_request_canceled(request_id_input);
							else
							{
								glui_invalid_request_id();
							}
						}
					}
				}
			}
			else
			{
				struct ser_client_trans_header object;
				object.data_code = REQ_STATUS;
				int check = send(socketfd, &object, sizeof(struct ser_client_trans_header), ERROR_CHECK);
				if(check < ERROR_CHECK)
				{
					perror("!cancel_status failure");
					exit(ERROR_CHECK);
				}
				else
				{
					int check = send( socketfd, &request_id_input, sizeof(int), ERROR_CHECK);
					if(check < ERROR_CHECK)
					{
						perror("!cancel_status failure");
						exit(ERROR_CHECK);
					}
					else
					{
						int req_id_valid;
						check = recv( socketfd, &req_id_valid, sizeof(int), ERROR_CHECK);
						if(check < ERROR_CHECK)
						{
							perror("!request_valid failure");
							exit(ERROR_CHECK);
						}
						else
						{
							if(req_id_valid == REQ_ID_VALID)
							{
								int req_status_code;
								check = recv( socketfd, &req_status_code, sizeof(int), ERROR_CHECK);
								if(check < ERROR_CHECK)
								{
									perror("!request_valid failure");
									exit(ERROR_CHECK);
								}
								glui_ask_request_status(req_status_code);
							}
				 		}
					}
				}
			}
			break;
		}
		case SET_SUBMIT_HIGH_PRIOR:
		{
			end_time = final_time_calculator();
			alot_final_time(end_time);
			bool date_check = check_calender_entry(info_object.timeline_fin.day_num, info_object.timeline_fin.month_num, info_object.timeline_fin.year_num);
			bool capacity_check = check_capacity(info_object.resources_data.capacity_room);

			if( (date_check==true) && (capacity_check==true) )
			{
				send_server(HIGH_PRIOR_BOOK);
				send_server(BOOK_DATA);
			}
			if( (date_check == false) && (capacity_check == true) )
			{
				invalid_calender_entry();
			}
			if( (date_check == true) && (capacity_check == false) )
			{
				invalid_capacity();
			}
			if( (date_check == false) && (capacity_check == false) )
			{
				invalid_capacity();
				invalid_calender_entry();
			}
			break;
		}
		case SET_CHECK_USER_ID:
		{
			struct ser_client_trans_header object;
			object.user_id = user_id;
			object.data_code = REG_USER;
			int check = send(socketfd, &object, sizeof(struct ser_client_trans_header), ERROR_CHECK);
			if(check < ERROR_CHECK)
			{
				perror("sending failed: case 31");
				exit(ERROR_CHECK);
			}

			check = recv(socketfd, &object, sizeof(struct ser_client_trans_header), ERROR_CHECK);
			if(check < ERROR_CHECK)
			{
				perror("sending failed: case 31");
				exit(ERROR_CHECK);
			}
			if(object.data_code == USER_ID_EXIST)
			{
				init_form_request();
			}
			else
			{
				glui_invalid_user_id();
			}
			break;
		}
	}
}
