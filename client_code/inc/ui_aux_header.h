#ifndef UI_AUX_HEADER
#define UI_AUX_HEADER
bool check_calender_entry(int date, int month, int year);
bool check_capacity(int capacity);
void alot_final_time(class time final_time);
void init_menu();
void init_default();
void init_form_request();
void invalid_capacity();
void invalid_calender_entry();
void glui_accepted_request(int request_var);
void glui_invalid_user_id();
void glui_invalid_request_id();
void glui_request_canceled(int variable);
void glui_old_request();
void glui_ask_request_status(int variable);
void glui_not_accepted();
class time final_time_calculator();
#endif
