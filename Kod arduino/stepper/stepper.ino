#include <Servo.h>

#define PRINTTIME  500
#define STEPTIME 5
#define TOLERANCE 15
#define SERVOTIME 20
#define READTIME 2

#define LEFT_DOWN_PIN A2
#define RIGHT_DOWN_PIN A0
#define LEFT_UP_PIN A3
#define RIGHT_UP_PIN A1
#define SERVOPIN A4
#define DIRECTION 3
#define STP 2

Servo serwo;
int16_t Left_Up_Value = 0, Right_Up_Value = 0, Left_Down_Value = 0, Right_Down_Value = 0;
uint16_t HorizontalDifference = 0, VerticalDifference = 0;
int16_t StepCounter = 0, SumRight = 0, SumLeft = 0, DownSum = 0, UpSum = 0;
uint32_t ActualTime = 0, PreviousTimeStep = 0, PreviousTimePrint = 0, PreviousTimeServo = 0, PreviousTimeRead = 0;
bool state = LOW;
uint8_t sposition = 90;

void setup() {
  // put your setup code here, to run once:
  pinMode(DIRECTION, OUTPUT);
  digitalWrite(DIRECTION, HIGH);
  pinMode(STP, OUTPUT);
  Serial.begin(9600);
  serwo.attach(SERVOPIN);
  serwo.write(sposition);
  delay(500);
}

void ReadLight();
void SetServoPosition();
void SetStepperPosition();

void loop() {
  ActualTime = millis();
  if( (ActualTime - PreviousTimeRead) >= READTIME )
  {
    PreviousTimeRead = ActualTime;
    ReadLight();
    Calculate();
  }
  if( (ActualTime - PreviousTimeStep) >= STEPTIME )
  {
    PreviousTimeStep = ActualTime;
    SetStepperPosition();
  }
  
  if( (ActualTime - PreviousTimeServo) >= SERVOTIME )
  {
    PreviousTimeServo = ActualTime;
    SetServoPosition();
  }
  /*
  if( (ActualTime - PreviousTimePrint) >= PRINTTIME )
  {
    PreviousTimePrint = ActualTime;
    Serial.println(String("") + "Step counter: " + StepCounter/2);
  }
  */
}

void ReadLight()
{
    Left_Down_Value = analogRead(LEFT_DOWN_PIN);
    Right_Down_Value = analogRead(RIGHT_DOWN_PIN);
    Left_Up_Value = analogRead(LEFT_UP_PIN);
    Right_Up_Value = analogRead(RIGHT_UP_PIN);
}

void Calculate()
{
    SumRight = Right_Up_Value + Right_Down_Value;
    SumLeft = Left_Up_Value + Left_Down_Value;
    HorizontalDifference = abs(SumRight - SumLeft);
    
    UpSum = Right_Up_Value + Left_Up_Value;
    DownSum = Left_Down_Value + Right_Down_Value;
    VerticalDifference = abs(UpSum - DownSum);
}
void SetStepperPosition()
{
    if ( HorizontalDifference > TOLERANCE)
    {
      if (SumRight > SumLeft)
      {
        digitalWrite(DIRECTION, HIGH);
        ++StepCounter;
      }
      else if (SumRight < SumLeft)
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

void SetServoPosition()
{
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
    //Serial.println(String("") + "UpSum" + UpSum + " DownSum " + DownSum + " Vertical Difference " + 
                  //VerticalDifference + " Horizonatal difference: " + HorizontalDifference + 
                  //" Step Counter: " + StepCounter);
}
