// Thermostat.cpp -- implementation of Thermostat class
#include "Thermostat.h"

Thermostat::Thermostat(const byte& sp, const byte& cp, const byte& hp, byte tarTemp, const byte tType, const unsigned int poll)
	: SENSOR_PIN(sp), COOL_PIN(cp), HEAT_PIN(hp), tempType(tType), poll(poll)
{
	coolPinState = false;
	heatPinState = false;
	runState = false;
	_running = false;
	readSensor();//throw away
	
	if (tempType == 0)
	{
		targetTemp = tarTemp;
		tempBuffer = 1;
	}
	else if (tempType == 1)
	{
		targetTemp = tarTemp * 1.8 + 32;
		tempBuffer = 3;
	}
}
	

void Thermostat::readSensor()
{
	const byte Size = 100;
	unsigned short rawRead[Size];
	unsigned short avgRead = 0;
	for (byte i = 0; i < Size; i++)
	{
		rawRead[i] = analogRead(SENSOR_PIN);
		avgRead += rawRead[i];
	}
	avgRead /= Size;
	float voltage = (avgRead * AREF_SIGNAL / 1024) - 0.495; //Ov * Iv / range of sensor) - (voltage offset of tmp36)
	if (tempType == 0)	//if C
		currentTemp = voltage * 100;
	else if (tempType == 1)	// if F
		currentTemp = (byte)((voltage * 100) * 1.8 + 32) + 0.5;
}

bool Thermostat::pollTimer(double& lM)
{
	if (millis() - lM >= poll)
	{
		lM = millis();
		return true;
	}
	return false;
}

void Thermostat::setRunState()
{
	if (runState == false)
		runState = true;
	else if (runState == true)
	{
		runState = false;
		digitalWrite(COOL_PIN, LOW);
		digitalWrite(HEAT_PIN, LOW);
	}
	displayTemp();
}

void Thermostat::displayTemp()	//replace this with code for an actual display
{
	readSensor();
	Serial.print("Current Temp: "); Serial.println(currentTemp);
	if (runState == true)
	{
		Serial.print("Target: "); Serial.println(targetTemp);
		if (coolPinState == LOW && heatPinState == LOW)
			Serial.println("ON");
		if (coolPinState == HIGH)
			Serial.println("AC On");
		if (heatPinState == HIGH)
			Serial.println("Heat On");
	}
	else if (runState == false)
		Serial.println("OFF");
}

void Thermostat::changeTargetTemp(int d)
{
	if (runState == true)
	{
		if (d < 0)
			targetTemp -= 1;
		if (d > 0)
			targetTemp += 1;
		Serial.print("Target: "); Serial.println(targetTemp);
	}
	readSensor();
	climateControl();
}

void Thermostat::climateControl()
{
	if (runState == false)
	{
		digitalWrite(COOL_PIN, LOW);
		digitalWrite(HEAT_PIN, LOW);
		_running = false;
	}
	if (runState == true)
	{
		if (_running == false)
		{
			if ((currentTemp > targetTemp) && (currentTemp - targetTemp) > tempBuffer)//turn ac on
			{
				digitalWrite(COOL_PIN, HIGH);
				_running = true;
			}
			if ((targetTemp > currentTemp) && (targetTemp - currentTemp) > (tempBuffer))//turn heat on
			{
				digitalWrite(HEAT_PIN, HIGH);
				_running = true;
			}
		}
		if (_running == true)
		{
			if ((targetTemp - currentTemp) >= (tempBuffer-1))
			{
				digitalWrite(COOL_PIN, LOW);//turn ac off
				_running = false;
			}
			if ((currentTemp - targetTemp) >= (tempBuffer - 1))
			{
				digitalWrite(HEAT_PIN, LOW);//turn heat off
				_running = false;
			}
		}		
	}
	coolPinState = digitalRead(COOL_PIN);
	heatPinState = digitalRead(HEAT_PIN);
	displayTemp();
}

void Thermostat::setTime()
{

}