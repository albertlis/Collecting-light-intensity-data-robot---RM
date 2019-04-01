#define DIRECTION 3
#define STP 2
#define MAXSPEED 50.0
#define PRINTTIME  500
#define STEPTIME 10
#define TOLERANCE 10
#define LEFTPIN A0
#define RIGHTPIN A1

int16_t LeftValue = 0, RightValue = 0;
uint16_t difference = 0;
int16_t StepCounter = 0;
uint32_t ActualTime = 0, PreviousTimeStep = 0, PreviousTimePrint = 0;
bool state = LOW;

void setup() {
  // put your setup code here, to run once:
  pinMode(DIRECTION, OUTPUT);
  pinMode(STP, OUTPUT);
  digitalWrite(DIRECTION, HIGH);
  Serial.begin(500000);
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void loop() {
  ActualTime = millis();
  if( (ActualTime - PreviousTimeStep) >= STEPTIME )
  {
    PreviousTimeStep = ActualTime;
    LeftValue = analogRead(LEFTPIN);
    RightValue = analogRead(RIGHTPIN);
    difference = abs(LeftValue - RightValue);
    if ( difference > TOLERANCE)
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

  if( (ActualTime - PreviousTimePrint) >= PRINTTIME )
  {
    PreviousTimePrint = ActualTime;
    Serial.print("Lewa: ");
    Serial.print(LeftValue);
    Serial.print(" Prawa: ");
    Serial.print(RightValue);
    Serial.print(" Różnica: ");
    Serial.print(difference);
    Serial.print(" StepCounter: ");
    Serial.println(StepCounter/2); //2 addons for one step
  }
}
