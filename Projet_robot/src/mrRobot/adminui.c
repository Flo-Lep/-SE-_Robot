#include <stdio.h>
#include <stdbool.h>
#include "adminui.h"
#include "defs.h"

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
	//Pilot_start();
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
		case ERASE_LOGS :		AdminUI_askClearLog();		break;
		case DISPLAY_ROBOT :	AdminUI_ask4Log();			break;
		case EXIT :				AdminUI_quit();				break;
		default :				printf("Wrong command");	break;
	}
}

static void AdminUI_askMvt(Direction direction){
	switch(direction){
			case LEFT :		AdminUI_askMvt(LEFT);		break;
			case RIGHT :	AdminUI_askMvt(RIGHT);		break;
			case FORWARD :	AdminUI_askMvt(FORWARD);	break;
			case BACKWARD :	AdminUI_askMvt(BACKWARD);	break;
			default : 									break;
		}
}

static VelocityVector AdminUI_translate(Direction direction){
	VelocityVector vel = {direction, 0}; //TODO : which value for power ?
	return vel;
}

static void AdminUI_ask4Log(){
	printf("Asking for logs...");
}

static void AdminUI_askClearLog(){
	system("@cls||clear");
}

static void AdminUI_eraseLog(){
	//erase the log file
	printf("Erasing logs...");
}

static void AdminUI_quit(){
	printf("\n");
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
	AdminUI_stop();
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
   
