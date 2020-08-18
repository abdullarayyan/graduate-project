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

void setup() {
  Serial.begin (9600);
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
  pinMode (ENA, OUTPUT);
  pinMode (ENB, OUTPUT);
  pinMode(IRL, INPUT); // initialize Right sensor as an inut
  pinMode(IRR, INPUT); // initialize Left sensor as as input

  delay(300);

  analogWrite(ENA, 200);
  analogWrite(ENB, 200);

  delay(2000);
}

void loop() {
  checkIRSensor();
  FirstProgram();
}

void checkIRSensor() {
  LEFT_SENSOR = analogRead(IRL);
  RIGHT_SENSOR = analogRead(IRR);
}

void FirstProgram() {
  if (moveNum == 0 || moveNum == 2 || moveNum == 4 || moveNum == 6) {
    normalMove();
  }
  else if(moveNum == 1){
    turnRight();
  }
  else if(moveNum == 3){
    turnReverseRight();
  }
  else if(moveNum == 5){
    turnLeft();
  }
  else if(moveNum == 7){
    turnReverseRight();
    moveNum = 0;
  }
}

void turnRight(){
    do{
      moveRight();
      delay(500);
      mstop();
      delay(500);
      checkIRSensor();
    } while (RIGHT_SENSOR < thresholdIR || LEFT_SENSOR < thresholdIR);
    moveForward();
    delay(2000);
    moveNum++;
}

void turnLeft(){
    do{
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

void turnReverseRight(){
    do{
      moveRight();
      delay(500);
      mstop();
      delay(500);
      checkIRSensor();
    } while (RIGHT_SENSOR < thresholdIR);
    
    do{
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

  else if (RIGHT_SENSOR > thresholdIR && LEFT_SENSOR < thresholdIR) //LEFT
  {
    moveLeft();
  }

  else if (RIGHT_SENSOR < thresholdIR && LEFT_SENSOR > thresholdIR) //RIGHT
  {
    moveRight();
  }

  else if (RIGHT_SENSOR > thresholdIR && LEFT_SENSOR > thresholdIR) //BACK
  {
    moveNum++;
    mstop();
    delay(1000);
    checkIRSensor();
    while (RIGHT_SENSOR < thresholdIR || LEFT_SENSOR < thresholdIR) {
      moveBack();
      delay(500);
      mstop();
      delay(500);
      checkIRSensor();
    }
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

void moveLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveRight() {
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
