// Saara - tester code for line tracking alterations
//www.elegoo.com

//Line Tracking IO define
#define LT_R !digitalRead(10)
#define LT_M !digitalRead(4)
#define LT_L !digitalRead(2)

#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11

//#define carSpeed 175
int carSpeed = 0;
bool slowSpeed = true;
//250 , 125

void determineSpeed (){
  if(slowSpeed){
    carSpeed = 100;
  }
  else {
    carSpeed = 175;  
  }
}

void forward(int speedC){
  analogWrite(ENA, speedC);
  analogWrite(ENB, speedC);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("go forward!");
}

void back(){
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("go back!");
}

void left(int speedC){
  analogWrite(ENA, speedC);
  analogWrite(ENB, speedC);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("go left!");
}

void right(int speedC){
  analogWrite(ENA, speedC);
  analogWrite(ENB, speedC);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW); 
  Serial.println("go right!");
} 

void stop(){
   digitalWrite(ENA, LOW);
   digitalWrite(ENB, LOW);
   Serial.println("Stop!");
} 

void setup(){
  Serial.begin(9600);
  pinMode(10,INPUT);
  pinMode(4,INPUT);
  pinMode(2,INPUT);
}

void loop() {
  determineSpeed();
  
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