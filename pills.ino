/************************************************************************
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Author: Pedro Barcha
**************************************************************************/    

#include <DS3232RTC.h>    //http://github.com/JChristensen/DS3232RTC
#include <CustomStepper.h> //https://playground.arduino.cc/Main/CustomStepper
#include <EEPROM.h>
 
//clocker pins should be SDA->A4 and SCL->A5, according to its library.

//led pin
const int led_pin=17;

//stepper pins
const int N1=2, N2=3, N3=4, N4=5;
CustomStepper stepper(N1, N2, N3, N4);

//cases pins:
//which verifier pin corresponds to each hour (as each pill case represents one hour);
//note, in arduino/elegoo , pins 14, 15, 16 and 17 are the digital reads of A0, A1, A2, A3, respectively.
const int cases_pins[]={6,7,8,9,10,11,12,13,14,15,16}; //[0]=1h pin , [1]=2h pin, ...

const int pill_cases = 12; //one case for each hour of the day (am/pm standard);

//if the user doesn't grab the pills in time T, the led blinks in this minutes period, after T
const int blinking_thresholds[]={5,15};

int seconds = 0;
int led_position;// case/hour to which led is pointing to
float degrees_to_rotate;

bool rotate = false;
bool already_checked = false;

void setup()
{
  Serial.begin(9600);

  //set clock and check its status
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  if(timeStatus() != timeSet) 
      Serial.println("Unable to sync with the RTC");
  else
      Serial.println("RTC has set the system time");

 //set the pills cases that verifies if there are pills to be taken each hour
 for (int i = 0; i < pill_cases; i++)
   pinMode(cases_pins[i], INPUT);

 //set led
 pinMode(led_pin, OUTPUT);
 digitalWrite(led_pin, LOW);

 //get led position from EEPROM
 led_position=EEPROM.read(0);
}

void loop()
{  
  //check if there are pills to be taken
  checkPills();

  //if so, then rotate the lamp and turn it on
  if (rotate && stepper.isDone())
    setLamp();

  //makes the motor steps when necessary
    stepper.run();
}

//if it is a round hour (1:00,2:00,...), check if there are pills to be taken
void checkPills()
{
  //check if it is a round hour (1:00,2:00,...)
  if (minute()==0 && second()==0 && !alreay_checked)
  { 
    if ((convertHour() <= 11) &&  digitalRead(cases_pins[convertHour()-1])) //check if there are pills to be taken this hour (circuit is closed)
    {
      already_checked = true;
      rotate = true;
      calculateRotation (convertHour());
    }
  }
  else if (!(minute()==0 && second==0)) //this avoids entering the above condition 2 times in the same second gap
    already_checked = false;
}

//calculate LED rotation degrees and set it to turn wise/clockwise
void calculateRotation (int hour)
{ 
  degrees_to_rotate = abs(hour-led_position) * (360/pill_cases);
  
  if (hour > led_position)
    stepper.setDirection(CW); //clockwise  
  else
    stepper.setDirection(CCW); //counter clockwise

  led_position = hour;

  //write led position to EEPROM, so when arduino goes out of power, its value is still stored
  EEPROM.write(0,led_position);
}

//repositionate LED and set its blinking patterns
void setLamp()
{
  stepper.rotateDegrees(degrees_to_rotate); //start to rotate the led
  rotate=false;

  //first of all the led goes on steadly
  digitalWrite(led_pin, HIGH);

  //if the pills aren't taken after blinking_thresholds[0] minutes, the led starts to blink.
  while (digitalRead(cases_pins[led_position-1]) && minute() < blinking_thresholds[1])
  {
    //makes the motor steps when necessary
    stepper.run();
    
    if (minute() >= blinking_thresholds[0])
    {
      if (digitalRead(led_pin))
        digitalWrite(led_pin,LOW);
      else
        digitalWrite(led_pin,HIGH);

      delay(1000);
    }
  }

  //the led goes off, even if the pills aren't taken after blinking_thresholds[1]
  digitalWrite(led_pin, LOW);
}

//convert hour from 24h format (0h-23h) to am/pm
int convertHour()
{
  int hr= hour();

  if ((hr >= 1) && (hr <= 12))
    return hr;
  else if ((hr >= 13) && (hr <= 23))
    return ((hr%12));
  else //hr=0
    return 12;
}

