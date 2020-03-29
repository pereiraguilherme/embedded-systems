#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>

#define LOCK_SPEED 20
typedef void *(*state_func)();

int c = 0;
bool lockedCockpit = true;
int jetSpeed = 0, parachoteState =0;
int ejectButtonHandle = 0;
int startBraking = 0, startAcelerating = 0;


int kbhit(void);
//FIRST LEVEL FSM
void *flyng();
//SECOND LEVEL FSM
void *jetAccelerate();
void *jetBreak();

//FIRST LEVEL FSM
void *eject();
//SECOND LEVEL FSM
void *unlockCockpit();
void *ejectSeat();
void *openParachute();

void *flyng(){
	fprintf(stderr, "FLYNG");
	//FSM AQUI DENTRO
}

void *eject(){
	fprintf(stderr, "EJECT");
	//FSM AQUI DENTRO
}

void Commands(){
	printf("\n\n  0 - Brake/Stop braking \n 1/2 - Acelerate/Stop acelerating \n 3 - Eject button \n");
}


int main(){
	state_func curr_state = flyng;
	printf(" \n               PRESS 8 TO SEE COMMANDS \n\n Starting...\n");
	while(1){
		curr_state = (state_func)(*curr_state)();
		fprintf(stderr, "| Jetpeed: %d | Cockpit open? %d \n", jetSpeed, lockedCockpit);
		if (jetSpeed == 0) {startBraking =0 ;}
		if (startBraking){ jetSpeed--;}
		if (startAcelerating == 1) { jetSpeed++;}
		if (startAcelerating == 2) { jetSpeed += 3; }
		ejectButtonHandle = 0;
		sleep(1);
		
		if (kbhit()){
			c = getchar();
			
			switch (c){
				case '8':
					Commands();
					break;
			}
			printf("\n");
		}
	}
	return 0;
}
