#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>

#define LOCK_SPEED 20
typedef void *(*state_func)();

int c = 0;
bool lockedCockpit = true;
int jetSpeed = 0, parachuteState =0;
int ejectButtonHandle = 0;
int startBraking = 0, startAcelerating = 0;
bool straightUp = true;

int kbhit(void);
//FIRST LEVEL FSM
void *flyng();
//SECOND LEVEL FSM
void *jetAccelerate();
void *jetBrake();

//FIRST LEVEL FSM
void *eject();
//SECOND LEVEL FSM
void *ejectProcedure();
void *openParachute();
void *LANDING();



void *flying(){
	fprintf(stderr, "FLYING ");
	if (ejectButtonHandle && straightUp && jetSpeed < 1225 ) return eject;

	if (startAcelerating > 0) return jetAccelerate;
	if (startBraking > 0) return jetBrake;

	return flying;
	//FSM AQUI DENTRO
}

void *jetAccelerate(){
	fprintf(stderr, "ACELERATING ");
	if (ejectButtonHandle && straightUp && jetSpeed < 1225 ) return eject;

	switch (startAcelerating){
		case 0:
			return flying;
			break;
		case 1:
			jetSpeed += 75;
			return jetAccelerate;
			break;
		case 2:
			jetSpeed += 230;
			return jetAccelerate;
			break;
	}
	return flying;

}

void *jetBrake(){
	//EJECT
	fprintf(stderr, "BRAKING ");
	if (ejectButtonHandle && straightUp && jetSpeed < 1225 ) return eject;
	if(startBraking){
		if (jetSpeed >= 30){
			jetSpeed -= 30;
		}else jetSpeed = 0;
		return jetBrake;
	} else return flying;

}

void *eject(){
	fprintf(stderr, "\nEJECT SWITCH ACTIVATED ");
	if (!ejectButtonHandle) return flying; //BUTTON DEBOUNCE
	return ejectProcedure;

}

void *ejectProcedure(){
	fprintf(stderr, "\nEJECT COMFIRMED \n");
	// BREAK GLASS
	// SEAT ROCKETS


	// PARACHUTE
	return openParachute;

}

void *openParachute(){
	fprintf(stderr, "OPENING PARACHUTE \n");
	//OPEN PARACHUTE
	sleep(1);
	fprintf(stderr, "LANDING ");
	return LANDING;
}

void *LANDING(){

	sleep(1000);
	return LANDING;
}

void Commands(){
	printf("\n\n  0 - Brake/Stop braking \n 1/2 - Acelerate/Stop acelerating \n 3 - Eject button \n 4 - Half barrel roll\n");
}


int main(){
	state_func curr_state = flying;
	printf(" \n               PRESS 8 TO SEE COMMANDS \n\n Starting...\n");
	while(1){
		
		curr_state = (state_func)(*curr_state)();

		if (curr_state != ejectProcedure && curr_state != eject && curr_state != openParachute && curr_state != LANDING) 
				fprintf(stderr, "| Jetpeed: %d  | Straight up? %d \n", jetSpeed, straightUp);

		ejectButtonHandle = 0;
		sleep(1);
		
		if (kbhit()){
			c = getchar();
			
			switch (c){
				case '0':
					if(startBraking == 1) startBraking = 0; else startBraking = 1;
					break;
				case '1':
					if(startAcelerating == 0){ startAcelerating = 1;} else { startAcelerating = 0;}
					break;
				case '2':
					if (startAcelerating == 0) startAcelerating = 2; else startAcelerating = 0;
					break;
				case '3': // EJECT BUTTON
					ejectButtonHandle = 1;
					break;
				case '4':
					straightUp = !straightUp;
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
