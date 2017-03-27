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
	const byte SENSOR_PIN;			//pin for reading temp sensor
	const byte COOL_PIN;			//pin for turning on AC
	const byte HEAT_PIN;			//pin for turning on heat
	byte coolPinState;			//for reading state of COOL_PIN
	byte heatPinState;			//for reading state of HEAT_PIN
	byte tempType;				//for storing whether C or F
	byte currentTemp;			//stores current temperature(no decimal, can change to float)
	unsigned int poll;			//time between checking temp(in millis)
	byte targetTemp;			//temp we want the room to be
	bool runState;				//stores whether thermostat is on or off
	byte tempBuffer;			//buffer between targetTemp and currentTemp to turn therm on or off
	bool _running;				//stores whether thermostat is actively cooling/heating
public:
	//constructor
	Thermostat(const byte& sp, const byte& cp, const byte& hp, byte tarTemp = 23, const byte tType = 1, const unsigned int p = 120000);
	void readSensor();			//get temperature from sensor, does conversions
	void displayTemp();			//temporary, to display the temperature to serial monitor
	bool pollTimer(double& m);		//determines if we've reached the poll time to check temperature again
	void setRunState();			//turns thermostat on or off (not heat/cooling)
	void changeTargetTemp(int d);		//adjust desired temperature up or down
	void climateControl();			//turns heat/cooling on/off
	void setTime();				//for setting date/time(not implemented yet
};


#endif
