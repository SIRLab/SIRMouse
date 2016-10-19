/*
   SirTitan
   Author: https://github.com/rafaelalmeidatk
*/

//--------------------------------------------------
// RC Receiver

const int rcCh1Pin = 10;
const int rcCh2Pin = 9;

int ch1Value = 0;
int ch2Value = 0;

//--------------------------------------------------
// Motors

int motorsE[2] = { 5, 6 };
int motorsM[2] = { 4, 7 };

//--------------------------------------------------
// Moves

const int M_LEFT = 0;
const int M_RIGHT = 1;

const int FRONT = 0;
const int BACK = 1;
const int LEFT = 2;
const int RIGHT = 3;

const int FAST = 0;
const int MEDIUM = 1;
const int SLOW = 2;

int speeds[3] = { 255, 100, 50 };

//--------------------------------------------------
// Current Speed

int currentSpeed = 0;

//--------------------------------------------------
// Movement changed

bool changed = false;

//--------------------------------------------------

void motorDirection(int moveId) {
  int e[2];
  switch (moveId) {
    case FRONT:
      e[0] = HIGH;
      e[1] = HIGH;
      break;
    case BACK:
      e[0] = LOW;
      e[1] = LOW;
      break;
    case LEFT:
      e[0] = LOW;
      e[1] = HIGH;
      break;
    case RIGHT:
      e[0] = HIGH;
      e[1] = LOW;
      break;
  }
  for (int i = 0; i < 2; i++) {
    digitalWrite(motorsM[i], e[i]);
  }
}

void moveRobot(int moveId) {
  Serial.print("move robot: ");
  Serial.println(moveId);
  motorDirection(moveId);
  for (int i = 0; i < 2; i++) {
    analogWrite(motorsE[i], 255);
  }
}

void readRCReceiver() {
  ch1Value = pulseIn(rcCh1Pin, HIGH);
  ch2Value = pulseIn(rcCh2Pin, HIGH);
}

void changedState() {
  changed = true;
  delay(50);
}

bool gotMove(int idMove) {
  if (idMove == FRONT && ch1Value > 1700) {
    return true;
  } else if (idMove == BACK && ch1Value < 1100) {
    return true;
  }
  if (idMove == RIGHT && ch2Value > 1700) {
    return true;
  } else if (idMove == LEFT && ch2Value  < 1400) {
    return true;
  }

  return false;
}

void setup()
{
  Serial.begin(9600);
  for (int i = 0; i < 2; i++) {
    pinMode(motorsM[i], OUTPUT);
  }
  digitalWrite(motorsM[0], LOW);
  digitalWrite(motorsM[1], HIGH);
  
  pinMode(rcCh1Pin, INPUT);
  pinMode(rcCh2Pin, INPUT);

  for (int i = 0; i < 2; i++) {
    analogWrite(motorsE[i], 255);
  }
}

void loop()
{
  readRCReceiver();

  Serial.print("Channel #1: ");
  Serial.print(ch1Value);
  Serial.print('\t');
  Serial.print("Channel #2: ");
  Serial.println(ch2Value);
  
  if (gotMove(FRONT)) {
    moveRobot(FRONT);
    changedState();
  } else if (gotMove(BACK)) {
    moveRobot(BACK);
    changedState();
  }

  if (gotMove(RIGHT)) {
    moveRobot(RIGHT);
    changedState();
  } else if (gotMove(LEFT)) {
    moveRobot(LEFT);
    changedState();
  }
  
  if (changed) {
    for (int i = 0; i < 2; i++) {
      analogWrite(motorsE[i], 0);
    }
    changed = false;
  }
}
