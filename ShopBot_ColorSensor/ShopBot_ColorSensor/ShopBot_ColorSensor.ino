#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <Servo.h> //to test color detection when not connected to the computer and can see the serial monitor

// green to red is 5v
// blue to brown is GND
// black to SCL A5
// white to SDA A4

/* Initialise with default values (int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_1X); //TCS34725_INTEGRATIONTIME_700MS

char tapeColor = 'x';

//uint16_t r, g, b, c, colorTemp, lux;

Servo myservo;  // create servo object to control a servo
int pos = 0;
int angle = 0;

void setup(void) {
  Serial.begin(9600);

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }

  myservo.attach(13);  // attaches the servo on pin 3 to the servo object //13
}

void checkpointLogic (char color) {
  switch(color) {
    case 'r':
      Serial.println("red detected");
      angle = 45;
      break;
    case 'b':
      Serial.println("blue detected");
      angle = 90;
      break;
    case 'g':
      Serial.println("green detected");
      angle = 135;
      break;
    case 'o':
      Serial.println("orange detected");
      angle = 180;
      break;
    case 'x':
      Serial.println("no color detected");
      angle = 0;
      break;
    default: 
      Serial.println("Error: Color Values detected are unspecified.");
      break;
  }

  //move servo motor
    for (pos = 0; pos <= angle; pos += 1) { // goes from 0 degrees to 90 degrees
      // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
  
  // reset the tape color
  tapeColor = 'x';
}

void colorDetected(int r, int g, int b, int c) {

  if(r <= 142 && r >= 80 && g <= 83 && g >= 24 && b <= 81 && b >= 23 && c <= 228 && c >= 145) { // if Red
    tapeColor = 'r'; 
  //Serial.println("red detected");
  }
  else if(r <= 66 && r >= 2 && g <= 83 && g >= 23 && b <= 110 && b >= 49 && c <= 183 && c >= 110) { // if Blue
    tapeColor = 'b';
    //Serial.println("blue detected");
  }
  else if(r <= 79 && r >= 14 && g <= 138 && g >= 73 && b <= 88 && b >= 25 && c <= 242 && c >= 157) { //if Green
    tapeColor = 'g';
    Serial.println("green detected");
  }
   else if(r <= 84 && r >= 22 && g <= 89 && g >= 25 && b <= 119 && b >= 55 && c <= 214 && c >= 120) { //if Purple
    tapeColor = 'p';
    Serial.println("purple detected");
  }
  else if(r <= 212 && r >= 148 && g <= 136 && g >= 79 && b <= 92 && b >= 39 && c <= 379 && c >= 295) { //if Orange
    tapeColor = 'o';
    Serial.println("orange detected");
  }
  else {
    tapeColor = 'x';
    Serial.println("OTHER COLOR detected");
  }
}

void loop(void) {

  uint16_t r, g, b, c, colorTemp, lux;
  tcs.getRawData(&r, &g, &b, &c);

  Serial.print("R: "); Serial.print(r); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");
  
  colorDetected(r, g, b, c);
  checkpointLogic(tapeColor);
}
