#include "WiiModule.h"

wiimote** wiimotes;

int handle_event(struct wiimote_t* wm, float *num, float *num2) {
	//printf("\n\n--- EVENT [id %i] ---\n", wm->unid);
	// only reads for WBB, if needed go through wiiuse.c to implement other wii controllers

	if (wm->exp.type == EXP_WII_BOARD) {
		// wii balance board 
		struct wii_board_t* wb = (wii_board_t*)&wm->exp.wb;
		float total = wb->tl + wb->tr + wb->bl + wb->br;
		//float x = ((wb->tr + wb->br -1) / total) * 2 - 1;
		//float y = ((wb->tl + wb->tr) / total) * 2 - 1;
		float x = ((wb->tr + wb->br - wb->bl - wb->tl));
		float y = ((wb->tl + wb->tr - wb-> bl - wb->br));
		//printf("tr: %f  tl: %f  br: %f  bl: %f\n",wb->tr,wb->tl,wb->br,wb->bl);
		*num = x;
		*num2 = y;
		//printf("Weight: %f kg @ (%f, %f)\n", total, x, y);
		//printf("Weight: %f kg @ (%f, %f)\n", total, x, y);
		// printf("Interpolated weight: TL:%f  TR:%f  BL:%f  BR:%f\n", wb->tl, wb->tr, wb->bl, wb->br); 
		// printf("Raw: TL:%d  TR:%d  BL:%d  BR:%d\n", wb->rtl, wb->rtr, wb->rbl, wb->rbr);
		return 1;
	}
	else
	{
		printf("Not a Wii Balance Board Connected");
		return -1;
	}
}

void handle_ctrl_status(struct wiimote_t* wm) {
	printf("\n\n--- CONTROLLER STATUS [wiimote id %i] ---\n", wm->unid);

	printf("attachment:      %i\n", wm->exp.type);
	printf("speaker:         %i\n", WIIUSE_USING_SPEAKER(wm));
	printf("ir:              %i\n", WIIUSE_USING_IR(wm));
	printf("leds:            %i %i %i %i\n", WIIUSE_IS_LED_SET(wm, 1), WIIUSE_IS_LED_SET(wm, 2), WIIUSE_IS_LED_SET(wm, 3), WIIUSE_IS_LED_SET(wm, 4));
	printf("battery:         %f %%\n", wm->battery_level);
}

void handle_read(struct wiimote_t* wm, byte* data, unsigned short len) {
	int i = 0;

	printf("\n\n--- DATA READ [wiimote id %i] ---\n", wm->unid);
	printf("finished read of size %i\n", len);
	for (; i < len; ++i) {
		if (!(i % 16)) {
			printf("\n");
		}
		printf("%x ", data[i]);
	}
	printf("\n\n");
}

void handle_disconnect(wiimote* wm) {
	printf("\n\n--- DISCONNECTED [wiimote id %i] ---\n", wm->unid);
}


void test(struct wiimote_t* wm, byte* data, unsigned short len) {
	printf("test: %i [%x %x %x %x]\n", len, data[0], data[1], data[2], data[3]);
}

short any_wiimote_connected(wiimote** wm, int wiimotes) {
	int i;
	if (!wm) {
		return 0;
	}

	for (i = 0; i < wiimotes; i++) {
		if (wm[i] && WIIMOTE_IS_CONNECTED(wm[i])) {
			return 1;
		}
	}

	return 0;
}


void run(float *valueP, float *value2P)
{
	int found, connected;
	//Initialize an array of wiimote objects. The parameter is the number of wiimotes I want to create.
	wiimotes =  wiiuse_init(MAX_WIIMOTES);
	found = wiiuse_find(wiimotes, MAX_WIIMOTES, 5);
	if (!found) 
	{
		printf("No wiimotes found.\n");
		return;
	}
	connected = wiiuse_connect(wiimotes, MAX_WIIMOTES);
	if (connected) 
	{
		printf("Connected to %i wiimotes (of %i found).\n", connected, found);
	} 
	else 
	{
		printf("Failed to connect to any wiimote.\n");
		return;
	}
	wiiuse_set_leds(wiimotes[0], WIIMOTE_LED_1);
	#ifndef WIIUSE_WIN32
	usleep(200000);
	#else
	Sleep(200);
	#endif
	while (any_wiimote_connected(wiimotes, MAX_WIIMOTES)) 
	{
		if (wiiuse_poll(wiimotes, MAX_WIIMOTES)) 
		{
		//This happens if something happened on any wiimote.
			switch (wiimotes[0]->event){
				case WIIUSE_EVENT:
					// a generic event occurred
					// go through wiimote event handle to change value and value2
					// Our X and Y coordinates of the Center of Pressure
					handle_event(wiimotes[0], valueP, value2P);
					break;

				case WIIUSE_STATUS:
					// a status event occurred 
					handle_ctrl_status(wiimotes[0]);
					break;

				case WIIUSE_DISCONNECT:
				case WIIUSE_UNEXPECTED_DISCONNECT:
					// the wiimote disconnected 
					handle_disconnect(wiimotes[0]);
					break;

				case WIIUSE_READ_DATA:
					// Data we requested to read was returned. Take a look at wiimotes[i]->read_req for the data.
					break;
			}
		}
	}
	wiiuse_cleanup(wiimotes, MAX_WIIMOTES);
}