/*
 Name:		Thermostat.ino
 Created:	3/22/2017
 Author:	Jason Williams
*/
#include "Thermostat.h"
const byte SENSOR_PIN = A0;
const byte COOL_PIN = 4;
const byte HEAT_PIN = 5;
const byte TEMP_TYPE = 1;
const byte BUTTONS[2] = { 2, 3 };
Thermostat therm(SENSOR_PIN, COOL_PIN, HEAT_PIN, 23, TEMP_TYPE);
double lastMillis = 0;

////buttonstuffs////
const byte NUM_BUT = 2;
const byte DEBOUNCE_DELAY = 85;
const unsigned short BUTTON_HOLD_TIME = 1000;
volatile byte buttonState[NUM_BUT] = { LOW, LOW };
volatile byte prevButtonState[NUM_BUT];
unsigned long lastButtonTime = 0;
unsigned long lastDebounceTime = 0;
inline unsigned long debouncer() { return millis() - lastDebounceTime; }
void setButtonState();
void readButtonState();
void checkButtonPress();


void setup() 
{
	Serial.begin(9600);
	analogReference(EXTERNAL);
	pinMode(LED_BUILTIN, OUTPUT);
	for (byte i = 0; i < NUM_BUT; i++)
		pinMode(BUTTONS[i], INPUT_PULLUP);
	
}

void loop() 
{
	therm.readSensor();
	therm.displayTemp();
	byte i = 0;
	while (i < 1)
	{
		setButtonState();
		readButtonState();
		checkButtonPress();
		if (therm.pollTimer(lastMillis))
		{
			therm.climateControl();
		}
	}
}

void setButtonState()
{
	for (byte i = 0; i < NUM_BUT; i++)
		prevButtonState[i] = HIGH;
	if (buttonState[LEFT] == HIGH && buttonState[RIGHT] == HIGH)
	{
		lastDebounceTime = millis();
		lastButtonTime = millis();
	}
}

void readButtonState()
{
	buttonState[LEFT] = digitalRead(BUTTONS[LEFT]);
	buttonState[RIGHT] = digitalRead(BUTTONS[RIGHT]);
}

void checkButtonPress()
{
	if (debouncer() >= DEBOUNCE_DELAY && (buttonState[LEFT] == LOW || buttonState[RIGHT] == LOW))
	{
		while (buttonState[LEFT] == LOW || buttonState[RIGHT] == LOW)//as long as atleast one button is still pressed lets loop..
		{
			//read both button states again and store in prevButtonState array
			prevButtonState[LEFT] = buttonState[LEFT];
			prevButtonState[RIGHT] = buttonState[RIGHT];
			buttonState[LEFT] = digitalRead(BUTTONS[LEFT]);
			buttonState[RIGHT] = digitalRead(BUTTONS[RIGHT]);
			//if BUTTONS[LEFT] was pressed but is no longer pressed and its been less than HOLD_TIME...
			if (buttonState[LEFT] != prevButtonState[LEFT] && (millis() - lastButtonTime) < BUTTON_HOLD_TIME)
				therm.changeTargetTemp(-1);
			else if (buttonState[RIGHT] != prevButtonState[RIGHT] && (millis() - lastButtonTime) < BUTTON_HOLD_TIME)//same for BUTTONS[RIGHT]
				therm.changeTargetTemp(1);
			if (buttonState[LEFT] == LOW && buttonState[RIGHT] == HIGH && (debouncer() >= BUTTON_HOLD_TIME))//if we've eclipsed the hold time for left button...
			{
				therm.setRunState();
				while (buttonState[LEFT] == LOW) //stay in loop until button released so as not to repeat press.
					buttonState[LEFT] = digitalRead(BUTTONS[LEFT]);
			}
			else if (buttonState[LEFT] == HIGH && buttonState[RIGHT] == LOW && (debouncer() >= BUTTON_HOLD_TIME))//or if we've eclipsed the hold time for right button...
			{
				Serial.println("BUTTONS[RIGHT] hold");  //replace
				while (buttonState[RIGHT] == LOW)  //stay in loop until button released so as not to repeat press
					buttonState[RIGHT] = digitalRead(BUTTONS[RIGHT]);
			}
		}
	}
}
