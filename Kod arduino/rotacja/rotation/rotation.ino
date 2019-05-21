#include <Servo.h>
#include <limits.h>

#define PRINTTIME  1000
#define STEPTIME 5
#define TOLERANCE 15
#define SERVOTIME 10
#define READTIME 4

#define LEFT_DOWN_PIN A0
#define RIGHT_DOWN_PIN A1
#define LEFT_UP_PIN A2
#define RIGHT_UP_PIN A3
#define SERVOPIN A4
#define DIRECTION 6
#define STP 7

Servo serwo;
volatile int16_t Left_Up_Value = 0, Right_Up_Value = 0, Left_Down_Value = 0, Right_Down_Value = 0;
volatile int16_t values[4];
int16_t StepCounter = 0;
uint32_t ActualTime = 0, PreviousTimeStep = 0, PreviousTimePrint = 0, PreviousTimeServo = 0, PreviousTimeRead = 0;
bool state = LOW;
uint8_t sposition = 90 , pos = 0;
int16_t upMax, downMax, leftMax, rightMax;

void setup() {
  // put your setup code here, to run once:
  pinMode(DIRECTION, OUTPUT);
  digitalWrite(DIRECTION, HIGH);
  pinMode(STP, OUTPUT);
  Serial.begin(2000000);
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
    pos = FindMax();
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
    values[0] = Left_Down_Value;
    values[1] = Right_Down_Value;
    values[2] = Left_Up_Value;
    values[3] = Right_Up_Value;
    //Serial.println(String("") + "Left_Down_Value " + Left_Down_Value + " Right_Down_Value " + Right_Down_Value + " Left_Up_Value " + 
                  //Left_Up_Value + " Right_Up_Value " + Right_Up_Value);
}

void SetStepperPosition()
{   
  if(HorizontalDiff())
  {
  //left max
    if ((pos == 0) || (pos == 2))
    {
      digitalWrite(DIRECTION, HIGH);
      ++StepCounter;
    }
    else
    {
      digitalWrite(DIRECTION, LOW);
      --StepCounter;
    }
    digitalWrite(STP,state);
    state = !state;
  }
}

void SetServoPosition()
{
  if(VerticalDiff())
  {
    //down position max
    if ( (pos == 0) || (pos == 1) )
    {
      if(sposition > 0)
        serwo.write(--sposition);
    }
    else
    {
      if(sposition < 180)
        serwo.write(++sposition);
    }
    //Serial.println(String("") + "UpSum" + UpSum + " DownSum " + DownSum + " Vertical Difference " + 
                  //VerticalDifference + " Horizonatal difference: " + HorizontalDifference + 
                  //" Step Counter: " + StepCounter + " sposition: " + sposition + " pos " + pos); 
  }
}

int16_t FindMax()
{
  int max_v = INT_MIN;
  int max_i = 0;
  for ( int i = 0; i < sizeof(values)/sizeof(values[0]); i++ )
  {
    if ( values[i] > max_v )
    {
      max_v = values[i];
      max_i = i;
    }
  }
  return max_i;
}

inline bool VerticalDiff()
{
  upMax = ( values[2] > values[3] ? values[2] : values[3] );
  downMax = ( values[0] > values[1] ? values[0] : values[1] );
  return ( abs(upMax - downMax) > TOLERANCE ? true : false );
}

inline bool HorizontalDiff()
{
  leftMax = ( values[0] > values[2] ? values[0] : values[2] );
  rightMax = ( values[1] > values[3] ? values[1] : values[3]);
  return ( abs(leftMax - rightMax) > TOLERANCE ? true : false);
}
