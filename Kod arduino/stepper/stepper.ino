#include <Servo.h>

#define STP 2
#define MAXSPEED 50.0
#define PRINTTIME  500
#define STEPTIME 10
#define TOLERANCE 10
#define SERVOTIME 10
#define LEFTPIN A0
#define RIGHTPIN A1
#define UPPIN A2
#define DOWNPIN A3
#define SERVOPIN A8
#define DIRECTION 3

Servo serwo;
int16_t LeftValue = 0, RightValue = 0, UpValue = 0, DownValue = 0;
uint16_t HorizontalDifference = 0, VerticalDifference = 0;
int16_t StepCounter = 0;
uint32_t ActualTime = 0, PreviousTimeStep = 0, PreviousTimePrint = 0, PreviousTimeServo = 0;
bool state = LOW;
uint8_t sposition = 90;

void setup() {
  // put your setup code here, to run once:
  pinMode(DIRECTION, OUTPUT);
  pinMode(STP, OUTPUT);
  digitalWrite(DIRECTION, HIGH);
  Serial.begin(500000);
  serwo.attach(SERVOPIN);
  serwo.write(sposition);
}
/*
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
*/
void loop() {
  ActualTime = millis();
  if( (ActualTime - PreviousTimeStep) >= STEPTIME )
  {
    PreviousTimeStep = ActualTime;
    LeftValue = analogRead(LEFTPIN);
    RightValue = analogRead(RIGHTPIN);
    HorizontalDifference = abs(LeftValue - RightValue);
    if ( HorizontalDifference > TOLERANCE)
    {
      if (RightValue > LeftValue)
      {
        digitalWrite(DIRECTION, HIGH);
        ++StepCounter;
      }
      else if (RightValue < LeftValue)
      {
        digitalWrite(DIRECTION, LOW);
        --StepCounter;
      }
      digitalWrite(STP,state);
      state = !state;
    }
    else
      StepCounter = 0;
  }

  if( (ActualTime - PreviousTimeServo) >= SERVOTIME )
  {
    PreviousTimeServo = ActualTime;
    UpValue = analogRead(UPPIN);
    DownValue = analogRead(DOWNPIN);
    VerticalDifference = abs(UpValue - DownValue);
    if ( VerticalDifference > TOLERANCE)
    {
      if (UpValue > DownValue)
      {
        if(sposition < 180)
          serwo.write(++sposition);
      }
      else if (UpValue < DownValue)
      {
        if(sposition > 0)
          serwo.write(--sposition);
      }
    }
  }
  
  if( (ActualTime - PreviousTimePrint) >= PRINTTIME )
  {
    PreviousTimePrint = ActualTime;
    Serial.println(String("") + "Lewa: " + LeftValue + " Prawa: " + RightValue + " Różnica l|p: " + HorizontalDifference + " StepCounter: " + StepCounter/2);
  }
}
