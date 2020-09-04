#include <Stepper.h>
#include <HCSR04.h>

const int stepsPerRevolution = 800;  // change this to fit the number of steps per revolution
const int radius = 1; //radius of dowel

int btnTest = 2;
int redLedPin = 3;
int greenLedPin = 4;
int trigPin = 5;
int echoPin = 6;
int btnDetect = 7;

int fsrAnalogPin = A0;

float revCount = 0;
float distWinded;
int timeCounter = 0;
int start = 0;
int testOrDetect = -1; //-1 - Off; 0 - Test; 1 - Calculations; 2 - Volume Detect
float initialHeight;
float finalVolume;
int testReady = HIGH;
int detectReady = 0;
int coeff;
int finalHeight;


void doughTest();
void volumeDetect();
void predictVolume();

Stepper stepper(stepsPerRevolution, 8, 10, 9, 11);
UltraSonicDistanceSensor ultrasonicSensor(trigPin, echoPin);



void setup()
{
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(fsrAnalogPin, INPUT);
  pinMode(btnTest, INPUT_PULLUP);
  pinMode(btnDetect, INPUT_PULLUP);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);

  stepper.setSpeed(20); //Set speed to 20 RPM
  testReady = digitalRead(btnTest);

  while (start==0) 
  {
   testReady = digitalRead(btnTest);


    if (testReady == LOW) 
    {
      testOrDetect = 0;
      analogWrite(redLedPin, 255);
      start=1;
    }
  }
}

void loop()
{
  if (testOrDetect == 0 ) //Test 
  {
    doughTest();
    Serial.println("loop if 0");
    testOrDetect = 1;
  }
  else if (testOrDetect == 1)
  {
    initialHeight = detectHeight();
    distWinded = calcDist(revCount);
    coeff = predictVolume(distWinded);
    finalHeight = initialHeight*coeff;
    testOrDetect = 2;
  }
  else if (testOrDetect == 2) 
  {
    while (detectReady == 0)
    {
      if (digitalRead(btnDetect) == LOW)
      {
        detectReady = 1;
      }
    }

    volumeDetect();

  }

}

float detectHeight()
{ 
  int i;
  float heightSum = 0;
  float height;

  for(i=0; i<10; i++)
  {
    height = ultrasonicSensor.measureDistanceCm();
    if (height != -1)
    {
      heightSum+=height;
    }
    else {
      i-=1;
    }
  }

  return heightSum/10.0;
}

void doughTest()
{
      Serial.println("doughTest");

  float fsrReading;
  float threshold = 100; //Change Based on Experiment
  int underCounter;

  fsrReading = analogRead(fsrAnalogPin);
  Serial.println("fsrReading");

  if (fsrReading > threshold)
  {
    Serial.println("doughTest if");

    stepper.step(10);
    revCount += 0.0125;
    analogWrite(redLedPin, 0);
    analogWrite(greenLedPin, 255);
  }
  else
  {
          Serial.println("doughTest else");

    underCounter++;
    if (underCounter > 10) //If it is the tenth time that it is less than the threshold
    {
      testOrDetect = 1;
      analogWrite(redLedPin, 255);
      analogWrite(greenLedPin, 0);
    }
  }
}

void volumeDetect()
{
  
}

float calcDist(float revCount)
{
  return 2*3.141592*radius*revCount;
}

float predictVolume(float revCount)
{
  //ADD MATH FUNCTION HERE
  return revCount;
}
