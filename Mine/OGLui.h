/*	Header File for OGLui
	OGLui implementing:
		OGLGraph	by Gabriel Wong
		wiiuse		by Michael Laforest

	Modified by for personal use by:
		Thomas Lee
	Copyright (c) 2015 Thomas Lee. All rights reserved.
	June 2015
*/
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>

#include <math.h>		// Math functions
#include "glut.h"   // The GL Utility Toolkit (Glut) Header
#include "OGLGraph.h"

#define PI 3.14159265

using namespace std; 

//////////////////////////////////////////////////////////
/////////////////       OGLGraph       //////////////////
////////////////////////////////////////////////////////


void init ( GLvoid );

void display ( void );

void display2();

void reshape ( int w, int h );

void keyboard ( unsigned char key, int x, int y);

void idle(void);


void OGL(int argc, char** argv);