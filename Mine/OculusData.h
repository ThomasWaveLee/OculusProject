#pragma once

#include <iostream>
#include "OVR_CAPI.h"
#include "OVR_Math.h"

typedef struct OCULUSPASSER{
	float *OcXP, *OcYP, *OcZP;
	float *YawP, *PitchP, *RollP;
}OculusPasser;

void OculusDataAngles(float*, float*, float*);
void OculusDataTracker(OculusPasser passer);
