/*
	Header file for OGLGraph class
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
#pragma once

#include <time.h>
#include <List>
#include "glut.h"
#include <math.h>

#define PI 3.14159265

class OGLGraph
{
public:
	static OGLGraph* Instance();
	~OGLGraph() {}
	void setup( int, int, int, int, float, int, int, int );
	void update( float, float,float, float, float,float, float, float, int);
	void draw();
	void setDrawMode(int);
	void setCoP(int);
	void setAngle(int);

private:
	static OGLGraph* _instance;
	OGLGraph() {}
	///// Functions
	void drawCoP();
	void drawStoredCoP(float, float);
	void drawAngles(int);
	void drawStoredAngles(float, float, float, int);
	void drawTimeStamp(int);
	void drawWBBIndic();
	void WBBSetup();
	void drawWBB_Box();
	void drawBox();
	void drawOcIndic();
	//void 
	///// Variables
	int _cacheSize;
	int _pWidth, _pHeight;	// in percentages of existing window sizes
	int _width, _height;
	int _channels;
	float _scaleX;
	int _scaleY;
	int _offsetX, _offsetY;
	int zeroAxis;
	int _drawMode; // 0 = All View || 1 = WBB View
	int _WBBCoPY, _WBBCoPX, _WBBWall, _WBBXaxis, _WBBCeiling, _WBBFloor; // WBB_Box Paramaters
	int _showCoP[6];
	int _yawHeight, _pitchHeight, _rollHeight, _yawX, _pitchX, _rollX;
	float _CoPX[6], _CoPY[6];
	float _OculusX[6], _OculusY[6], _OculusZ[6];
	float _yaw[6], _pitch[6], _roll[6];
	float _currentX, _currentY, _currentYaw, _currentPitch, _currentRoll, _currentOcX, _currentOcY, _currentOcZ;
	char sWBBX[7], sWBBY[7], sYaw[7], sPitch[7], sRoll[7], sOcX[7], sOcY[7], sOcZ[7];
	std::list<float> _X;
	std::list<float> _Y;
	std::list<float> _yawData;
	std::list<float> _pitchData;
	std::list<float> _rollData;
	std::list<float> _OcXData;
	std::list<float> _OcYData;
	std::list<float> _OcZData;
	std::list<float> _timeStamp;
	float _CoPDataX[11];
	float _CoPDataY[11];
	float _dataX[11];
	float _dataY[11];
	//float _dataXUp[11];
	//float _dataYUp[11];
	std::list<float>::const_iterator _iterX;
	std::list<float>::const_iterator _iterY;
	std::list<float>::const_iterator _iterYaw;
	std::list<float>::const_iterator _iterPitch;
	std::list<float>::const_iterator _iterRoll;
	std::list<float>::const_iterator _iterOcX;
	std::list<float>::const_iterator _iterOcY;
	std::list<float>::const_iterator _iterOcZ;
	std::list<float>::const_iterator _iterTimeStamp;
};
