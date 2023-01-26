#include <stdio.h>
#include <stdlib.h>
#include "pilot.h"
#include "robot.h"

static Pilot *pPilot;
static Direction init_direction = STOP;
static int init_power = 0;
static int init_speed = 0;
static int init_collision = NO_BUMP;
static float init_luminosity = 0.0;
static pilot_state_e pilot_state = PILOT_IDLE;
/*-----------------------PRIVATE FUNCTIONS PROTOTYPES-----------------------*/
static void Pilot_new();
static void Pilot_free();
static bool_e Pilot_hasBumped();
static void Pilot_sendMvt(VelocityVector vel);
static VelocityVector Pilot_init_velocity_vector(Direction direction, int power);
static PilotSensors Pilot_init_sensors(int speed, int collision, float luminosity);

/*-----------------------PUBLIC FUNCTIONS-----------------------*/
extern void Pilot_start(){
	Robot_start();
	Pilot_new();
}

extern void Pilot_stop(){
	Robot_stop();
	Pilot_free();
}

extern void Pilot_setVelocity(VelocityVector vel){
	if(vel.dir==STOP){
		Pilot_sendMvt(vel);
		pilot_state = PILOT_IDLE;
	}
	else{
		Pilot_sendMvt(vel);
		pilot_state = PILOT_RUN;
	}
}

extern PilotSensors Pilot_getState(){
	Pilot pilot = *pPilot;
	return pilot.sensors;
}

extern void Pilot_check(){
	bool_e bump = Pilot_hasBumped();
	if(bump){
		pilot_state = PILOT_IDLE;
	}
}

/*-----------------------PRIAVTE FUNCTIONS-----------------------*/

static VelocityVector Pilot_init_velocity_vector(Direction direction, int power){
	VelocityVector velocity_vector = {direction, power};
	return velocity_vector;

}

static PilotSensors Pilot_init_sensors(int speed, int collision, float luminosity){
	PilotSensors pilot_sensors = {speed, collision, luminosity};
	return pilot_sensors;
}

static void Pilot_new(){
	VelocityVector velocity_vector = Pilot_init_velocity_vector(init_direction, init_power);
	PilotSensors pilot_sensors = Pilot_init_sensors(init_speed, init_collision, init_luminosity);
	pPilot = (Pilot *)malloc(sizeof(Pilot));
	pPilot->velocity_vector = velocity_vector;
	pPilot->sensors = pilot_sensors;
}

static void Pilot_free(){
	free(pPilot);
}

extern void Pilot_run(pilot_event_e event, VelocityVector vel){
	switch(event){
		case PILOT_IDLE :
			if(event==EVENT_SET_VELOCITY){ Pilot_setVelocity(vel);}
			else if(event==EVENT_STOP){
				vel.dir = STOP;
				Pilot_sendMvt(vel);
				pilot_state=PILOT_IDLE;
			}
			break;
		case PILOT_RUN :
			if(event==EVENT_SET_VELOCITY){ Pilot_setVelocity(vel);}
			else if(event==EVENT_CHECK){ Pilot_check();}
			else if(event==EVENT_STOP){
				vel.dir = STOP;
				Pilot_sendMvt(vel);
				pilot_state=PILOT_IDLE;
			}
			break;
		case PILOT_EXIT : //TODO : EXIT state entering condition ?
			Pilot_stop();
			break;
		default :
			printf("ERROR : Wrong pilot state provided");
			break;
	}
}

static void Pilot_sendMvt(VelocityVector vel){
	switch(vel.dir){
		case LEFT :		Robot_setWheelsVelocity(-vel.power, vel.power);		break;
		case RIGHT :	Robot_setWheelsVelocity(vel.power, -vel.power);		break;
		case FORWARD :	Robot_setWheelsVelocity(vel.power, vel.power);		break;
		case BACKWARD :	Robot_setWheelsVelocity(-vel.power, -vel.power);	break;
		default:	printf("ERROR : wrong direction provided in pilot");	break;
	}
}

static bool_e Pilot_hasBumped(){
	if(Robot_getSensorState().collision){return TRUE;}
	else{return FALSE;}
}


   
       
   

