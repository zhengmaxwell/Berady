#include <Stepper.h>
#include <HCSR04.h>

#define RED 3
#define GREEN 4
#define btnReset 12
#define btnTest 2
#define btnDetect 7
#define TRIG 5
#define ECHO 6

#define forceSensor A0

const int stepsPerRevolution = 800;
const int motorSpeed = 20;
const int threshold = 20;

int state = 0; // 0 - beginning; 1 - test; 2 - detect; 3 - bready;
float height;
float initialHeight = -1;
float finalHeight;
float halfHeight;
int yellow = 0;

Stepper stepper(stepsPerRevolution, 8, 10, 9, 11);
UltraSonicDistanceSensor ultrasonicSensor(TRIG, ECHO);


void setup() 
{ Serial.begin(9600);

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(btnReset, INPUT_PULLUP);
  pinMode(btnTest, INPUT_PULLUP);
  pinMode(btnDetect, INPUT_PULLUP);
  pinMode(forceSensor, INPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  
  stepper.setSpeed(motorSpeed);
}

void loop() 
{

  stepper.step(-1);
  if (digitalRead(btnReset) == LOW)
  {
    digitalWrite(GREEN, LOW);
    digitalWrite(RED, HIGH);
    state = 0;
  }


  if (digitalRead(btnTest) == LOW)
  {
    state = 1;
  }

  if (digitalRead(btnDetect) == LOW)
  {
    state = 2;
  }

  if (state == 1)
  {
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, HIGH);
    motor();
  }

  if (state == 2)
  {
    if (yellow == 0)
    {
    digitalWrite(RED, LOW);
    }
    else
    {
      digitalWrite(RED, HIGH);
    }
    digitalWrite(GREEN, HIGH);
    volumeDetection();
  }

  if (state == 3)
  {
    digitalWrite(GREEN, LOW);
    blinkLED();
  }
  
}



void motor()
{
  Serial.print("Force: ");
  Serial.println(analogRead(forceSensor));
  stepper.step(1);
}

void volumeDetection()
{
  finalHeight = 5;
  
  height = ultrasonicSensor.measureDistanceCm();
  Serial.print("Distance: ");
  Serial.println(height);

  if (initialHeight == -1 && height != -1)
  {
    initialHeight = 23;
    Serial.print("Initial Height: ");
    Serial.println(initialHeight);
    halfHeight = (initialHeight - finalHeight) *.2 + finalHeight;
    Serial.print("Half Height: ");
    Serial.println(halfHeight);
  }

  if (height <= halfHeight && height != -1)
  {
    Serial.println("asdf");
    yellow = 1;
  }

  if (height <= finalHeight && height != -1)
  {
    state = 3;
  }
}



void real_motor()
{
  Serial.print("Force");
  Serial.println(analogRead(forceSensor));
  if (analogRead(forceSensor) > threshold)
  {
    stepper.step(1);
  }
  else
  {
    state = 0;
  }
}


float InitialHeight()
{
  float sum;
  for (int i = 0; i < 10; i ++)
  {
    height = ultrasonicSensor.measureDistanceCm();  
    if (height != -1)
    {
      sum += height;
    }
    else 
    {
      i--;
    }
  }
  
  return sum/10.0;
}

void blinkLED()
{
  digitalWrite(RED, HIGH);
  delay(500);
  digitalWrite(RED, LOW);
  delay(500);
}
