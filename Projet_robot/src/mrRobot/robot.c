#include "robot.h"
#include "prose.h"
#include "defs.h"
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#define LEFT_MOTOR MD
#define RIGHT_MOTOR MA
#define LIGHT_SENSOR S1
#define FRONT_BUMPER S3
#define FLOOR_SENSOR S2

static char address[] = "127.0.0.1";
static int port = 12345;
static LegoMotor right_motor_port = RIGHT_MOTOR;
static LegoMotor left_motor_port = LEFT_MOTOR;
static LegoSensor luminosity_sensor_port = LIGHT_SENSOR;
static LegoSensor contact_sensor_port = FRONT_BUMPER;
static Robot *pRobot;
/*-----------------------PRIVATE FUNCTIONS PROTOTYPES-----------------------*/
static void Robot_new();
static void Robot_free();

/*-----------------------PUBLIC FUNCTIONS-----------------------*/
extern void Robot_start(){
	if (ProSE_Intox_init(address, port) == -1) {
		PProseError("Problème d'initialisation du simulateur Intox");
	}
	Robot_new();
}

extern void Robot_stop(){
	Robot_free();
	ProSE_Intox_close();
}

extern int Robot_getRobotSpeed(){
	int speed;
	Robot robot = *pRobot;
	Cmd m1 = Motor_getCmd(robot.motors.right_motor);
	Cmd m2 = Motor_getCmd(robot.motors.left_motor);
	speed = (int)((abs(m1)+abs(m2))/2);
	return speed;
}

extern RobotSensors Robot_getSensorState(){
	Robot robot = *pRobot;
	robot.sensors.collision = ContactSensor_getStatus(robot.sensors.contact_sensor); //TODO : convert initial sensor data to Collision type ?
	robot.sensors.luminosity = LightSensor_getStatus(robot.sensors.light_sensor);
	return robot.sensors;
}

extern void Robot_setWheelsVelocity(int mr,int ml){
	Robot robot = *pRobot;
	Cmd cmdM1 = mr; Cmd cmdM2 = ml;
	if (Motor_setCmd(robot.motors.right_motor, cmdM1) == -1) {PProseError("Problème de commande du moteur droit");}
	if (Motor_setCmd(robot.motors.left_motor, cmdM2) == -1) {PProseError("Problème de commande du moteur droit");}
}

/*-----------------------PRIVATE FUNCTIONS-----------------------*/
static RobotSensors Robot_init_sensors(LegoSensor light_sensor_port, LegoSensor contact_sensor_port){
	LightSensor* light_sensor = LightSensor_open(light_sensor_port);
	if (light_sensor == NULL) PProseError("Couldn't open light_sensor_port");
	ContactSensor* contact_sensor = ContactSensor_open(contact_sensor_port);
	if (contact_sensor == NULL) PProseError("Couldn't open contact_sensor_port");
	RobotSensors robotSensors = {NO_BUMP, 0.0,light_sensor, contact_sensor};
	return robotSensors;
}

static RobotMotors Robot_init_motors(LegoMotor right_motor_port, LegoMotor left_motor_port){
	RobotMotors robotMotors;
	robotMotors.right_motor = Motor_open(right_motor_port);
	if (robotMotors.right_motor == NULL) PProseError("Couldn't open right motor port");
	robotMotors.left_motor = Motor_open(left_motor_port);
	if (robotMotors.left_motor == NULL) PProseError("Couldn't open left motor port");
	return robotMotors;
}

static void Robot_new(){
	RobotMotors robotMotors = Robot_init_motors(right_motor_port, left_motor_port);
	RobotSensors robotSensors = Robot_init_sensors(luminosity_sensor_port, contact_sensor_port);
	pRobot = (Robot *)malloc(sizeof(Robot));
	pRobot->sensors = robotSensors;
	pRobot->motors = robotMotors;
}

static void Robot_free(){
	Robot robot = *pRobot;
	Motor_close(robot.motors.right_motor);
	Motor_close(robot.motors.left_motor);
	free(pRobot);
}
