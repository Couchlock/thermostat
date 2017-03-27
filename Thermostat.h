/*
Name:		Thermostat.h
Created:	3/22/2017
Author:	Jason Williams
*/
#ifndef THERMOSTAT_H_
#define THERMOSTAT_H_

#include "Arduino.h"
#define AREF_SIGNAL 3.3	//needs to be adjusted to actual aref voltage
enum { LEFT, RIGHT };

class Thermostat
{
private:
	const byte SENSOR_PIN;
	const byte COOL_PIN;
	const byte HEAT_PIN;
	byte coolPinState;
	byte heatPinState;
	byte tempType;
	byte currentTemp;
	unsigned int poll;	
	byte targetTemp;
	bool runState;
	byte tempBuffer;
	bool _running;
public:
	Thermostat(const byte& sp, const byte& cp, const byte& hp, byte tarTemp = 23, const byte tType = 1, const unsigned int p = 120000);
	void readSensor();
	void displayTemp();
	bool pollTimer(double& m);
	void setRunState();
	void changeTargetTemp(int d);
	void climateControl();
	void setTime();
};


#endif
