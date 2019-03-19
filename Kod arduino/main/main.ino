#include <Servo.h> // include Servo library 

Servo horizontal; // horizontal servo
int servoh = 90; // stand horizontal servo

Servo vertical; // vertical servo 
int servov = 90; // stand vertical servo

// LDR pin connections
// name = analogpin;
int lup = 0; //up left
int rup = 1; //up rigt
int ldwn = 2; //down left
int rdwn = 3; //down rigt
//analog 4 potencjometr 1
//analog 5 potencjometr 2

void setup()
{
  Serial.begin(9600);
// servo connections
// name.attacht(pin);
  horizontal.attach(9); 
  vertical.attach(10);
}

void loop() 
{
  int lt = analogRead(lup); // top left
  int rt = analogRead(rup); // top right
  int ld = analogRead(ldwn); // down left
  int rd = analogRead(rdwn); // down rigt

  int delay_time = analogRead(4)/20; //ustawiamy opóźnienie
  int tol = analogRead(5)/4;  //ustawiamy tolerancję błędu
  
  int avt = (lt + rt) / 2; // average value top
  int avd = (ld + rd) / 2; // average value down
  int avl = (lt + ld) / 2; // average value left
  int avr = (rt + rd) / 2; // average value right
  
  int dvert = avt - avd; // check the diffirence of up and down
  int dhoriz = avl - avr;// check the diffirence og left and rigt
  
  if (-1*tol > dvert || dvert > tol) // check if the diffirence is in the tolerance else change vertical angle
  {
    if (avt > avd)
    {
      servov = ++servov;
      if (servov > 180)
        servov = 180;
    }
    else if (avt < avd)
    {
      servov= --servov;
      if (servov < 0)
        servov = 0;
    }
    vertical.write(servov);
  }
  
  if (-1*tol > dhoriz || dhoriz > tol) // check if the diffirence is in the tolerance else change horizontal angle
  {
    if (avl > avr)
    {
      servoh = --servoh;
      if (servoh < 0)
        servoh = 0;
    }
    else if (avl < avr)
    {
      servoh = ++servoh;
      if (servoh > 180)
        servoh = 180;
    }
  
  horizontal.write(servoh);
  }
  delay(delay_time);
} 
