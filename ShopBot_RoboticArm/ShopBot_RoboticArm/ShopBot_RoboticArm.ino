// Robotic Arm Servo Motor Functions

#include <Servo.h>

Servo arm;
Servo wrist;
Servo finger; 

int pos = 0;

void setup() {
  arm.attach(13);
  wrist.attach(12);
  finger.attach(3);
}

// Arm, Wrist and Finger Motion Functions for Grocery Pickup // 

void armDefault() {
  arm.write(20); // -- INITIAL POSITION
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
  for (pos = 20; pos <= 120; pos += 1) {
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
  for (pos = 120; pos >= 20; pos -= 1) { 
    arm.write(pos);              
    delay(15);          
  }
}

void groceryPickup() {
    armDefault(); // -- SETS UP INITIAL POSITIONS LISTED ABOVE
    lowerWrist();
    closeFingers();
    raiseWrist();
    moveArmToCart();
    lowerWrist();
    openFingers();
    raiseWrist();
    resetArm();
}

void testItemPickUp() {

  //openFingers();
  //moveArmToItem();
  //lowerWrist();
  //closeFingers();
  //delay(500);
  //raiseWrist();
  //moveArmToDefault();

}

void groceryPickup_old() {
  // Item Pick Up, Drop to Basket, Return Arm to Default Position
  //openFingers();
  //moveArmToItem();
  //lowerWrist();
  //closeFingers();
  //raiseWrist();
  //moveArmToBasket();
  //lowerWrist();
  //openFingers();
  //raiseWrist();
  //closeFingers();
  //moveArmToDefault();
}

void loop() {
  // Item Pick Up, Drop to Basket, Return Arm to Default Position
  groceryPickup();

  // Testing to see if arm can move to an item, pick it up, and move back to default position
  //testItemPickUp();

}