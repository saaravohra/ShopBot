///// SHOPBOT CODE /////

// Code Functions: Line Tracking, Color Sensing, Robotic Arm

// Color Sensor //
#include <Wire.h>
#include "Adafruit_TCS34725.h" // Color Sensor
#include <millisDelay.h> // Timer library
#include <Servo.h> //Servo Motors

// Color Sensor //
/* Color Sensor Wiring 
 *  Green -> Red -- 5V (VIN)
 *  Blue -> Brown -- GND 
 *  Black -> Black -- SCL (A5)
 *  White -> White -- SDA (A4)
*/

/* Initialize with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_1X);

millisDelay blueDelay;
bool greenDelayRunning = false;
bool redDelayRunning = false;

bool blueTapeReached = false;
bool greenTapeReached = false;
bool redTapeReached = false;
bool detectLine = true;
int travelTime = 0;

unsigned long greenSection = 0;
unsigned long redSection = 0;

char tapeColor = 'x'; // Default tape color = x (black tape)

// Line Tracking Sensor //
//IO define
#define LT_R !digitalRead(10)
#define LT_M !digitalRead(4)
#define LT_L !digitalRead(2)

#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11

// Robotic Arm // 
Servo arm;
Servo wrist;
Servo finger; 

int pos = 0;

// Color Detection // 
uint16_t r, g, b, c, colorTemp, lux;

// Car Speed // 
int carSpeed = 0; 
bool slowSpeed = false; // slow speed once the colored section is detected

void determineSpeed(){
  if(slowSpeed){
    carSpeed = 95;
  }
  else {
    carSpeed = 120;
  }
}

/* Color Sensor Functions */

// Checks what color is detected by the Color Sensor. Returns the tapeColor detected. Based on color, robot will slow down or speed up.
void colorDetected(int r, int g, int b, int c) {
  if(r <= 142 && r >= 80 && g <= 83 && g >= 24 && b <= 81 && b >= 23 && c <= 228 && c >= 145) { // if Red
    tapeColor = 'r';
    slowSpeed = true;
  }
  else if(r <= 66 && r >= 2 && g <= 83 && g >= 23 && b <= 110 && b >= 49 && c <= 183 && c >= 110) { // if Blue
    tapeColor = 'b';
    slowSpeed = true;
  }
  else if(r <= 79 && r >= 14 && g <= 138 && g >= 73 && b <= 88 && b >= 25 && c <= 242 && c >= 157) { //if Green
    tapeColor = 'g';
    slowSpeed = true;
  }
  else if(r <= 212 && r >= 148 && g <= 136 && g >= 79 && b <= 92 && b >= 39 && c <= 379 && c >= 295) { //if Orange
    tapeColor = 'o';
    slowSpeed = false;
  }
  else {
    tapeColor = 'x';
  }
}

/* End of Color Sensor Functions */

/* Beginning of Grocery Shop Logic*/

// time in milliSeconds
void pauseRun(int delayTime){
  forward(0);
  delay(delayTime);
}

// Takes in the tapeColor detected.  
void checkpointLogic (char color) {
  switch(color) {
    case 'b':
      pauseRun(2000);
      alignToItem(0,1,0,0,0,0,0,0,0); //bSqr
      pauseRun(10000); //robotic arm movement
      groceryPickup();
      resetLineDetect();
      pauseRun(500);
      break;
    case 'g':
      pauseRun(1000);
      //alignToItem(0,0,0,0,1,0,0,0,0); //gSqr
      pauseRun(10000);
      groceryPickup();
      resetLineDetect();
      pauseRun(500);
      break;
    case 'r':
      pauseRun(1000);
      alignToItem(0,0,0,0,0,0,1,0,0); //rCir
      pauseRun(10000);
      groceryPickup();
      resetLineDetect();
      pauseRun(500);
      break;
    case 'x':
      Serial.println("no color detected");
      break;
    default: 
      Serial.println("Error: Color Values detected are unspecified.");
      break;
  }
}

// function alignToItem - how much time to delay for the robot to move to the desired item
void alignToItem(int bCir, int bSqr, int bCyl, int gCir, int gSqr, int gCyl, int rCir, int rSqr, int rCyl) {
  if((bCir || gCir || rCir) == 1){ 
    travelTime = 1500;
  }
  else if((bSqr || rSqr || gSqr) == 1) {
    travelTime = 2600;
  }
  else if((bCyl || rCyl || gCyl) == 1){
    travelTime = 3000;
  }
  else {
    travelTime = 0;
  }
  forward(95);
  delay(travelTime);
}

/* End of Grocery Shop Logic */


/* Line Tracking Functions */

void resetLineDetect(){
  detectLine = true;
}

void forward(int cSpeed){
  analogWrite(ENA, cSpeed);
  analogWrite(ENB, cSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void back(int cSpeed){
  analogWrite(ENA, cSpeed);
  analogWrite(ENB, cSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void left(int cSpeed){
  analogWrite(ENA, cSpeed);
  analogWrite(ENB, cSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void right(int cSpeed){
  analogWrite(ENA, cSpeed);
  analogWrite(ENB, cSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW); 
} 

void stop(){
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
} 

void lineFollowing() {
   if(LT_M){
      forward(carSpeed);
    }
    else if(LT_R) { 
      right(carSpeed);
      while(LT_R);                             
    }   
    else if(LT_L) {
      left(carSpeed);
      while(LT_L);  
    }   
}

/* End of Line Tracking Functions */

/* Beginning of Robotic Arm Functions */

// Arm, Wrist and Finger Motion Functions for Grocery Pickup // 

void armDefault() {
  arm.write(40); // -- INITIAL POSITION
  delay(500);
  wrist.write(50); // -- INITIAL POSITION
  delay(500);
  finger.write(140); // -- INITIAL POSITON / DROP OFF / FINGERS OPEN
  delay(500);
}

void lowerWrist() { 
  // lower wrist
  for (pos = 50; pos <= 100; pos += 1) {
    wrist.write(pos); 
    delay(15);
  }
}

void closeFingers() {
  //close fingers
  for (pos = 140; pos <= 180; pos += 1) {
    finger.write(pos); 
    delay(15);
  }
}

void raiseWrist() {
  // raise wrist
  for (pos = 100; pos >= 50; pos -= 1) { 
    wrist.write(pos);              
    delay(15);                     
  }
}

void moveArmToCart() {
  // move arm to grocery cart
  for (pos = 40; pos <= 120; pos += 1) {
    arm.write(pos); 
    delay(15);
  }
}

void openFingers() {
  // close fingers
  for (pos = 180; pos >= 140; pos -= 1) { 
    finger.write(pos);              
    delay(15);                     
  }
}

void resetArm() {
  // reset arm base to default position
  for (pos = 120; pos >= 40; pos -= 1) { 
    arm.write(pos);              
    delay(15);          
  }
}

void groceryPickup() {
  // Item Pick Up, Drop to Basket, Return Arm to Default Position
    armDefault();
    lowerWrist();
    closeFingers();
    raiseWrist();
    moveArmToCart();
    lowerWrist();
    openFingers();
    raiseWrist();
    resetArm();
}

/* End of Robotic Arm Functions */

/* Beginning of Timer Functions */

void checkBlueTimer() {
  // check if delay has timed out
  if (blueDelay.justFinished()) { // only returns TRUE once.
    blueTapeReached = true;
    detectLine = false;
  }
}

void checkGreenTimer() {
  if (greenDelayRunning && ((millis() - greenSection) >= 35000)){ //36000
    greenDelayRunning = false; //prevent from being run more than once
    greenTapeReached = true;
    detectLine = false;
  }
}

void checkRedTimer() {
  if (redDelayRunning && ((millis() - redSection) >= 20000)){ //61000, 42000
    redDelayRunning = false; //prevent from being run more than once
    redTapeReached = true;
    detectLine = false;
  }
}

/* End of Timing Functions */

void shopBotGroceryRun() { 
 determineSpeed();

  //tcs.getRawData(&r, &g, &b, &c); // get R G B C values  
  //colorDetected(r, g, b, c);  // check what color is detected by the sensor

  checkBlueTimer();
  checkGreenTimer();
  checkRedTimer();
  
  if(detectLine){
    lineFollowing();
  }
  else { // if line detection is turned off
    if (blueTapeReached) {
      checkpointLogic('b');
      blueTapeReached = false;
    }
    else if (greenTapeReached) {
      checkpointLogic('g');
      greenTapeReached = false;
    }
    else if (redTapeReached) {
      checkpointLogic('r');
      redTapeReached = false;
    }
  }
}


void setup(){
  Serial.begin(9600);

  // Line Sensor Pins //
  pinMode(10,INPUT);
  pinMode(4,INPUT);
  pinMode(2,INPUT);

  // Robotic Arm Servo Motors //
  arm.attach(13);
  wrist.attach(12);
  finger.attach(3);

  // Find Color Sensor //
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }

  // Timers // 
  blueDelay.start(5400); // start a 5.3 second delay for robot to get to blue section
  
  greenSection = millis(); // start delay
  greenDelayRunning = true; // not finished yet 

  redSection = millis();
  redDelayRunning = true;

  
}


void loop() {

  shopBotGroceryRun();
 
}