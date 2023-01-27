#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include "adminui.h"
#include "defs.h"
#include "pilot.h"

static bool_e running = FALSE;

/*-----------------------PRIVATE FUNCTIONS PROTOTYPES-----------------------*/
static void AdminUI_captureChoice();
static void AdminUI_askMvt(Direction direction);
static VelocityVector AdminUI_translate(Direction direction);
static void AdminUI_ask4Log();
static void AdminUI_askClearLog();
static void AdminUI_eraseLog();
static void AdminUI_quit();
static void AdminUI_run();
static void AdminUI_display();

/*-----------------------PUBLIC FUNCTIONS-----------------------*/
extern void AdminUI_new(){
	//
}

extern void AdminUI_free(){
	//free(pAdminUI);
}

extern void AdminUI_start(){
	system("stty -echo");
	system("stty cbreak");//removes enter validation from user to read inputs
	//AdminUI_new();
	Pilot_start();
	AdminUI_run();
}

extern void AdminUI_stop(){
	Pilot_stop();
	//AdminUI_free();
}

/*-----------------------PRIVATE FUNCTIONS-----------------------*/

static void AdminUI_captureChoice(){
	switch(getchar()){
		case TURN_RIGHT :		AdminUI_askMvt(RIGHT);		break;
		case TURN_LEFT :		AdminUI_askMvt(LEFT);		break;
		case MOVE_FORWARD :		AdminUI_askMvt(FORWARD);	break;
		case MOVE_BACKWARD :	AdminUI_askMvt(BACKWARD);	break;
		case STOP_MOVING :		AdminUI_askMvt(STOP);		break;
		case ERASE_LOGS :		AdminUI_askClearLog();		break;
		case DISPLAY_ROBOT :	AdminUI_ask4Log();			break;
		case EXIT :				AdminUI_quit();				break;
		default :				printf("Wrong commands\n");	break;
	}
}

static void AdminUI_askMvt(Direction direction){
	switch(direction){
			case LEFT :			Pilot_run(EVENT_SET_VELOCITY, AdminUI_translate(LEFT));		break;
			case RIGHT :		Pilot_run(EVENT_SET_VELOCITY, AdminUI_translate(RIGHT));	break;
			case FORWARD :		Pilot_run(EVENT_SET_VELOCITY, AdminUI_translate(FORWARD));	break;
			case BACKWARD :		Pilot_run(EVENT_SET_VELOCITY, AdminUI_translate(BACKWARD));	break;
			case STOP :			Pilot_run(EVENT_SET_VELOCITY, AdminUI_translate(STOP));		break;
			default :																		break;
		}
}

static VelocityVector AdminUI_translate(Direction direction){
	VelocityVector vel = {direction, 100};
	return vel;
}

static void AdminUI_ask4Log(){
	Pilot_run(EVENT_CHECK, AdminUI_translate(STOP));
	PilotSensors logs = Pilot_getState();
	printf("Robot status :\n");
	printf("Collision : %d\n", logs.collision);
	printf("Luminosity : %f\n", logs.luminosity);
	printf("Speed : %d\n", logs.speed);
}

static void AdminUI_askClearLog(){
	AdminUI_eraseLog();
}

static void AdminUI_eraseLog(){
	system("@cls||clear");
}

static void AdminUI_quit(){
	printf("\n");
	printf("Exiting ROBOT CONTROLLER...\n");
	system("stty -cbreak");
	system("stty echo");
	running = FALSE;
}

static void AdminUI_run(){
	running = TRUE;
	while(running){
		AdminUI_display();
		AdminUI_captureChoice();
	}
}

static void AdminUI_display(){
	printf("Welcome to ROBOT CONTROLLER\n");
	printf("Here is a list of commands you can execute :\n");
	printf("Z : Move forward\n");
	printf("S : Move backward\n");
	printf("Q : Move left\n");
	printf("D : Move right\n");
	printf("R : Display robot state\n");
	printf("E : Clear logs\n");
	printf("A : Exit ROBOT CONTROLLER\n");
}
   
