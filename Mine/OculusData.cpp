//
//  main.cpp
//  oculusModule
//

//#include <unistd.h>
//#include <iostream>
//#include "OVR.h"
#include "OculusData.h"
#include "OVR_Math.h"
#include <windows.h>

//void OculusDataTracker(float *OcXP, float *OcYP, float *OcZP, float *YawP, float *PitchP, float *RollP)
void OculusDataTracker(OculusPasser passer)
{
    if ( !(OVR_FAILURE(ovr_Initialize(nullptr))))
    {
		printf("Oculus Detected\n");
       /* printf("Oculus detected: %d\n", ovrHmd_Detect());
		ovrHmd mainHmd;*/
        ovrSession mainSession;
		ovrGraphicsLuid luid;
        ovrResult result = ovr_Create(&mainSession, &luid);

		if(OVR_FAILURE(result)){ 
			printf("Failed to create HMD\n");
			ovr_Destroy(mainSession);
			ovr_Shutdown();
			return; 
		}
        
		float yaw, eyePitch, eyeRoll;
        float x,y,z;

        while (true)
        {
            double frameTime = ovr_GetTimeInSeconds();
            ovrTrackingState currentState = ovr_GetTrackingState(mainSession, frameTime, ovrTrue);
			if(currentState.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked))
			{
				unsigned int currentFeature = currentState.StatusFlags;
				ovrPoseStatef currentPose = currentState.HeadPose;
         
				OVR::Posef currentOrientation = currentPose.ThePose;
				currentOrientation.Rotation.GetYawPitchRoll(&yaw, &eyePitch, &eyeRoll);

				*passer.YawP = yaw;
				*passer.PitchP = eyePitch;
				*passer.RollP = eyeRoll;

				x = currentOrientation.Translation.x;
				y = currentOrientation.Translation.y;
				z = currentOrientation.Translation.z;

				*passer.OcXP = x;
				*passer.OcYP = y;
				*passer.OcZP = z;
				/*
				printf("------\nCurrent time: %.6f\n", frameTime);
				printf("\tFeatureas: Camera connected: %5s| Camera catching: %5s| Camera initialized: %5s\n", cameraConnect?"True":"False", cameraSuccess?"True":"False", cameraInit?"True":"False");
				printf("\t\tPosition: x  :%+.6f y    :%+.6f z   :%+.6f\n", x, y, z);
				printf("\t\tRotation: Yaw:%+.6f Pitch:%+.6f Roll:%+.6f\n", yaw, eyePitch, eyeRoll);
				Sleep(1);
				*/
			}
            
        }
        
        
        ovr_Destroy(mainSession);
        ovr_Shutdown();
    } 
	else{
		ovrErrorInfo errorInfo;
        ovr_GetLastErrorInfo(&errorInfo);
        printf("ovr_Initialize failed: %s\n", errorInfo.ErrorString);
		ovr_Shutdown();
	}
}

/* Deprecated old code kept for reference
void OculusDataAngles(float *YawP, float *PitchP, float *RollP)
{
    if ( ovr_Initialize(nullptr) == ovrSuccess)
    {
        printf("Oculus detected: %d\n", ovrHmd_Detect());
		ovrHmd mainHmd;
        ovrResult result = ovrHmd_Create(0, &mainHmd);
        
        ovrTrackingCaps mainSupportCaps = (ovrTrackingCaps)(ovrTrackingCap_MagYawCorrection | ovrTrackingCap_Orientation | ovrTrackingCap_Position |ovrTrackingCap_Idle);
        
        if (!ovrHmd_ConfigureTracking(mainHmd, mainSupportCaps, mainSupportCaps)) printf("Tracking Capacibility not supported\n");
		
		/*
		ovrTrackingState startState = ovrHmd_GetTrackingState(mainHmd, 0);
        unsigned int startFeature = startState.StatusFlags;
        ovrPoseStatef startPose = startState.HeadPose;
		OVR::Posef startOrientation = startPose.ThePose;
        float startyaw, starteyePitch, starteyeRoll;
        startOrientation.Rotation.GetEulerAngles<OVR::Axis_Y, OVR::Axis_X, OVR::Axis_Z>(&startyaw, &starteyePitch, &starteyeRoll);
            

        while (true)
        {
            double frameTime = ovr_GetTimeInSeconds();
            ovrTrackingState currentState = ovrHmd_GetTrackingState(mainHmd, frameTime);
            unsigned int currentFeature = currentState.StatusFlags;
            ovrPoseStatef currentPose = currentState.HeadPose;
            
            bool cameraConnect = (bool)(currentFeature & ovrStatus_PositionConnected);
            bool cameraSuccess = (bool)(currentFeature & ovrStatus_PositionTracked);
            bool cameraInit = (bool)(currentFeature & ovrStatus_CameraPoseTracked);
            
            
            OVR::Posef currentOrientation = currentPose.ThePose;
            float yaw, eyePitch, eyeRoll;
            currentOrientation.Rotation.GetEulerAngles<OVR::Axis_Y, OVR::Axis_X, OVR::Axis_Z>(&yaw, &eyePitch, &eyeRoll);

			*YawP = yaw;
			*PitchP = eyePitch;
			*RollP = eyeRoll;
            /*
            float x,y,z;
            x = currentOrientation.Translation[0];
            y = currentOrientation.Translation[1];
            z = currentOrientation.Translation[2];

			*OcXP = x;
			*OcYP = y;
			*OcZP = z;
			*/
            /*
            printf("------\nCurrent time: %.6f\n", frameTime);
            printf("\tFeatureas: Camera connected: %5s| Camera catching: %5s| Camera initialized: %5s\n", cameraConnect?"True":"False", cameraSuccess?"True":"False", cameraInit?"True":"False");
            printf("\t\tPosition: x  :%+.6f y    :%+.6f z   :%+.6f\n", x, y, z);
            printf("\t\tRotation: Yaw:%+.6f Pitch:%+.6f Roll:%+.6f\n", yaw, eyePitch, eyeRoll);
            Sleep(1);
			
            
        }
        
        
        ovrHmd_Destroy(mainHmd);
        ovr_Shutdown();
    } else ovr_Shutdown();
}

void OculusDataTracker(float *OcXP, float *OcYP, float *OcZP)
{
    if ( ovr_Initialize(nullptr) == ovrSuccess)
    {
        printf("Oculus detected: %d\n", ovrHmd_Detect());
		ovrHmd mainHmd;
        ovrResult result = ovrHmd_Create(0, &mainHmd);
        
        ovrTrackingCaps mainSupportCaps = (ovrTrackingCaps)(ovrTrackingCap_MagYawCorrection | ovrTrackingCap_Orientation | ovrTrackingCap_Position |ovrTrackingCap_Idle);
        
        if (!ovrHmd_ConfigureTracking(mainHmd, mainSupportCaps, mainSupportCaps)) printf("Tracking Capacibility not supported\n");
		
		/*
		ovrTrackingState startState = ovrHmd_GetTrackingState(mainHmd, 0);
        unsigned int startFeature = startState.StatusFlags;
        ovrPoseStatef startPose = startState.HeadPose;
		OVR::Posef startOrientation = startPose.ThePose;
        float startyaw, starteyePitch, starteyeRoll;
        startOrientation.Rotation.GetEulerAngles<OVR::Axis_Y, OVR::Axis_X, OVR::Axis_Z>(&startyaw, &starteyePitch, &starteyeRoll);
           

        while (true)
        {
            double frameTime = ovr_GetTimeInSeconds();
            ovrTrackingState currentState = ovrHmd_GetTrackingState(mainHmd, frameTime);
            unsigned int currentFeature = currentState.StatusFlags;
            ovrPoseStatef currentPose = currentState.HeadPose;
            
            bool cameraConnect = (bool)(currentFeature & ovrStatus_PositionConnected);
            bool cameraSuccess = (bool)(currentFeature & ovrStatus_PositionTracked);
            bool cameraInit = (bool)(currentFeature & ovrStatus_CameraPoseTracked);
            
            
            OVR::Posef currentOrientation = currentPose.ThePose;
			/*
            float yaw, eyePitch, eyeRoll;
            currentOrientation.Rotation.GetEulerAngles<OVR::Axis_Y, OVR::Axis_X, OVR::Axis_Z>(&yaw, &eyePitch, &eyeRoll);

			*YawP = yaw;
			*PitchP = eyePitch;
			*RollP = eyeRoll;
			
            
            float x,y,z;
            x = currentOrientation.Translation[0];
            y = currentOrientation.Translation[1];
            z = currentOrientation.Translation[2];

			*OcXP = x;
			*OcYP = y;
			*OcZP = z;
            /*
            printf("------\nCurrent time: %.6f\n", frameTime);
            printf("\tFeatureas: Camera connected: %5s| Camera catching: %5s| Camera initialized: %5s\n", cameraConnect?"True":"False", cameraSuccess?"True":"False", cameraInit?"True":"False");
            printf("\t\tPosition: x  :%+.6f y    :%+.6f z   :%+.6f\n", x, y, z);
            printf("\t\tRotation: Yaw:%+.6f Pitch:%+.6f Roll:%+.6f\n", yaw, eyePitch, eyeRoll);
            Sleep(1);
			
            
        }
        
        
        ovrHmd_Destroy(mainHmd);
        ovr_Shutdown();
    } else ovr_Shutdown();
}*/
