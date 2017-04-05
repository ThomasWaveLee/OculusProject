/*	Source.cpp
	Used to run OGLui and OculusData at the same time.
	Written by:
		Thomas Lee
	Copyright (c) 2015 Thomas Lee. All rights reserved.

*/

#include "Main.h"
//#include <Windows.h>

#define PI 3.14159265
#define DEBUG

// universal globals for ease of data collection
float X, Y;
float Yaw, Pitch, Roll;
float OcX, OcY, OcZ;
int EXIT = 0;

void main ( int argc, char** argv )   // Create Main Function For Bringing It All Together
{

	#ifndef DEBUG
	ShowWindow( GetConsoleWindow(), SW_HIDE);
	#endif

	X = 0, Y = 0;
	Yaw = 0, Pitch = 0, Roll = 0;
	OcX = 0, OcY = 0, OcZ = 0;
	float *xP = &X, *yP = &Y;
	OculusPasser passer;
	passer.OcXP = &OcX;
	passer.OcYP = &OcY;
	passer.OcZP = &OcZ;
	passer.YawP = &Yaw;
	passer.PitchP = &Pitch;
	passer.RollP = &Roll;
	//float *YawP = &Yaw, *PitchP = &Pitch, *RollP = &Roll;
	//float *OcXP = &OcX, *OcYP = &OcY, *OcZP = &OcZ;
	thread oculusTracker(OculusDataTracker, passer);
	//thread oculusAngles(OculusDataAngles, YawP, PitchP, RollP);
	thread wii(run, xP, yP);
	thread graph(OGL, argc, argv);
	wii.join();
	//oculusAngles.join();
	oculusTracker.join();
	graph.join();
}