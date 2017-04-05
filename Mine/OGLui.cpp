/*	Implementation File for OGLui
	OGLui implementing:
		OGLGraph	by Gabriel Wong
		wiiuse		by Michael Laforest

	Modified by for personal use by:
		Thomas Lee
	Copyright (c) 2015 Thomas Lee. All rights reserved.
	June 2015
*/

#include "OGLui.h"

ofstream file;
ifstream settings;
string name;
int printOn = 0;
int timeCount = 0; // counter or timeGap
int timeGap = 1; // time inbetween data prints 
int printCount = 0;
int prints = 10; // number of prints

OGLGraph* myGraph;
GLint WindowID1;  

//////////////////////////////////////////////////////////
/////////////////       OGLGraph       //////////////////
////////////////////////////////////////////////////////
extern float X, Y;
extern float Yaw, Pitch, Roll;
extern float OcX, OcY, OcZ;

// returns the current date as a String
std::string now( const char* format = "%a %Y-%m-%d %H %M %S" )
{
    std::time_t t = std::time(0) ;
    char cstr[128] ;
    std::strftime( cstr, sizeof(cstr), format, std::localtime(&t) ) ;
    return cstr ;
}

void init ( GLvoid )     // Create Some Everyday Functions
{
	glClearColor(135.0f, 206.0f, 250.0f, 0.f);				// Black Background 135-206-250
	myGraph = OGLGraph::Instance();
	int cache = 480;
	int xOffset = 20;
	int yOffset = 20;
	int width = 1000;
	int height = 1000;
	float xScale = (width - 2 * xOffset)/cache;
	int yScale = 2;
	myGraph->setup( width, height, xOffset, yOffset, xScale, yScale, 1, cache );
}

void display ()   // Create The Display Function
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float) 800 / (float) 600, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	// Timestamp functions
	static int prevTime = glutGet(GLUT_ELAPSED_TIME);
	int time = glutGet(GLUT_ELAPSED_TIME) ;
	int delta = time - prevTime;
	int check = 0;
	if (delta > 1000)
	{
		prevTime = time;
		check = 1;
		//cout << "CHECK" << endl;
		timeCount++;
		// print to file if timeCount is freater than the desired gap
		if(timeCount >= timeGap)
		{
			if(printOn)
			{
				// print into file X Y Yaw Pitch Roll OcX OcY OcZ
				file << X << "\t" << Y << "\t" << Yaw << "\t" << Pitch << "\t" << Roll << "\t" << OcX << "\t" << OcY << "\t" << OcZ << endl;
				printCount++;
				if (printCount >= prints)
				{
					timeCount = 0;
					printCount = 0;
					printOn = 0;
					file.close();
					cout << "Print Ended" << endl;
				}
			}
		}

	}
	myGraph->update( X, Y, Yaw, Pitch, Roll, OcX, OcY, OcZ, check);
	myGraph->draw();

	glutSwapBuffers ( );
}

void reshape ( int w, int h )   // Create The Reshape Function (the viewport)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float) w / (float) h, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
}

void keyboard ( unsigned char key, int x, int y )  // Create Keyboard Function
{
	switch ( key ) 
	{
	case 27:        // When Escape Is Pressed...
		exit ( 0 );   // Exit The Program
		break;        // Ready For Next Case
	case 113:		// q 1st Test
		myGraph->setCoP(1);
		break;
	case 119:		// w 2nd Test
		myGraph->setCoP(2);
		break;
	case 101:		// e 3rd Test
		myGraph->setCoP(3);
		break;
	case 114:		// r 4th Test
		myGraph->setCoP(4);
		break;
	case 116:		// t 5th Test
		myGraph->setCoP(5);
		break;
	case 121:		// y 6th Test
		myGraph->setCoP(6);
		break;
	case 49:		// When 1 Is Pressed change drawMode to 1
		myGraph->setDrawMode(1);
		break;
	case 50:		// 2
		myGraph->setDrawMode(2);
		break;
	case 51:		// 3
		myGraph->setDrawMode(3);
		break;
	case 52:		// 4
		myGraph->setDrawMode(4);
		break;
	case 53:		// 5
		myGraph->setDrawMode(5);
		break;
	case 54:		// 6
		myGraph->setDrawMode(6);
		break;
	case 55:		// 7
		myGraph->setDrawMode(7);
		break;
	case 112:		// p
		if (!printOn)
		{
			name = now() + ".txt";
			//name  = "needaname.txt";
			file.open(name);
			cout << name << "\n" << "Print Started Holds "<< prints <<" data points in " << timeGap * prints << " seconds" << endl;
			printOn = 1;
		}
		break;
	default:        // Now Wrap It Up
		break;
	}
}

/*
time_t currentTime;
  struct tm *localTime;

  time( &currentTime );                   // Get the current time
  localTime = localtime( &currentTime );  // Convert the current time to the local time

  int Day    = localTime->tm_mday;
  int Month  = localTime->tm_mon + 1;
  int Year   = localTime->tm_year + 1900;
  int Hour   = localTime->tm_hour;
  int Min    = localTime->tm_min;
  int Sec    = localTime->tm_sec;

  std::cout << "This program was exectued at: " << Hour << ":" << Min << ":" << Sec << std::endl;
  std::cout << "And the current date is: " << Day << "/" << Month << "/" << Year << std::endl;
  return 0;
  */

void idle(void)
{
  glutPostRedisplay();
}

void OGL(int argc, char** argv)
{
	settings.open("settings.txt", std::ofstream::in);
	if(settings.is_open())
	{
		cout << "settings.txt File Read" << endl;
		string line;
		int change;
		// get & set time increments
		getline(settings,line);
		change = stoi(line.substr(16));
		timeGap = change;
		// get & set number of prints
		getline(settings,line);
		change = stoi(line.substr(16));
		prints = change;
		settings.close();
	}
	glutInit( &argc, argv ); // Erm Just Write It =)
	init();
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE ); // Display Mode
	glutInitWindowSize( 750, 750 ); // If glutFullScreen wasn't called this is the window size
	glutInitWindowPosition(50,50); // Set Starting position
	WindowID1 = glutCreateWindow( "Data Display" ); // Window Title (argv[0] for current directory as title)
	glutDisplayFunc( display );  // Matching Earlier Functions To Their Counterparts
	glutReshapeFunc( reshape );
	glutKeyboardFunc( keyboard );
	glutIdleFunc(idle);
	glutMainLoop( );          // Initialize The Main Loop
}




