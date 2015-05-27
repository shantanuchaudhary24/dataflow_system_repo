#include "../inc/global_header.h"
#include "../inc/admin_functions_header.h"
int main(int argc, char**argv)
{
	glutInit( &argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_SIZE_X,WINDOW_POSITION_Y);
	glutInitWindowPosition(WINDOW_POSITION_X,WINDOW_POSITION_Y);
	GLUI_Master.set_glutIdleFunc(NULL);
	init_menu();
	glutMainLoop();
	return 1;
}
