// COMP-10184 - Mohawk College
// COMP-10184_AlarmSystem
//
// This program simulates a real world case of motion detector for a home alarm system.
//
// @author  Darius Luft
// @id      000798737
//
// I Darius Luft, 000798737, certify that all code submitted is my own work; that I have not copied 
// it from any other source. I also certify that I have not allowed my work to be copied by others. 
//

#include <Arduino.h>

// Motion Dector 
#define PIN_PIR D5 
// Physical Button
#define PIN_BUTTON D6
// Alarm disabled state variable
#define ALARM_DISABLED 0
// Alarm enabled state variable
#define ALARM_ENABLED 1
// Alarm countdown state variable
#define ALARM_COUNTDOWN 2
// Alarm active state variable
#define ALARM_ACTIVE 3
// Delay for a button press (to capture more accurate input)
#define BUTTON_DELAY 200
// LED on state variable
#define LED_ON false
// LED off state variable
#define LED_OFF true
// Delay LED by 1 millisecond (used for blinking)
#define LED_DELAY 1 

// Variable to hold the alarm state
int iAlarmState;
// Variable to hold the LED sate
bool bLEDState;


// ************************************
// Collect the inputs for the arlarm system
// ************************************
void collectInputs()
{
  bool bPIR;
  int iButton;
  bPIR = digitalRead(PIN_PIR);
  iButton = digitalRead(PIN_BUTTON);
  

  if (iAlarmState == ALARM_ENABLED && bPIR)
    iAlarmState = ALARM_COUNTDOWN;
  else if (iAlarmState == ALARM_COUNTDOWN && iButton == 0)
  {
    delay(BUTTON_DELAY);
    iAlarmState = ALARM_DISABLED;
  }
  else if (iAlarmState == ALARM_DISABLED && iButton == 0)
  {
    delay(BUTTON_DELAY);
    iAlarmState = ALARM_ENABLED;
  }
}

// ************************************
// Toggle the LED on and off
// ************************************
void toggleLED()
{
  if (!bLEDState)
    bLEDState = LED_OFF;
  else
    bLEDState = LED_ON;
}

// ************************************
// Manages the different states of the alarm system
// ************************************
void manageStates()
{
  switch (iAlarmState)
  {
    case ALARM_DISABLED:
      digitalWrite(LED_BUILTIN, LED_OFF);
      collectInputs();
      break;
   
    case ALARM_ENABLED:
      collectInputs();
      break;
   
    case ALARM_COUNTDOWN:
      for (int i = 0; i < 10000; i++)
      {
        if (i % 125 == 0 || i == 0)
          toggleLED();

        digitalWrite(LED_BUILTIN, bLEDState);

        collectInputs();
        if (iAlarmState != ALARM_COUNTDOWN)
          break;
        delay(LED_DELAY);     
      }
      // If alarm was disabled before countdown ended
      if (iAlarmState == ALARM_DISABLED)
        break;

      // If countdown has run out  
      iAlarmState = ALARM_ACTIVE;
      break;
    
    case ALARM_ACTIVE:
      digitalWrite(LED_BUILTIN, LED_ON);
      break;
  }
}


void setup() {
  // configure the USB serial monitor 
  Serial.begin(115200); 
 
  // configure the LED output 
  pinMode(LED_BUILTIN, OUTPUT); 
 
  // PIR sensor is an INPUT 
  pinMode(PIN_PIR, INPUT); 
 
  // Button is an INPUT 
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  // Set alarm to enabled
  iAlarmState = ALARM_ENABLED;
  bLEDState = LED_OFF;
  digitalWrite(LED_BUILTIN, bLEDState); 
}


void loop() {
  // put your main code here, to run repeatedly:
  manageStates();
}



