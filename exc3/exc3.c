#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>

#define LOCK_SPEED 20
typedef void *(*state_func)();

int c = 0;
bool backDoorsLocked = false, frontDoorOpen = false, backDoorOpen = false;
int carSpeed = 0; 
int buttonHandle = 0;
int manualUnlocking = 0;
int startBraking = 0, startAcelerating = 0;


int kbhit(void);
void *doorsLocked();
void *doorsUnlocked();


void *doorsLocked(){
	fprintf(stderr, " DOORS LOCKED  ");
	switch (buttonHandle){
		case 2:
			manualUnlocking = 1;
			buttonHandle = 0;
			return doorsUnlocked;
			break;
	}
	if (frontDoorOpen || backDoorOpen) return doorsUnlocked;
	return doorsLocked;
}

void *doorsUnlocked(){
	fprintf(stderr, " DOORS UNLOCKED ");
	switch (buttonHandle){
		case 1:
			if (!backDoorOpen && !frontDoorOpen) return doorsLocked;
			break;
	}

	// AUTO LOCKING
	if (!carSpeed) manualUnlocking = 0;
	if (carSpeed > LOCK_SPEED && !frontDoorOpen && !backDoorOpen && !manualUnlocking) return doorsLocked;
	return doorsUnlocked;
}

void Commands(){
	printf("\n\n  0 - Brake/Stop braking \n 1/2- Acelerate/Stop acelerating \n  3 - Open/Close Front door \n  4 - Open/Close Back door \n  5 - Lock button \n  6 - Unlock button \n  7 - Enable/Disable Kid protection \n");
}


int main(){
	state_func curr_state = doorsUnlocked;
	printf(" \n               PRESS 8 TO SEE COMMANDS \n\n Starting...\n");
	while(1){
		curr_state = (state_func)(*curr_state)();
		fprintf(stderr, "| Carspeed: %d | Door open? %d | Back doors kid protection? %d \n", carSpeed,(frontDoorOpen || backDoorOpen),   backDoorsLocked);
		if (carSpeed == 0) {startBraking =0 ;}
		if (startBraking){ carSpeed--;}
		if (startAcelerating == 1) { carSpeed++;}
		if (startAcelerating == 2) { carSpeed += 3; }
		buttonHandle = 0;
		sleep(1);
		
		if (kbhit()){
			c = getchar();
			
			switch (c){
				case '0': // BRAKE
					printf("   Braking...");
					if (startBraking == 1) startBraking = 0; else startBraking = 1;;
					if (carSpeed > 0) carSpeed--;
					startAcelerating = 0;
					break;
				case '1': // SPEED UP 
					printf("   Acelerating...");
					carSpeed++;
					startBraking = 0;
					if (startAcelerating == 1) startAcelerating = 0; else startAcelerating = 1;
					break;
				case '2': // SPEED UP ALOT
					printf("   Acelerating...");
					if (startAcelerating == 2 ) startAcelerating = 0; else startAcelerating = 2;
					carSpeed += 3;
					startBraking = 0;
					break;
				case '3': // OPEN/CLOSE FRONT DOOR
					if (curr_state == doorsUnlocked && !frontDoorOpen){  printf("     Opening front door...");	frontDoorOpen = true;	}
					else if(frontDoorOpen){ printf("    Closing front door...");	frontDoorOpen = false; }
					 
					break;
				case '4': // OPEN/CLOSE BACK DOOR
					if (!backDoorsLocked && curr_state == doorsUnlocked && !backDoorOpen){ backDoorOpen = true; printf("     Opening back door..."); }
					else if(backDoorOpen){backDoorOpen = false; printf("     Closing back door...");}
					break;
				case '5': // LOCK BUTTON
					if(curr_state == doorsUnlocked && !frontDoorOpen && !backDoorOpen){ buttonHandle = 1; printf("   Locking doors...");}
					break;
				case '6': // UNLOCK BUTTON
					if(curr_state == doorsLocked){ buttonHandle = 2; printf("   Unlocking doors...");}
					break;
				case '7': // KID PROTECTION
					if(!backDoorsLocked){
						printf("   Kid protection enabled...");
					}else
					printf("    Kid protection disabled");	
					backDoorsLocked = !backDoorsLocked;
					break;
				case '8':
					Commands();
					break;
			}
			printf("\n");
		}
	}
	return 0;
}
