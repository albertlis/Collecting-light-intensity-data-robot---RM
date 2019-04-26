#include <Servo.h>

#define STP 2
#define MAXSPEED 50.0
#define PRINTTIME  500
#define STEPTIME 10
#define TOLERANCE 10
#define SERVOTIME 10

#define LEFT_DOWN_PIN A2
#define RIGHT_DOWN_PIN A0
#define LEFT_UP_PIN A3
#define RIGHT_UP_PIN A1
#define SERVOPIN A8
#define DIRECTION 3

Servo serwo;
int16_t Left_Up_Value = 0, Right_Up_Value = 0, Left_Down_Value = 0, Right_Down_Value = 0;
uint16_t HorizontalDifference = 0, VerticalDifference = 0;
int16_t StepCounter = 0;
uint32_t ActualTime = 0, PreviousTimeStep = 0, PreviousTimePrint = 0, PreviousTimeServo = 0;
bool state = LOW;
uint8_t sposition = 90;

void setup() {
  // put your setup code here, to run once:
  pinMode(DIRECTION, OUTPUT);
  digitalWrite(DIRECTION, HIGH);
  pinMode(STP, OUTPUT);
  Serial.begin(500000);
  serwo.attach(SERVOPIN);
  //serwo.write(sposition);
}

void ReadLight();
void SetServoPosition();
//void SetStepperPosition();

void loop() {
  ActualTime = millis();
  if( (ActualTime - PreviousTimeStep) >= STEPTIME )
  {
    PreviousTimeStep = ActualTime;
    ReadLight();
    //SetStepperPosition();
  }
  
  if( (ActualTime - PreviousTimeServo) >= SERVOTIME )
  {
    PreviousTimeServo = ActualTime;
    ReadLight();
    SetServoPosition();
    //serwo.write(170);
  }
  
  if( (ActualTime - PreviousTimePrint) >= PRINTTIME )
  {
    PreviousTimePrint = ActualTime;
    //Serial.println(String("") + "Lewa Up: " + Left_Up_Value + " Prawa Up: " + Right_Up_Value + " Left down: " + Left_Down_Value + " Right Down: " + Right_Down_Value);
  }
}






void ReadLight()
{
    Left_Down_Value = analogRead(LEFT_DOWN_PIN);
    Right_Down_Value = analogRead(RIGHT_DOWN_PIN);
    Left_Up_Value = analogRead(LEFT_UP_PIN);
    Right_Up_Value = analogRead(RIGHT_UP_PIN);
}
/*
void SetStepperPosition()
{
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
*/
void SetServoPosition()
{
    int16_t UpSum = Right_Up_Value + Left_Up_Value;
    int16_t DownSum = Left_Down_Value + Right_Down_Value;
    VerticalDifference = abs(UpSum - DownSum);
    if ( VerticalDifference > TOLERANCE)
    {
      if (UpSum > DownSum)
      {
        if(sposition > 0)
          serwo.write(--sposition);
      }
      else if (UpSum < DownSum)
      {
        if(sposition < 180)
          serwo.write(++sposition);
      }
    }
    Serial.println(String("") + "UpSum" + UpSum + " DownSum " + DownSum + " Difference " + VerticalDifference);
}
