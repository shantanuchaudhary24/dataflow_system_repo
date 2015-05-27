#include "../inc/global_header.h"
#include "../inc/admin_functions_header.h"
#include "../inc/aux_functions.h"

bool check_capacity(int capacity);
void query_entry(int value);
void invalid_capacity();
void init_default();
void init_menu();
void switcher_function(int value);
void init_form();
void general_settings();
struct general_pool pool_object;
struct book_info info_object;
GLUI_Panel *projector_panel, *sound_system_panel, *video_system_panel, *audio_panel, *lcd_panel ;
GLUI_EditText *capacity_box; GLUI *invalid_window, *invalid_capacity_window; 
GLUI_EditText *time_hours_box, *time_minutes_box, *duration_hours_box, *duration_minutes_box, *num_operator_box, *num_mics_box, *num_rooms_box;

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

void init_reinitialize()
{
	white_board_state		= false;
	AC_state 			= false;
	projector_state			= false;
	sound_system_state 		= false;
	audio_recording_state 		= false;
	video_recording_state 		= false;
	projector_with_LCD_state 	= true;
	projector_with_audio_state 	= true;
	sound_system_audience_state 	= true;
	audio_recording_audience_state 	= true;
	video_recording_audience_state 	= true;
}



bool check_capacity(int capacity)
{
	if( (capacity < 1) || (capacity > MAX_ROOM_SIZE) )
	return false;
	if((capacity > 1) && (capacity < MAX_ROOM_SIZE)) 
	return true;
	else if(capacity == MAX_ROOM_SIZE) 
	return true;
	else return false;
}


void invalid_capacity()
{
	invalid_capacity_window = GLUI_Master.create_glui( "ERROR");
	invalid_capacity_window->add_column(true);
  	invalid_capacity_window->add_separator();
  	invalid_capacity_window->add_statictext("INVALID CAPACITY");
  	invalid_capacity_window->add_button( "RETRY",SET_INVALID_CAPACITY,(GLUI_Update_CB)switcher_function);
  	invalid_capacity_window->add_button( "QUIT",ERROR_CHECK,(GLUI_Update_CB)exit);
  	invalid_capacity_window->add_separator();
  	invalid_capacity_window->add_column(true);
}


void init_default()
{
	info_object.resources_data.white_board 		= false;
	info_object.resources_data.AC 			= false;
	info_object.resources_data.projector 		= false;
	info_object.resources_data.sound_system 	= false;
	info_object.resources_data.microphones 		= false;
	info_object.resources_data.sound_system 	= false;
	info_object.resources_data.video_recording 	= false;
	info_object.resources_data.LCD_Display	 	= true;
	info_object.resources_data.proj_with_audio 	= true;
	info_object.resources_data.capacity_room	= INITIALIZED;
	
	info_object.timeline_init.day_num		= INITIALIZED;
	info_object.timeline_init.month_num		= INITIALIZED;
	info_object.timeline_init.year_num		= INITIALIZED;
	info_object.timeline_init.time_hours		= INITIALIZED;
	info_object.timeline_init.time_minutes		= INITIALIZED;
	
	info_object.timeline_fin.day_num		= INITIALIZED;
	info_object.timeline_fin.month_num		= INITIALIZED;
	info_object.timeline_fin.year_num		= INITIALIZED;
	info_object.timeline_fin.time_hours		= INITIALIZED;
	info_object.timeline_fin.time_minutes		= INITIALIZED;
}

void init_menu()
{
	GLUI_Master.close_all();
	GLUI *glui = GLUI_Master.create_glui( "RMS v1.0 ADMIN CONSOLE");
	glui->add_column(true);
  	glui->add_statictext("WELCOME!");
  	glui->add_separator();
  	glui->add_button("ADD NEW ROOM",ERROR_CHECK,(GLUI_Update_CB)init_form);
  	glui->add_separator();
  	glui->add_button("SET GENERAL SETTINGS:",ERROR_CHECK, (GLUI_Update_CB)general_settings);
  	glui->add_separator();
  	glui->add_button("QUIT",ERROR_CHECK,(GLUI_Update_CB)exit);
  	glui->add_separator();
  	glui->add_column(true);
}

void switcher_function(int value)
{
	switch(value)
		{
		case SET_WHITE_BOARD:// check for white_board
		{	if(white_board_state == false)
			{
				info_object.resources_data.white_board = true;
				white_board_state = true;
				printf("white_board true hua\n");
				break;
			}
			if(white_board_state == true)
			{
				info_object.resources_data.white_board = false;
				white_board_state = false;
				printf("white_board false hua\n");
				break;
			}
		}	
		case SET_AC://check for AC
		{	
			if(AC_state == false)
			{
				info_object.resources_data.AC = true;
				AC_state = true;
				printf("AC true hua\n");
				break;
			}
			if(AC_state == true)
			{
				info_object.resources_data.AC = false;
				AC_state = false;
				printf("AC false hua\n");
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
				printf("projector true hua\n");
				break;
			}
			if(projector_state == true)
			{
				info_object.resources_data.projector = false;
				projector_state = false;
				projector_panel->disable();
				lcd_panel->disable();
				printf("projector false hua\n");
				break;
			}
		}	
		case SET_PROJECTOR_AUDIO://check for projector with audio or not
		{
			if(projector_with_audio_state == false && projector_state == true)
			{
				info_object.resources_data.proj_with_audio = true;
				projector_with_audio_state = true;
				printf("projector with audio true hua\n");
				break;
			}
			if(projector_with_audio_state == true && projector_state == true)
			{
				info_object.resources_data.proj_with_audio = false;
				projector_with_audio_state = false;
				printf("projector with audio false hua\n");
				break;
			}
		}	
		case SET_PROJECTOR_LCD://check for projector with lcd or not
		{
			if(projector_with_LCD_state == true && projector_state == true)
			{
				info_object.resources_data.LCD_Display = false;
				projector_with_LCD_state = false;
				printf("projector_with_LCD false hua\n");
				break;
			}
			if(projector_with_LCD_state == false && projector_state == true)
			{
				info_object.resources_data.LCD_Display = true;
				projector_with_LCD_state = true;
				printf("projector_with_LCD true hua\n");
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
				printf("sound_system true hua\n");
				break;
			}
			if(sound_system_state == true)
			{
				info_object.resources_data.sound_system = false;
				sound_system_state = false;
				sound_system_panel->disable();
				printf("sound_system false hua\n");
				break;
			}
		}	
		case SET_AUDIENCE_SOUND://check for sound system for audience
		{
			if(sound_system_state == true && sound_system_audience_state == true)
			{
				info_object.resources_data.microphones = false;
				sound_system_audience_state = false;
				printf("sound_for_audience false hua\n");
				break;
			}
			if(sound_system_state == true && sound_system_audience_state == false)
			{
				info_object.resources_data.microphones = true;
				sound_system_audience_state = true;
				printf("sound_for_audience true hua\n");
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
				printf("video_recording true hua\n");
				break;
			}
			if(video_recording_state == true)
			{
				info_object.resources_data.video_recording = false;
				video_recording_state = false;
				video_system_panel->disable();
				printf("video_recording false hua\n");	
				break;
			}
		}	
		case SET_VIDEO_AUDIENCE://check for video system for audience
		{
			if(video_recording_state == true && video_recording_audience_state == true)
			{
				info_object.resources_data.microphones = false;
				video_recording_audience_state = false;
				printf("video_recording_audience_state false hua\n");
				break;
			}
			if(video_recording_state == true && video_recording_audience_state == false)
			{
				info_object.resources_data.microphones = true;
				video_recording_audience_state = true;
				printf("video_recording_audience_state true hua\n");
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
				printf("audio_recording true hua\n");
				break;
			}
			if(audio_recording_state == true)
			{
				info_object.resources_data.audio_recording = false;
				audio_recording_state = false;
				audio_panel->disable();
				printf("audio_recording false hua\n");
				break;
			}
		}	
		case SET_AUDIO_AUDIENCE://check for audio system for audience
		{
			if(audio_recording_state == true && audio_recording_audience_state == false)
			{
				info_object.resources_data.microphones = true;
				audio_recording_audience_state=true;
				printf("audio_recording_audience_state true hua\n");
				break;
			}
			if(audio_recording_state == true && audio_recording_audience_state == true ) 
			{
				info_object.resources_data.microphones = false;
				audio_recording_audience_state=false;
				printf("audio_recording_audience_state false hua\n");
				break;
			}
		}
		case SET_CAPACITY:
		{
			info_object.resources_data.capacity_room = capacity_box->get_int_val();
			printf("CAPACITY:%d",info_object.resources_data.capacity_room);
			break;
		}		
		case SET_DATABASE:
		{
			bool capacity_check = check_capacity(info_object.resources_data.capacity_room);
			if(capacity_check == true)
			{
				printf("Setting room configuration...done\n");
				database_writer_config(info_object);
				query_entry(2);
			}
			else
			{
				invalid_capacity();
			}
			break;
		}
		case SET_INVALID_CAPACITY:
		{
			invalid_capacity_window->close();
			break;
		}
		case SET_START_TIME_HOURS:
		{
			pool_object.start_time_hours = time_hours_box->get_int_val();
			printf("Start_hours:%d\n", pool_object.start_time_hours );
			break;
		}
		case SET_START_TIME_MINS:
		{
			pool_object.start_time_mins = time_minutes_box->get_int_val();
			printf("Start_minutes:%d\n", pool_object.start_time_mins );
			break;
		}
		case SET_END_TIME_HOURS:
		{
			pool_object.end_time_hours = duration_hours_box->get_int_val();
			printf("End_hours:%d\n", pool_object.end_time_hours );
			break;
		}
		case SET_END_TIME_MINS:
		{
			pool_object.end_time_hours = duration_minutes_box->get_int_val();
			printf("End_minutes:%d\n", pool_object.end_time_mins );
			break;
		}
		case SET_NUM_OPERATORS:
		{
			pool_object.num_operators = num_operator_box->get_int_val();
			printf("Number Of Operators:%d\n", pool_object.num_operators );
			break;
		}
		case SET_NUM_MICS:
		{
			pool_object.num_microphones = num_mics_box->get_int_val();
			printf("Number Of Mics:%d\n", pool_object.num_microphones );
			break;
		}
		case SET_NUM_ROOMS:
		{
			pool_object.num_rooms = num_rooms_box->get_int_val();
			printf("Number Of Rooms:%d\n", pool_object.num_rooms );
			break;
		}
		case SET_POOL_DATABASE:
		{
			database_writer_pool(pool_object);
			printf("Database Written");
			query_entry(1);
			break;
		}
	}
}

void query_entry(int value)
{
	if(value == 1)
	{
		GLUI_Master.close_all();
		GLUI *glui = GLUI_Master.create_glui( "RMS v1.0 ADMIN CONSOLE");
		glui->add_column(true);
  		glui->add_statictext("WHAT DO YOU WANT TO DO NOW?");
  		glui->add_separator();
  		glui->add_button("CHANGE GENERAL SETTINGS",ERROR_CHECK,(GLUI_Update_CB)general_settings);
  		glui->add_separator();
  		glui->add_button("GO BACK TO MENU",ERROR_CHECK, (GLUI_Update_CB)init_menu);
  		glui->add_separator();
  		glui->add_button("QUIT",ERROR_CHECK,(GLUI_Update_CB)exit);
  		glui->add_separator();
  		glui->add_column(true);
	}
	else
	{
		GLUI_Master.close_all();
		GLUI *glui = GLUI_Master.create_glui( "RMS v1.0 ADMIN CONSOLE");
		glui->add_column(true);
  		glui->add_statictext("WHAT DO YOU WANT TO DO NOW?");
  		glui->add_separator();
  		glui->add_button("ADD ANOTHER ROOM",ERROR_CHECK,(GLUI_Update_CB)init_form);
  		glui->add_separator();
  		glui->add_button("GO BACK TO MENU",ERROR_CHECK, (GLUI_Update_CB)init_menu);
  		glui->add_separator();
  		glui->add_button("QUIT",ERROR_CHECK,(GLUI_Update_CB)exit);
  		glui->add_separator();
  		glui->add_column(true);
	
	}
}

void init_form()
{
	init_reinitialize();
	init_default();
	GLUI_Master.close_all();
	GLUI *glui_window = GLUI_Master.create_glui( "RMS v1.0 ADMIN CONSOLE");
	GLUI_RadioGroup *projector_panel_group, *sound_system_panel_group, *video_system_panel_group, * audio_panel_group, *lcd_panel_group;
	
	glui_window->add_statictext("SELECT ROOM CONFIGURATION:");
	glui_window->add_checkbox("WHITEBOARD",NULL,SET_WHITE_BOARD,switcher_function);
	glui_window->add_checkbox("AIR-CONDITIONING",NULL,SET_AC,switcher_function);
	glui_window->add_checkbox("PROJECTOR",NULL,SET_PROJECTOR,switcher_function);
	projector_panel 	= glui_window->add_panel("CONFIGURATION");
	projector_panel_group 	= glui_window->add_radiogroup_to_panel(projector_panel,NULL,SET_PROJECTOR_AUDIO,switcher_function);
	glui_window->add_radiobutton_to_group(projector_panel_group,"WITH AUDIO");
	glui_window->add_radiobutton_to_group(projector_panel_group,"WITHOUT AUDIO");
	projector_panel->disable();
		
	lcd_panel 		= glui_window->add_panel("CONFIGURATION");
	lcd_panel_group 	= glui_window->add_radiogroup_to_panel(lcd_panel,NULL,SET_PROJECTOR_LCD,switcher_function);
	glui_window->add_radiobutton_to_group(lcd_panel_group,"WITH LCD");
	glui_window->add_radiobutton_to_group(lcd_panel_group,"WITHOUT LCD");
	lcd_panel->disable();
		

	glui_window->add_checkbox("SOUND SYSTEM",NULL,SET_SOUND_SYSTEM,switcher_function);
	sound_system_panel 	= glui_window->add_panel("CONFIGURATION");
	sound_system_panel_group= glui_window->add_radiogroup_to_panel(sound_system_panel,NULL,SET_AUDIENCE_SOUND,switcher_function);
	glui_window->add_radiobutton_to_group(sound_system_panel_group, "FOR AUDIENCE");
	glui_window->add_radiobutton_to_group(sound_system_panel_group, "NOT FOR AUDIENCE"); 
	sound_system_panel->disable();
	
	glui_window->add_column(true);
	glui_window->add_checkbox("VIDEO REC. SYSTEM",NULL,SET_VIDEO_RECORDING,switcher_function);
	video_system_panel 	= glui_window->add_panel("CONFIGURATION");
	video_system_panel_group= glui_window->add_radiogroup_to_panel(video_system_panel,NULL,SET_VIDEO_AUDIENCE,switcher_function);
	glui_window->add_radiobutton_to_group(video_system_panel_group, "FOR AUDIENCE");
	glui_window->add_radiobutton_to_group(video_system_panel_group, "NOT FOR AUDIENCE");
	video_system_panel->disable();
	
	glui_window->add_checkbox("AUDIO REC. SYSTEM",NULL,SET_AUDIO_STATE,switcher_function);
	audio_panel 	= glui_window->add_panel("CONFIGURATION");
	audio_panel_group= glui_window->add_radiogroup_to_panel(audio_panel,NULL,SET_AUDIO_AUDIENCE,switcher_function);
	glui_window->add_radiobutton_to_group(audio_panel_group, "FOR AUDIENCE");
	glui_window->add_radiobutton_to_group(audio_panel_group, "NOT FOR AUDIENCE");
	audio_panel->disable();
			
	capacity_box = glui_window-> add_edittext( "CAPACITY", GLUI_EDITTEXT_INT, NULL, SET_CAPACITY,(GLUI_Update_CB)switcher_function);
	glui_window-> add_button( "ADD TO DATABASE",SET_DATABASE,(GLUI_Update_CB)switcher_function);
	glui_window-> add_button( "GO BACK",ERROR_CHECK,(GLUI_Update_CB)init_menu);
	glui_window-> add_button( "QUIT",ERROR_CHECK,(GLUI_Update_CB)exit);
	
}

void general_settings()
{
	GLUI_Master.close_all();
	GLUI *glui_window = GLUI_Master.create_glui( "RMS v1.0 ADMIN CONSOLE");
	GLUI_Panel *time_slot_panel, *start_time_panel, *duration_panel;
//################################### time slot panel ################################################################	
	time_slot_panel = glui_window->add_panel("WORKING TIME", GLUI_PANEL_EMBOSSED);
	start_time_panel= glui_window-> add_panel_to_panel( time_slot_panel,"START TIME:",GLUI_PANEL_EMBOSSED);
	time_hours_box = glui_window-> add_edittext_to_panel( start_time_panel,"HOURS:", GLUI_EDITTEXT_INT,NULL, SET_START_TIME_HOURS,(GLUI_Update_CB)switcher_function);
	time_minutes_box = glui_window-> add_edittext_to_panel( start_time_panel,"MINS:",GLUI_EDITTEXT_INT, NULL, SET_START_TIME_MINS,(GLUI_Update_CB)switcher_function);
//################################# duration_panel ###################################################################		
	duration_panel  = glui_window-> add_panel_to_panel( time_slot_panel,"END TIME:",GLUI_PANEL_EMBOSSED);
	duration_hours_box = glui_window-> add_edittext_to_panel( duration_panel,"HOURS:", GLUI_EDITTEXT_INT, NULL, SET_END_TIME_HOURS,(GLUI_Update_CB)switcher_function);
	duration_minutes_box = glui_window-> add_edittext_to_panel( duration_panel,"MINS:", GLUI_EDITTEXT_INT, NULL, SET_END_TIME_MINS,(GLUI_Update_CB)switcher_function);

	glui_window->add_column(true);
//################################## operator box, mics, number of rooms ############################################## 	
	num_operator_box = glui_window-> add_edittext("NUMBER OF OPERATORS", GLUI_EDITTEXT_INT, NULL, SET_NUM_OPERATORS,(GLUI_Update_CB)switcher_function);
	num_mics_box = glui_window-> add_edittext("NUMBER OF MICROPHONES", GLUI_EDITTEXT_INT, NULL, SET_NUM_MICS,(GLUI_Update_CB)switcher_function);
	num_rooms_box = glui_window-> add_edittext("NUMBER OF ROOMS", GLUI_EDITTEXT_INT, NULL, SET_NUM_ROOMS,(GLUI_Update_CB)switcher_function);
	glui_window->add_button("ADD TO DATABASE",SET_POOL_DATABASE,(GLUI_Update_CB)switcher_function);
	glui_window->add_button("GO BACK",ERROR_CHECK,(GLUI_Update_CB)init_menu);
	glui_window->add_button("QUIT",ERROR_CHECK,(GLUI_Update_CB)exit);
	
}

