#include <NewPing.h>
#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

const int IN1 = 7;
const int IN2 = 6;
const int IN3 = 5;
const int IN4 = 4;
const int ENA = 9;
const int ENB = 10;
const int IRL = A0;
const int IRR = A1;
int LEFT_SENSOR = 1000;
int RIGHT_SENSOR = 1000;
int thresholdIR = 50;

int program = 0;
int moveNum = 0;
int flagR = 0;
int flagL = 0;
int flagRFirst = 0;
int distance = 0;
char data = 0; 

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup() {
  Serial.begin(9600);
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
  pinMode (ENA, OUTPUT);
  pinMode (ENB, OUTPUT);
  pinMode(IRL, INPUT); // initialize Right sensor as an inut
  pinMode(IRR, INPUT); // initialize Left sensor as as input

  delay(300);

  analogWrite(ENA, 150);
  analogWrite(ENB, 150);

  delay(5000);
}

void loop() {
  checkIRSensor();
  distance = sonar.ping_cm();
  if (distance <= 50) {
    mstop();
  }
  else {
    if (Serial.available() > 0)
    {
      data = Serial.read();
      if (data == '1')
        program = 1;
      else if (data == '2')
        program = 2;
      else if (data == '3')
        program = 3;
      else if (data == '4')
        program = 4;
    }

    decideWhichTable();
  }
}

void decideWhichTable(){
  if(program == 1){
    FirstTable();
  }
  else if(program == 2){
    SecondTable();
  }
}

void checkIRSensor() {
  LEFT_SENSOR = analogRead(IRL);
  RIGHT_SENSOR = analogRead(IRR);
}

void FirstTable() {
  if (moveNum == 0 || moveNum == 2 || moveNum == 4 || moveNum == 6) {
    normalMove();
  }
  else if (moveNum == 1) {
    turnRight();
  }
  else if (moveNum == 3) {
    turnReverseRight();
  }
  else if (moveNum == 5) {
    turnLeft();
  }
  else if (moveNum == 7) {
    turnReverseRight();
    moveNum = 0;
  }
}


void SecondTable() {
  if (moveNum == 3 || moveNum == 5 || moveNum == 7) {
    normalMove();
  }
  else if (moveNum == 2) {
    turnRight();
  }
  else if (moveNum == 4) {
    turnReverseRight();
  }
  else if (moveNum == 6) {
    turnLeft();
  }
  else if (moveNum == 8) {
    turnReverseRight();
    moveNum = 0;
  }
}

void turnRight() {
  do {
    moveRight();
    checkIRSensor();
  } while (RIGHT_SENSOR > thresholdIR || LEFT_SENSOR > thresholdIR);
  mstop();
  delay(1000);
  do {
    moveRight();
    checkIRSensor();
  } while (RIGHT_SENSOR < thresholdIR && LEFT_SENSOR < thresholdIR);

  if (RIGHT_SENSOR > thresholdIR) {
    flagRFirst = 1;
  }

  while (flagR == 0 ||  flagL == 0) {
    if (RIGHT_SENSOR < thresholdIR && flagRFirst == 1) {
      flagR = 2;
    }
    else if (RIGHT_SENSOR > thresholdIR && flagRFirst != 1) {
      flagR = 1;
    }
    if (LEFT_SENSOR > thresholdIR) {
      flagL = 1;
    }
    checkIRSensor();
    moveOneSide();
  }

  moveNum++;
}

void turnLeft() {
  do {
    moveLeft();
    delay(500);
    mstop();
    delay(500);
    checkIRSensor();
  } while (RIGHT_SENSOR < thresholdIR || LEFT_SENSOR < thresholdIR);
  moveForward();
  delay(2000);
  moveNum++;
}

void turnReverseRight() {
  do {
    moveRight();
    delay(500);
    mstop();
    delay(500);
    checkIRSensor();
  } while (RIGHT_SENSOR < thresholdIR);

  do {
    moveRight();
    delay(500);
    mstop();
    delay(500);
    checkIRSensor();
  } while (RIGHT_SENSOR > thresholdIR);

  moveNum++;
}


void normalMove() {
  if (RIGHT_SENSOR < thresholdIR && LEFT_SENSOR < thresholdIR) //FORWARD
  {
    moveForward();
  }

  else if (RIGHT_SENSOR > thresholdIR && LEFT_SENSOR < thresholdIR) //RIGHT
  {
    moveRight();
  }

  else if (RIGHT_SENSOR < thresholdIR && LEFT_SENSOR > thresholdIR) //LEFT
  {
    moveLeft();
  }

  else if (RIGHT_SENSOR > thresholdIR && LEFT_SENSOR > thresholdIR) //BACK
  {
    moveNum++;
    mstop();
    delay(1000);
    checkIRSensor();
    while (RIGHT_SENSOR < thresholdIR || LEFT_SENSOR < thresholdIR) {
      moveBack();
      checkIRSensor();
    }
    mstop();
  }
}

void moveOneSide() {
  if (RIGHT_SENSOR > thresholdIR && (flagR == 2 || flagRFirst == 0)) //RIGHT
  {
    moveRight();
  }
  else if (flagRFirst == 1 || flagR == 0) //FORWARD
  {
    moveForward();
  }
}

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void moveBack() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void mstop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
