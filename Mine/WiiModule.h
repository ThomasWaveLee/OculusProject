#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "..\wiiuse\wiiuse.h"                     /* for wiimote_t, classic_ctrl_t, etc */
using namespace std;

#ifndef WIIUSE_WIN32
#include <unistd.h>                     /* for usleep */
#endif


#define MAX_WIIMOTES				1

//////////////////////////////////////////////////////////
/////////////////       WiiStuff       //////////////////
////////////////////////////////////////////////////////

int handle_event(struct wiimote_t* wm, float *num, float *num2);

void handle_ctrl_status(struct wiimote_t* wm);

void handle_read(struct wiimote_t* wm, byte* data, unsigned short len);

void handle_disconnect(wiimote* wm);

void test(struct wiimote_t* wm, byte* data, unsigned short len);

short any_wiimote_connected(wiimote** wm, int wiimotes);

void run(float *valueP, float *value2P);