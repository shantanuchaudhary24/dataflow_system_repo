#include "../../inc/global_header.h"
#include "../../inc/main_header.h"
//######################################### main function ###############################################################
int main(int argc,char **argv)
{
	glutInit( &argc,argv);			// call to all glui functions for initiating the program and menus
	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_SIZE_X,WINDOW_POSITION_Y);
	glutInitWindowPosition(WINDOW_POSITION_X,WINDOW_POSITION_Y);
	GLUI_Master.set_glutIdleFunc(NULL);
	init_menu();
	init_Client();
	glutMainLoop();
	return 0;
}

