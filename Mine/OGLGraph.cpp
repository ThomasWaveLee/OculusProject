/*
	Implementation of the OGLGraph class functions
	Copyright (C) 2006 Gabriyel Wong (gabriyel@gmail.com)

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

	Modified for personal Use by:
		Thomas Lee
	Copyright (c) 2015 Thomas Lee. All rights reserved.
	June 2015
*/
#include "OGLGraph.h"

OGLGraph* OGLGraph::_instance = 0;
void drawCircle(int, float, float, float, int);

// turns a string into glut character 
void
print_bitmap_string(void* font, char* s)
{
   if (s && strlen(s)) {
      while (*s) {
         glutBitmapCharacter(font, *s);
         s++;
      }
   }
}


OGLGraph* OGLGraph::Instance()
{
	if (_instance == 0) 
	{
		_instance = new OGLGraph();
    }
    return _instance;
}

// creates the Graph w/ given parameters
void
OGLGraph::setup( int width, int height, int offsetX, int offsetY, 
				 float scaleX, int scaleY, int channels, int cacheSize )
{
	_offsetX = offsetX;
	_offsetY = offsetY;
	_width = width;
	_height = height;
	_scaleX = scaleX;
	_scaleY = scaleY;
	_cacheSize = cacheSize;
	zeroAxis = 8*_offsetY;
	setDrawMode(1);
	setCoP(0);
}

void
OGLGraph::update( float X ,float Y, float yaw, float pitch, float roll, float OcX , float OcY, float OcZ , int time)
// data = x, data2 = y
{
	// all data vectors are the same size so can put them all in one loop
	if ( _X.size() > (unsigned int)(_cacheSize))
	{ 
		_X.pop_front();
		_Y.pop_front();
		_timeStamp.pop_front();
		_yawData.pop_front();
		_pitchData.pop_front();
		_rollData.pop_front();	
		_OcXData.pop_front();
		_OcYData.pop_front();
		_OcZData.pop_front();
	}

	// WBB data push backs
	_X.push_back( X );
	_Y.push_back( Y );
	/*
	for (int x = 0; x <=10; ++x)
		{
			if (_drawMode == 1) {
				_dataXUp[x] = _dataX[x] + X*2;
				_dataYUp[x] = _dataY[x] + Y*2;
			}
			else if (_drawMode == 2) {
				_dataXUp[x] = _dataX[x] + X * 7;
				_dataYUp[x] = _dataY[x] + Y * 7;
			}
		}
	*/

	// Oculus position push backs
	_OcXData.push_back(OcX);
	_OcYData.push_back(OcY);
	_OcZData.push_back(OcZ);

	_yawData.push_back( yaw );
	_pitchData.push_back( pitch );
	_rollData.push_back( roll );
	if(time)
		_timeStamp.push_back(10);
	else
		_timeStamp.push_back(0);

	// set up marker references
	_currentX = X;
	_currentY = Y;
	_currentYaw = yaw;
	_currentPitch = pitch;
	_currentRoll = roll;
	_currentOcX = OcX;
	_currentOcY = OcY;
	_currentOcZ = OcZ;
	
	// set up strings for each data input
	sprintf(sWBBX, "%.2f", X);
	sprintf(sWBBY, "%.2f", Y);
	sprintf(sYaw, "%.2f", yaw *180/3.14);
	sprintf(sPitch, "%.2f", pitch*180/3.14);
	sprintf(sRoll, "%.2f", roll*180/3.14);
	sprintf(sOcX, "%.2f", OcX);
	sprintf(sOcY, "%.2f", OcY);
	sprintf(sOcZ, "%.2f", OcX);
	
}

void
OGLGraph::draw()
{
	int cntTime = 0;
	int cntX = 0;
	int cntY = 0;
	int cntYaw = 0, cntPitch = 0, cntRoll = 0;
	// Set up the display
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity(); 
	glOrtho(0, _width, 0, _height, 0, 1.0); 

	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
	glLineWidth(1.5);
	int xStep = 0;

	// WBB Data in Text
	if (_drawMode == 1 || _drawMode == 2 || _drawMode == 3)
	{
		glColor3ub( 255, 255, 0 );	// Yellow
		glRasterPos2f(_offsetX +5,_offsetY +5);
		print_bitmap_string(GLUT_BITMAP_HELVETICA_12, "WBBX:");
		xStep += 5*18;
		glRasterPos2f(_offsetX +5 + xStep,_offsetY +5);
		print_bitmap_string(GLUT_BITMAP_HELVETICA_12, sWBBX);
		xStep += 5*18;

		glColor3ub( 0, 0, 255 );	// Blue
		glRasterPos2f(_offsetX +5 + xStep,_offsetY +5);
		print_bitmap_string(GLUT_BITMAP_HELVETICA_12, "Y:");
		xStep += 2*18;
		glRasterPos2f(_offsetX +5 + xStep,_offsetY +5);
		print_bitmap_string(GLUT_BITMAP_HELVETICA_12, sWBBY);
		xStep += 5*18;
	}
	// Oculus Angles in Text
	if (_drawMode == 1 || _drawMode == 4 || _drawMode == 5)
	{
		glColor3ub( 0, 255, 0 );	// Green
		glRasterPos2f(_offsetX +5 + xStep,_offsetY +5);
		print_bitmap_string(GLUT_BITMAP_HELVETICA_12, "Yaw:");
		xStep += 3.5*18;
		glRasterPos2f(_offsetX +5 + xStep,_offsetY +5);
		print_bitmap_string(GLUT_BITMAP_HELVETICA_12, sYaw);
		xStep += 5*18;

		glColor3ub( 255, 0, 255 );	// Purple
		glRasterPos2f(_offsetX +5+ xStep,_offsetY +5);
		print_bitmap_string(GLUT_BITMAP_HELVETICA_12, "Pitch:");
		xStep += 4*18;
		glRasterPos2f(_offsetX +5 + xStep,_offsetY +5);
		print_bitmap_string(GLUT_BITMAP_HELVETICA_12, sPitch);
		xStep += 5*18;

		glColor3ub( 0, 255, 255 );	// Cyan
		glRasterPos2f(_offsetX +5 + xStep,_offsetY +5);
		print_bitmap_string(GLUT_BITMAP_HELVETICA_12, "Roll:");
		xStep += 3*18;
		glRasterPos2f(_offsetX +5 + xStep,_offsetY +5);
		print_bitmap_string(GLUT_BITMAP_HELVETICA_12, sRoll);
		xStep += 5*18;
	}
	// Oculus Position in Text
	if (_drawMode == 6 || _drawMode == 7)
	{
		glColor3ub( 0, 100, 00 );	// OcX
		glRasterPos2f(_offsetX +5 + xStep,_offsetY +5);
		print_bitmap_string(GLUT_BITMAP_HELVETICA_12, "OcX:");
		xStep += 4*18;
		glRasterPos2f(_offsetX +5 + xStep,_offsetY +5);
		print_bitmap_string(GLUT_BITMAP_HELVETICA_12, sOcX);
		xStep += 5*18;

		glColor3ub( 100, 0, 100 );	// OcY
		glRasterPos2f(_offsetX +5 + xStep,_offsetY +5);
		print_bitmap_string(GLUT_BITMAP_HELVETICA_12, "Y:");
		xStep += 3*18;
		glRasterPos2f(_offsetX +5 + xStep,_offsetY +5);
		print_bitmap_string(GLUT_BITMAP_HELVETICA_12, sOcY);
		xStep += 5*18;

		glColor3ub( 0, 100, 100 );	// OcZ
		glRasterPos2f(_offsetX +5 + xStep,_offsetY +5);
		print_bitmap_string(GLUT_BITMAP_HELVETICA_12, "Z:");
		xStep += 3*18;
		glRasterPos2f(_offsetX +5 + xStep,_offsetY +5);
		print_bitmap_string(GLUT_BITMAP_HELVETICA_12, sOcZ);
	}

	if (_drawMode == 1) // All View
	{
		// Draw the axes - OpenGL screen coordinates start from bottom-left (0,0)
		drawBox();
		// Draw 0-axis
		glVertex3f( _offsetX, zeroAxis, 0 );
		glVertex3f( _width - _offsetX, zeroAxis, 0 );
		//	9.2/10   7.9/10   6.7/10
		// Oculus Angles' Axis
		//	Yaw
		glVertex3f( _offsetX, _height * 9.2/10, 0 );
		glVertex3f( _width - _offsetX, _height * 9.2/10, 0 );
		//	Pitch
		glVertex3f( _offsetX, _height * 7.9/10, 0 );
		glVertex3f( _width - _offsetX, _height * 7.9/10, 0 );
		//	Roll
		glVertex3f( _offsetX, _height * 6.7/10, 0 );
		glVertex3f( _width - _offsetX, _height * 6.7/10, 0 );
		glEnd();
		drawWBB_Box();

		// Draw the data points || X
		glBegin( GL_LINE_STRIP );
		glColor3ub( 255, 255, 0 );	// Yellow
		for ( _iterX=_X.begin(); _iterX != _X.end(); _iterX++ )
		{
			glVertex3f( _offsetX + (cntX*_scaleX),  zeroAxis + (*_iterX)*_scaleY *.8, 0 );
			cntX++;
		}
		glEnd();

		// Draw the data2 points || Y
		glBegin( GL_LINE_STRIP );
		glColor3ub( 0, 0, 255 );	// Blue
		for ( _iterY=_Y.begin(); _iterY != _Y.end(); _iterY++ )
		{
			glVertex3f( _offsetX + (cntY*_scaleX),  zeroAxis + (*_iterY)*_scaleY *.8, 0 );
			cntY++;
		}
		glEnd();


		// Draw Data CoP
		drawCoP();
		// Draw WBB CoP
		drawWBBIndic();
		glBegin( GL_LINE_STRIP );
		// Draw Data Angles
		drawAngles(75);

		// Yaw
		drawCircle(75, 450, _yawHeight , _currentYaw, 1);
		glBegin( GL_LINE_STRIP );
		glColor3ub( 0, 255, 0 );	// Green
		for ( _iterYaw=_yawData.begin(); _iterYaw != _yawData.end(); _iterYaw++ )
		{
			glVertex3f( _offsetX + (cntYaw*_scaleX),  _height * 9.2/10 + (*_iterYaw)*_scaleY * 15, 0 );
			cntYaw++;
		}
		glEnd();

		// Pitch
		drawCircle(75, 650, _pitchHeight , _currentPitch, 2);
		glBegin( GL_LINE_STRIP );
		glColor3ub( 255, 0, 255 );	// Purple
		for ( _iterPitch=_pitchData.begin(); _iterPitch != _pitchData.end(); _iterPitch++ )
		{
			glVertex3f( _offsetX + (cntPitch*_scaleX),  _height * 7.9/10 + (*_iterPitch)*_scaleY * 15, 0 );
			cntPitch++;	
		}
		glEnd();

		// Roll
		drawCircle(75, 850, _rollHeight , _currentRoll, 3);
		glBegin( GL_LINE_STRIP );
		glColor3ub( 0, 255, 255 );	// Cyan
		for ( _iterRoll=_rollData.begin(); _iterRoll != _rollData.end(); _iterRoll++ )
		{
			glVertex3f( _offsetX + (cntRoll*_scaleX),  _height * 6.7/10 + (*_iterRoll)*_scaleY * 15, 0 );
			cntRoll++;
		}
		glEnd();

		// Time Stamp
		for ( _iterTimeStamp=_timeStamp.begin(); _iterTimeStamp != _timeStamp.end(); _iterTimeStamp++ )
		{
			if (*_iterTimeStamp != 0)
			{
				glBegin( GL_LINES );
				glColor3ub( 255, 255, 255 );	// White
				//	0 - Axis
				glVertex3f( _offsetX + (cntTime*_scaleX),  zeroAxis + (*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + (cntTime*_scaleX),  zeroAxis + -1*(*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + ((cntTime + .2)*_scaleX),  zeroAxis + (*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + ((cntTime + .2)*_scaleX),  zeroAxis + -1*(*_iterTimeStamp)*_scaleY, 0 );
				//	Yaw
				glVertex3f( _offsetX + (cntTime*_scaleX),  _height * 9.2/10 + (*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + (cntTime*_scaleX),  _height * 9.2/10 + -1*(*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + ((cntTime + .2)*_scaleX),  _height * 9.2/10 + (*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + ((cntTime + .2)*_scaleX),  _height * 9.2/10 + -1*(*_iterTimeStamp)*_scaleY, 0 );
				// Pitch
				glVertex3f( _offsetX + (cntTime*_scaleX),  _height * 7.9/10 + (*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + (cntTime*_scaleX),  _height * 7.9/10 + -1*(*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + ((cntTime + .2)*_scaleX),  _height * 7.9/10 + (*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + ((cntTime + .2)*_scaleX),  _height * 7.9/10 + -1*(*_iterTimeStamp)*_scaleY, 0 );
				//	Roll
				glVertex3f( _offsetX + (cntTime*_scaleX),  _height * 6.7/10 + (*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + (cntTime*_scaleX),  _height * 6.7/10 + -1*(*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + ((cntTime + .2)*_scaleX),  _height * 6.7/10 + (*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + ((cntTime + .2)*_scaleX),  _height * 6.7/10 + -1*(*_iterTimeStamp)*_scaleY, 0 );
				
				glEnd();
			}
			cntTime++;
		}
	}
	// WBB View
	else if (_drawMode == 2)

	{
		drawWBB_Box();
		drawCoP();
		// Draw WBB CoP
		drawWBBIndic();
	}

	// WBB Data Only
	else if (_drawMode == 3)
	{
		drawBox();
		// Draw 0-axis
		glVertex3f( _offsetX, zeroAxis*2, 0 );
		glVertex3f( _width - _offsetX, zeroAxis*2, 0 );
		// Draw 2nd 0-axis
		glVertex3f( _offsetX, zeroAxis*4, 0 );
		glVertex3f( _width - _offsetX, zeroAxis*4, 0 );
		glEnd();

		// Draw the data points || X
		glBegin( GL_LINE_STRIP );
		glColor3ub( 255, 255, 0 );	// Yellow
		for ( _iterX=_X.begin(); _iterX != _X.end(); _iterX++ )
		{
			glVertex3f( _offsetX + (cntX*_scaleX),  zeroAxis*2 + (*_iterX)*_scaleY, 0 );
			cntX++;
		}
		glEnd();

		// Draw the data2 points || Y
		glBegin( GL_LINE_STRIP );
		glColor3ub( 0, 0, 255 );	// Blue
		for ( _iterY=_Y.begin(); _iterY != _Y.end(); _iterY++ )
		{
			glVertex3f( _offsetX + (cntY*_scaleX),  zeroAxis*4 + (*_iterY)*_scaleY, 0 );
			cntY++;
		}
		glEnd();

		// Time Stamp
		for ( _iterTimeStamp=_timeStamp.begin(); _iterTimeStamp != _timeStamp.end(); _iterTimeStamp++ )
		{
			if (*_iterTimeStamp != 0)
			{
				glBegin( GL_LINES );
				glColor3ub( 255, 255, 255 );	// White
				//	X -  Axis
				glVertex3f( _offsetX + (cntTime*_scaleX),  zeroAxis *2 + (*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + (cntTime*_scaleX),  zeroAxis *2 + -1*(*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + ((cntTime + .2)*_scaleX),  zeroAxis *2 + (*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + ((cntTime + .2)*_scaleX),  zeroAxis *2 + -1*(*_iterTimeStamp)*_scaleY, 0 );
				//	Y -  Axis
				glVertex3f( _offsetX + (cntTime*_scaleX),  zeroAxis *4 + (*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + (cntTime*_scaleX),  zeroAxis *4 + -1*(*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + ((cntTime + .2)*_scaleX),  zeroAxis *4 + (*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + ((cntTime + .2)*_scaleX),  zeroAxis *4 + -1*(*_iterTimeStamp)*_scaleY, 0 );


				
				glEnd();
			}
			cntTime++;
		}
	}

	// Oculus View Only
	else if (_drawMode == 4)
	{
		drawBox();
		glEnd();
		drawAngles(175);
		drawCircle(175, _yawX, _yawHeight, _currentYaw, 1);
		drawCircle(175, _pitchX, _pitchHeight , _currentPitch, 2);
		drawCircle(175, _rollX , _rollHeight , _currentRoll, 3);
	}
	// Oculus Angle Data Only
	else if( _drawMode == 5)
	{
		drawBox();
		glVertex3f(_offsetX ,_height * 5/6, 0);
		glVertex3f(_width - _offsetX, _height * 5/6, 0 );
		glVertex3f(_offsetX ,_height * .5, 0);
		glVertex3f(_width - _offsetX, _height * .5, 0 );
		glVertex3f(_offsetX ,_height * 1/6, 0);
		glVertex3f(_width - _offsetX, _height * 1/6, 0 );
		glEnd();
		// Yaw
		glBegin( GL_LINE_STRIP );
		glColor3ub( 0, 255, 0 );	// Green
		for ( _iterYaw=_yawData.begin(); _iterYaw != _yawData.end(); _iterYaw++ )
		{
			glVertex3f( _offsetX + (cntYaw*_scaleX),  _height * 5/6 + (*_iterYaw)*_scaleY * 15, 0 );
			cntYaw++;
		}
		glEnd();

		// Pitch
		glBegin( GL_LINE_STRIP );
		glColor3ub( 255, 0, 255 );	// Purple
		for ( _iterPitch=_pitchData.begin(); _iterPitch != _pitchData.end(); _iterPitch++ )
		{
			glVertex3f( _offsetX + (cntPitch*_scaleX),  _height * 1/2 + (*_iterPitch)*_scaleY * 15, 0 );
			cntPitch++;	
		}
		glEnd();


		// Roll
		glBegin( GL_LINE_STRIP );
		glColor3ub( 0, 255, 255 );	// Cyan
		for ( _iterRoll=_rollData.begin(); _iterRoll != _rollData.end(); _iterRoll++ )
		{
			glVertex3f( _offsetX + (cntRoll*_scaleX),  _height * 1/6 + (*_iterRoll)*_scaleY * 15, 0 );
			cntRoll++;
		}
		glEnd();

		// Time Stamp
		for ( _iterTimeStamp=_timeStamp.begin(); _iterTimeStamp != _timeStamp.end(); _iterTimeStamp++ )
		{
			if (*_iterTimeStamp != 0)
			{
				glBegin( GL_LINES );
				glColor3ub( 255, 255, 255 );	// White
				//	Yaw
				glVertex3f( _offsetX + (cntTime*_scaleX),  _height * 5/6 + (*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + (cntTime*_scaleX),  _height * 5/6 + -1*(*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + ((cntTime + .2)*_scaleX),  _height * 5/6 + (*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + ((cntTime + .2)*_scaleX),  _height * 5/6 + -1*(*_iterTimeStamp)*_scaleY, 0 );
				// Pitch
				glVertex3f( _offsetX + (cntTime*_scaleX),  _height * .5 + (*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + (cntTime*_scaleX),  _height * .5 + -1*(*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + ((cntTime + .2)*_scaleX),  _height * .5 + (*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + ((cntTime + .2)*_scaleX),  _height * .5 + -1*(*_iterTimeStamp)*_scaleY, 0 );
				//	Roll
				glVertex3f( _offsetX + (cntTime*_scaleX),  _height /6 + (*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + (cntTime*_scaleX),  _height /6 + -1*(*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + ((cntTime + .2)*_scaleX),  _height /6 + (*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + ((cntTime + .2)*_scaleX),  _height /6 + -1*(*_iterTimeStamp)*_scaleY, 0 );
				
				glEnd();
			}
			cntTime++;
		}

	}
	else if ( _drawMode == 6)
	{
		drawBox();
		glVertex3f(_offsetX ,_height * 5/6, 0);
		glVertex3f(_width - _offsetX, _height * 5/6, 0 );
		glVertex3f(_offsetX ,_height * .5, 0);
		glVertex3f(_width - _offsetX, _height * .5, 0 );
		glVertex3f(_offsetX ,_height * 1/6, 0);
		glVertex3f(_width - _offsetX, _height * 1/6, 0 );
		glEnd();
		// Oculus Tracker X
		glBegin( GL_LINE_STRIP );
		glColor3ub( 0, 100, 0 );	// Green
		for ( _iterOcX=_OcXData.begin(); _iterOcX != _OcXData.end(); _iterOcX++ )
		{
			glVertex3f( _offsetX + (cntYaw*_scaleX),  _height * 5/6 + (*_iterOcX)*_scaleY * 60, 0 );
			cntYaw++;
		}
		glEnd();

		// Oculus Tracker Y
		glBegin( GL_LINE_STRIP );
		glColor3ub( 100, 0, 100 );	// Purple
		for ( _iterOcY=_OcYData.begin(); _iterOcY != _OcYData.end(); _iterOcY++ )
		{
			glVertex3f( _offsetX + (cntPitch*_scaleX),  _height * 1/2 + (*_iterOcY)*_scaleY * 60, 0 );
			cntPitch++;	
		}
		glEnd();


		// Oculus Tracker Z
		glBegin( GL_LINE_STRIP );
		glColor3ub( 0, 100, 100 );	// Cyan
		for ( _iterOcZ=_OcZData.begin(); _iterOcZ != _OcZData.end(); _iterOcZ++ )
		{
			glVertex3f( _offsetX + (cntRoll*_scaleX),  _height * 1/6 + (*_iterOcZ)*_scaleY * 60, 0 );
			cntRoll++;
		}
		glEnd();

		// Time Stamp
		for ( _iterTimeStamp=_timeStamp.begin(); _iterTimeStamp != _timeStamp.end(); _iterTimeStamp++ )
		{
			if (*_iterTimeStamp != 0)
			{
				glBegin( GL_LINES );
				glColor3ub( 255, 255, 255 );	// White
				//	Yaw
				glVertex3f( _offsetX + (cntTime*_scaleX),  _height * 5/6 + (*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + (cntTime*_scaleX),  _height * 5/6 + -1*(*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + ((cntTime + .2)*_scaleX),  _height * 5/6 + (*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + ((cntTime + .2)*_scaleX),  _height * 5/6 + -1*(*_iterTimeStamp)*_scaleY, 0 );
				// Pitch
				glVertex3f( _offsetX + (cntTime*_scaleX),  _height * .5 + (*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + (cntTime*_scaleX),  _height * .5 + -1*(*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + ((cntTime + .2)*_scaleX),  _height * .5 + (*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + ((cntTime + .2)*_scaleX),  _height * .5 + -1*(*_iterTimeStamp)*_scaleY, 0 );
				//	Roll
				glVertex3f( _offsetX + (cntTime*_scaleX),  _height /6 + (*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + (cntTime*_scaleX),  _height /6 + -1*(*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + ((cntTime + .2)*_scaleX),  _height /6 + (*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + ((cntTime + .2)*_scaleX),  _height /6 + -1*(*_iterTimeStamp)*_scaleY, 0 );
				
				glEnd();
			}
			cntTime++;
		}

	}
	else if ( _drawMode == 7)
	{
		drawBox();
		// central Y
		glVertex3f(_width/2, _height * 3/4 ,0);
		glVertex3f(_width/2 , _height /4 ,0);
		// ceiling
		glVertex3f(_offsetX, _height * 3/4 ,0);
		glVertex3f(_width - _offsetX, _height * 3/4 ,0);
		// floor
		glVertex3f(_offsetX, _height /4 ,0);
		glVertex3f(_width - _offsetX, _height /4 ,0);
		// Center X-Axis
		glColor3ub( 0, 100, 0);
		glVertex3f(_offsetX, _height /2 ,0);
		glVertex3f(_width - _offsetX, _height /2 ,0);
		// Z-axis  X-Z
		glColor3ub( 0, 100, 100);
		glVertex3f(_width /4, _height * 3/4 ,0);
		glVertex3f(_width /4, _height /4 ,0);
		// Y-Axis X-Y
		glColor3ub( 100, 0, 100);
		glVertex3f(_width * 3/4, _height * 3/4 ,0);
		glVertex3f(_width * 3/4, _height /4 ,0);
		glEnd();

		drawOcIndic();

	}
}

void
OGLGraph::setDrawMode(int mode)
{
	if (mode == 1) // All View
	{
		// set up WBB Area
		_WBBCeiling = 30*_offsetY;
		_WBBFloor = 16*_offsetY;
		_WBBWall = 14*_offsetX;
		_WBBXaxis = 7*_offsetX;
		_WBBCoPX = (_offsetX + _WBBWall)/2;
		_WBBCoPY = (_WBBCeiling + _WBBFloor)/2;
		// WBB CoP Indicator Center Setup
		WBBSetup();
		_yawHeight = _WBBCoPY;
		_pitchHeight = _WBBCoPY;
		_rollHeight = _WBBCoPY;
		_yawX = 450;
		_pitchX = 650;
		_rollX = 850;
		_drawMode = 1;
	}
	else if (mode == 2) // WBB only View
	{
		// set up WBB Area
		_WBBWall = _width - _offsetX;
		_WBBXaxis = 7*_offsetX;
		_WBBCeiling = _height - _offsetY;
		_WBBFloor = _offsetY;
		_WBBCoPX = _width/2;
		_WBBCoPY = _height/2;
		// WBB CoP Indicator Center Setup
		WBBSetup();
		_drawMode = 2;
	}
	else if ( mode == 3) // WBB Data only
	{
		_drawMode = 3;
	}
	else if ( mode == 4) // Oculus View Only
	{
		_yawHeight = _height * 2/3;
		_pitchHeight = _height * 1/3;
		_rollHeight = _height * 2/3;
		_yawX = _width * 1.5/5;
		_pitchX = _width /2;
		_rollX = _width * 3.5/5;
		_drawMode = 4;
	}
	else if ( mode == 5)
	{
		_drawMode = 5;
	}
	else if ( mode == 6){
		_drawMode = 6;
	}
	else if ( mode == 7){
		_drawMode = 7;
		_WBBCoPX = _width/2;
		_WBBCoPY = _height/2;
		// WBB CoP Indicator Center Setup
		WBBSetup();
	}
}

// Sets all data
void OGLGraph::setCoP(int LR)
{
	if (LR != 0)
	{
		_CoPX[LR-1] = _currentX;
		_CoPY[LR-1] = _currentY;
		_yaw[LR-1] = _currentYaw;
		_pitch[LR-1] = _currentPitch;
		_roll[LR-1] = _currentRoll;
		_OculusX[LR-1] = _currentOcX;
		_OculusY[LR-1] = _currentOcY;
		_OculusZ[LR-1] = _currentOcZ;
		_showCoP[LR-1]  =1;
	}
	else
		for(int i = 0; i < 6; i++)
			_showCoP[i] = 0;
}

void
OGLGraph::drawCoP()
{
	if (_showCoP[0])
	{
		glBegin( GL_LINE_STRIP);
		glColor3ub( 150, 150, 150); // Gray
		drawStoredCoP( _CoPX[0], _CoPY[0]);
	}
	if (_showCoP[1])
	{
		glBegin( GL_LINE_STRIP);
		glColor3ub( 0, 150, 150); // 
		drawStoredCoP( _CoPX[1], _CoPY[1]);
	}
	if (_showCoP[2])
	{
		glBegin( GL_LINE_STRIP);
		glColor3ub( 150, 150, 0); // 
		drawStoredCoP( _CoPX[2], _CoPY[2]);
	}
	if (_showCoP[3])
	{
		glBegin( GL_LINE_STRIP);
		glColor3ub( 150, 0, 150); // 
		drawStoredCoP( _CoPX[3], _CoPY[3]);
	}
	if (_showCoP[4])
	{
		glBegin( GL_LINE_STRIP);
		glColor3ub( 0, 0, 150); // 
		drawStoredCoP( _CoPX[4], _CoPY[4]);
	}
	if (_showCoP[5])
	{
		glBegin( GL_LINE_STRIP);
		glColor3ub( 150, 0, 0); // 
		drawStoredCoP( _CoPX[5], _CoPY[5]);
	}
}

// declare line type and color before calling
void
OGLGraph::drawStoredCoP(float X, float Y)
{
	for (int x = 0; x <=10; ++x)
		{
			if (_drawMode == 1) {
				glVertex3f( _dataX[x] + X *2, _dataY[x] + Y *2, 0 );
				glVertex3f( _dataX[x] + X *2, _dataY[10-x] + Y *2, 0 );
			}
			else if (_drawMode == 2) {
				glVertex3f( _dataX[x] + X *7, _dataY[x] + Y *7, 0 );
				glVertex3f( _dataX[x] + X *7, _dataY[10-x] + Y *7, 0 );
			}
			else if (_drawMode == 7) {
				glVertex3f( _dataX[x] + X * 4, _dataY[x] + Y * 4, 0 );
				glVertex3f( _dataX[x] + X * 4, _dataY[10-x] + Y * 4, 0 );
			}
		}
	glEnd();
}

void
OGLGraph::drawAngles(int radius)
{
	if (_showCoP[0])
	{
		glBegin( GL_LINES);
		glColor3ub( 150, 150, 150); // Gray
		drawStoredAngles( _yaw[0], _pitch[0], _roll[0], radius);
	}
	if (_showCoP[1])
	{
		glBegin( GL_LINES);
		glColor3ub( 0, 150, 150); // 
		drawStoredAngles( _yaw[1], _pitch[1], _roll[1], radius);
	}
	if (_showCoP[2])
	{
		glBegin( GL_LINES);
		glColor3ub( 150, 150, 0); // 
		drawStoredAngles( _yaw[2], _pitch[2], _roll[2], radius);
	}
	if (_showCoP[3])
	{
		glBegin( GL_LINES);
		glColor3ub( 150, 0, 150); // 
		drawStoredAngles( _yaw[3], _pitch[3], _roll[3], radius);
	}
	if (_showCoP[4])
	{
		glBegin( GL_LINES);
		glColor3ub( 0, 0, 150); // 
		drawStoredAngles( _yaw[4], _pitch[4], _roll[4], radius);
	}
	if (_showCoP[5])
	{
		glBegin( GL_LINES);
		glColor3ub( 150, 0, 0); // 
		drawStoredAngles( _yaw[5], _pitch[5], _roll[5], radius);
	}
}

// for optimization can create a sin/cos table
// declare line type and color before calling
void
OGLGraph::drawStoredAngles(float yaw, float pitch, float roll, int radius)
{
	// Yaw
	glVertex3f( _yawX + radius *cos(PI/2 +yaw), _yawHeight + radius * sin(PI/2 +yaw), 0 );
	glVertex3f( _yawX + radius*1.2 *cos(PI/2 +yaw), _yawHeight + radius*1.2 * sin(PI/2 +yaw), 0 );
	// Pitch
	glVertex3f( _pitchX + radius *cos(pitch), _pitchHeight + radius * sin(pitch), 0 );
	glVertex3f( _pitchX + radius*1.2 *cos(pitch), _pitchHeight + radius*1.2 * sin(pitch), 0 );
	// Roll
	glVertex3f( _rollX + radius *cos(PI/2 +roll), _rollHeight + radius * sin(PI/2 +roll), 0 );
	glVertex3f( _rollX + radius*1.2 *cos(PI/2 +roll), _rollHeight + radius*1.2 * sin(PI/2 +roll), 0 );
	glEnd();
}

void OGLGraph::drawTimeStamp(int height)
{
	//	Time Stamp
		int cntTime = 0;
		for ( _iterTimeStamp=_timeStamp.begin(); _iterTimeStamp != _timeStamp.end(); _iterTimeStamp++ )
		{
			if (*_iterTimeStamp != 0)
			{
				glBegin( GL_LINE_STRIP );
				glColor3ub( 255, 255, 255 );	// White
				glVertex3f( _offsetX + (cntTime*_scaleX),  height + (*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + (cntTime*_scaleX),  height + -1*(*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + ((cntTime + .2)*_scaleX),  height + (*_iterTimeStamp)*_scaleY, 0 );
				glVertex3f( _offsetX + ((cntTime + .2)*_scaleX),  height + -1*(*_iterTimeStamp)*_scaleY, 0 );
				glEnd();
			}
			cntTime++;
		}
}

void
OGLGraph::drawWBBIndic()
// Draw WBB Indicator/Pointer
{
	glBegin( GL_LINE_STRIP);
	glColor3ub( 255, 255, 255); // White
	drawStoredCoP( _currentX, _currentY);
}

void
OGLGraph::drawOcIndic()
// Draw Oc Indicator/Pointer
{
	glBegin( GL_LINE_STRIP);
	glColor3ub( 255, 255, 255); // White
	drawStoredCoP( 60*_currentOcX -_width/16 , 60*_currentOcY);
	glBegin( GL_LINE_STRIP);
	glColor3ub( 255, 255, 255); // White
	drawStoredCoP( 60*_currentOcX + _width/16, 60*_currentOcZ);
	//drawStoredCoP( _currentOcX + _width/4, _currentOcZ);
}


void
OGLGraph::WBBSetup()
{
	// WBB CoP Indicator Center Setup
		for (int x = -5; x <= 5; ++x) {
			_dataX[x+5] = (x*2 + _WBBCoPX);
			//_dataXUp[x+5] = _dataX[x+5];
		}
		for (int y = 0; y <= 5; ++y) {
			_dataY[y] = (y*2 + _WBBCoPY);
			//_dataYUp[y] = _dataY[y];
		}
		for (int y = 0; y < 5; ++y) {
			_dataY[10-y] = (-y*2 + _WBBCoPY);
			//_dataYUp[10-y] = _dataY[10-y];
		}
}

void
OGLGraph::drawWBB_Box()
{
	glBegin( GL_LINES );
	glColor3ub( 255, 255, 255 );	// White
	// Draw y-axis
	glVertex3f( _offsetX, _offsetY, 0 );
	glVertex3f( _offsetX, _WBBCeiling, 0 );
	// WBB Floor
	glVertex3f( _offsetX, _WBBFloor, 0 );
	glVertex3f( _WBBWall, _WBBFloor, 0 );
	// WBB Box Wall
	glVertex3f( _WBBWall, _WBBCeiling, 0 );
	glVertex3f( _WBBWall, _WBBFloor, 0 );
	// WBB Ceiling
	glVertex3f( _offsetX, _WBBCeiling, 0 );
	glVertex3f( _width - _offsetX, _WBBCeiling, 0 );
	// WBB x-axis
	glVertex3f( _offsetX, _WBBCoPY, 0 );
	glVertex3f( _WBBWall, _WBBCoPY, 0 );
	// WBB y-axis
	glVertex3f( _WBBCoPX, _WBBCeiling, 0 );
	glVertex3f( _WBBCoPX, _WBBFloor, 0 );
	glEnd();
}

void 
OGLGraph::drawBox()
{
	glDisable (GL_LINE_STIPPLE);
	glBegin( GL_LINES );
	glColor3ub( 255, 255, 255 );	// White
	// Draw x-axis 
	glVertex3f( _offsetX, _offsetY, 0 );
	glVertex3f( _width - _offsetX, _offsetY, 0 );
	// Draw y-axis
	glVertex3f( _offsetX, _offsetY, 0 );
	glVertex3f( _offsetX, _height - _offsetY, 0 );
	// Right Border
	glVertex3f( _width - _offsetX, _offsetY, 0 );
	glVertex3f( _width - _offsetX, _height - _offsetY, 0 );
	// Draw Ceiling
	glVertex3f( _offsetX, _height - _offsetY, 0 );
	glVertex3f( _width - _offsetX, _height - _offsetY, 0 );
}

// draw a circle w/ radius centered at (X,Y) w/ line form center to edge in dierction of angle. Has 3 dif ccolor options
void 
drawCircle(int radius, float X, float Y, float angle, int color)
{
	glBegin(GL_LINE_STRIP);
	glColor3ub( 255, 255, 255 );	// White
	for (int degree = 0; degree < 360; degree++)	// draws the circle itself
		glVertex3f( radius * cos(degree * PI / 180) + X , radius * sin(degree * PI / 180) + Y, 0 );
	glVertex3f(X - radius, Y, 0);	// draw horizontal axis
	glEnd();
	glBegin(GL_LINE_STRIP);

	glVertex3f(X, Y - radius, 0);	// draw vertical axis
	glVertex3f(X, Y + radius, 0);	
	
	glEnd();
	glBegin(GL_LINE_STRIP);
	if (color == 1)
		glColor3ub( 0, 255, 0 );	// Green
	else if (color == 2)
		glColor3ub( 255, 0, 255 );	// Purple
	else
		glColor3ub( 0, 255, 255 );	// Cyan
	glVertex3f(X, Y, 0);
	
	// draw line from center to edge
	if (color != 2)
		glVertex3f(X + radius * cos(PI/2 + angle), Y + radius * sin(PI/2 + angle), 0);
	else
		glVertex3f(X + radius * cos(angle), Y + radius * sin(angle), 0);

	glEnd();
}

