#include "../../inc/global_header.h"
#include "../../inc/main_header.h"

extern GLUI *invalid_window; 
extern GLUI *invalid_capacity_window;
extern GLUI_EditText *request_id_box;
extern struct book_info info_object;
extern int socketfd;
extern char ID_string[ARRAY_SIZE];
extern int request_id_input;
//############################################ Menu window functions ###############################################
void init_menu()
{
	GLUI_Master.close_all();						// Closes the previous windows
	GLUI *glui = GLUI_Master.create_glui( "RMS v1.0");
	glui->add_column(true);							// Adds a column separator
  	glui->add_statictext("WELCOME!");
  	glui->add_separator();							// Adds horizontal separator
  	glui->add_button("NEW USER",SET_NEW_USER,(GLUI_Update_CB)switcher_function);	// Adds a button 
  	glui->add_separator();
  	glui->add_button("OLD USER",SET_OLD_USER, (GLUI_Update_CB)switcher_function);
  	glui->add_separator();
  	glui->add_button("Quit",ERROR_CHECK,(GLUI_Update_CB)exit);
  	glui->add_separator();
  	glui->add_column(true);
}


void glui_ask_request_status(int variable)
{
	GLUI_Master.close_all();
	GLUI *glui_window = GLUI_Master.create_glui( "RMS v1.0");		//Creates new window
	char variable_store[ARRAY_SIZE];
	sprintf(variable_store,"REQUEST ID:%d",request_id_input);		// Used to display a variable as static text in the dialog box
	glui_window->add_statictext(variable_store);
	glui_window->add_statictext(ID_string);					//displaying the text on the window
	glui_window->add_statictext("YOUR REQUEST ID STATUS:");
	if( variable == PENDING_CODE)
	{
		glui_window->add_statictext("PENDING");
	}
	else if(variable == APPROVED_CODE)
	{
		 glui_window->add_statictext("APPROVED");			// depending upon the input variable status is printed
	}
	else
	{
		 glui_window->add_statictext("CANCELLED");
	}
	glui_window-> add_button( "CONTINUE",INITIALIZED,(GLUI_Update_CB)glui_old_request);
	glui_window-> add_button( "EXIT",ERROR_CHECK,(GLUI_Update_CB)exit);		// continue and exit button
}




void glui_invalid_request_id()
{
	GLUI_Master.close_all();
	GLUI *glui_window = GLUI_Master.create_glui( "RMS v1.0");
	glui_window->add_statictext(ID_string);
	glui_window->add_statictext("YOUR REQUEST ID IS INVALID");
	glui_window-> add_button( "CONTINUE",INITIALIZED,(GLUI_Update_CB)glui_old_request);
	glui_window-> add_button( "EXIT",ERROR_CHECK,(GLUI_Update_CB)exit);
}

void glui_request_canceled(int variable)
{
	GLUI_Master.close_all();
	GLUI *glui_window = GLUI_Master.create_glui( "RMS v1.0");
	char variable_store[ARRAY_SIZE];
	glui_window->add_statictext(ID_string);
	sprintf(variable_store,"REQUEST ID:%d",variable);
	glui_window->add_statictext(variable_store);
	glui_window->add_statictext("CANCELLED");
	glui_window-> add_button( "CONTINUE",INITIALIZED,(GLUI_Update_CB)init_form_request);
	glui_window-> add_button( "EXIT",ERROR_CHECK,(GLUI_Update_CB)exit);
}

void init_form_request()
{
	GLUI_Master.close_all();
	GLUI *glui_window = GLUI_Master.create_glui( "RMS v1.0");
	glui_window->add_statictext(ID_string);
	glui_window-> add_button( "BOOK NEW REQUEST",SET_FORM_REQ,(GLUI_Update_CB)switcher_function);
	glui_window-> add_button( "CHECK OLD REQUEST",INITIALIZED,(GLUI_Update_CB)glui_old_request);
}


void glui_old_request()
{
	init_global_cancel();
	GLUI_Master.close_all();
	GLUI *glui_window = GLUI_Master.create_glui( "RMS v1.0");
	glui_window->add_statictext(ID_string);
	request_id_box = glui_window-> add_edittext( "REQUEST ID", GLUI_EDITTEXT_INT, NULL, SET_REQUEST_ID,(GLUI_Update_CB)switcher_function);
	GLUI_RadioGroup *form_request_radio_group;
	form_request_radio_group = glui_window->add_radiogroup(NULL,SET_CANCEL_STATUS,switcher_function);
	glui_window->add_radiobutton_to_group(form_request_radio_group,"CANCEL");
	glui_window->add_radiobutton_to_group(form_request_radio_group,"CHECK STATUS");
	glui_window-> add_button( "OK",SET_OLD_REQUEST,(GLUI_Update_CB)switcher_function);
}





bool check_capacity(int capacity)				// checks for invalid entry in capacity field
{
	if( (capacity < 1) || (capacity > MAX_ROOM_SIZE) )
	return false;						// error if entry is less than 1 and greater than the max capacity of room
	if((capacity > 1) && (capacity < MAX_ROOM_SIZE))
	return true;						// else cases return true
	else if(capacity == MAX_ROOM_SIZE)
	return true;
	else return false;
}



void glui_invalid_user_id()					// Window that displays invalid user message
{
	GLUI_Master.close_all();
	GLUI *glui_window = GLUI_Master.create_glui( "RMS v1.0");
	glui_window->add_statictext("THIS USER ID DOES NOT EXIST");
	glui_window->add_button("GO BACK",ERROR_CHECK,(GLUI_Update_CB)init_menu);
}


class time final_time_calculator() 				// function for calculating the final time and getting current system date
{
	class time final_time;
	int calculated_hours = info_object.timeline_init.time_hours + info_object.duration_hours;
	int calculated_minutes = info_object.timeline_init.time_minutes + info_object.duration_minutes;
	if(calculated_minutes >= MINUTE)
	{
		calculated_hours++;
		calculated_minutes = calculated_minutes % MINUTE;
	}
	final_time.hours = calculated_hours;
	final_time.minutes = calculated_minutes;
	return final_time;
}


void invalid_calender_entry()					// checks that the user doesn't write any invalid date 
{
	invalid_window = GLUI_Master.create_glui( "ERROR");
	invalid_window->add_column(true);
  	invalid_window->add_separator();
  	invalid_window->add_statictext("INVALID BOOKING DATE ENTRY!");
  	invalid_window->add_button( "RETRY",SET_CALENDER_ENTRY,(GLUI_Update_CB)switcher_function);
  	invalid_window->add_button( "QUIT",ERROR_CHECK,(GLUI_Update_CB)exit);
  	invalid_window->add_separator();
  	invalid_window->add_column(true);
}

void invalid_capacity()						// checks for invalid capacity
{
	invalid_capacity_window = GLUI_Master.create_glui( "ERROR");
	invalid_capacity_window->add_column(true);
  	invalid_capacity_window->add_separator();
  	invalid_capacity_window->add_statictext("INVALID CAPACITY");
  	invalid_capacity_window->add_button( "RETRY",SET_CAPACITY_ENTRY,(GLUI_Update_CB)switcher_function);
  	invalid_capacity_window->add_button( "QUIT",ERROR_CHECK,(GLUI_Update_CB)exit);
  	invalid_capacity_window->add_separator();
  	invalid_capacity_window->add_column(true);
}


void alot_final_time(class time end_time)			// calculates ending time of event based upon the start time and duration given
{
	info_object.timeline_fin.day_num = info_object.timeline_init.day_num ;
	info_object.timeline_fin.month_num = info_object.timeline_init.month_num ;
	info_object.timeline_fin.year_num = info_object.timeline_init.year_num ;
	info_object.timeline_fin.time_hours = end_time.hours ;
	info_object.timeline_fin.time_minutes = end_time.minutes ;
	printf("%d:%d\n",info_object.timeline_fin.time_hours,info_object.timeline_fin.time_minutes);
}

bool check_calender_entry(int date, int month, int year)	// checks calender entry by taking input as day, month and year
{
	struct tm *Sys_Time = NULL;
  	time_t Time_val = INITIALIZED;
    	Time_val = time(NULL);
    	Sys_Time = localtime(&Time_val);
	int current_year = DEFAULT_YEAR + Sys_Time->tm_year;
	int current_month = Sys_Time->tm_mon+1;
	int current_date = Sys_Time->tm_mday;
	if( (date > 31) || (month > 12) || (year < current_year) || (date <= 0) || (month <= 0) )
	return false;
	else if((month == 2 || month == 4 || month == 6 || month == 9 || month == 11 ) && (date >= 31 ))
	return false;
	else if( (month == 2) && (date >= 29))
	return false;
	else if( (month < current_month) && (date < current_date)  && (year == current_year))
	return false;
	else return true;
}

void glui_accepted_request(int request_var)
{
	char text[ARRAY_SIZE];
	sprintf(text, "REQUEST ID IS: %d",request_var);
	GLUI_Master.close_all();
	GLUI *glui_window = GLUI_Master.create_glui( "RMS v1.0");
	glui_window->add_statictext("REQUEST ACCEPTED");
	glui_window->add_statictext(text);
	glui_window->add_button( "CONTINUE",ERROR_CHECK,(GLUI_Update_CB)init_form_request);
	glui_window->add_button( "QUIT",ERROR_CHECK,(GLUI_Update_CB)exit);
}

void glui_not_accepted()
{
	GLUI_Master.close_all();
	GLUI *glui_window = GLUI_Master.create_glui( "RMS v1.0");
	glui_window->add_statictext("REQUEST NOT ACCEPTED");
	glui_window->add_button( "CONTINUE",ERROR_CHECK,(GLUI_Update_CB)init_form_request);
	glui_window->add_button( "QUIT",ERROR_CHECK,(GLUI_Update_CB)exit);

}

void init_default()						// initialises the default fields of the object
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

