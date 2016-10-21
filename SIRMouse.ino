/*
   SIRMouse
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

const int SIDE = 0;
const int MOVEMENT = 1;

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

unsigned int sideTick = 0;
unsigned int movementTick = 0;

//--------------------------------------------------

void motorDirection(int moveId) {
  int state, type;
  switch (moveId) {
    case FRONT:
      state = LOW;
      type = MOVEMENT;
      break;
    case BACK:
      state = HIGH;
      type = MOVEMENT;
      break;
    case LEFT:
      state = HIGH;
      type = SIDE;
      break;
    case RIGHT:
      state = LOW;
      type = SIDE;
      break;
  }
  digitalWrite(motorsM[type], state);
}

void moveRobot(int moveId) {
  motorDirection(moveId);
  if (moveId == FRONT || moveId == BACK) {
    analogWrite(motorsE[MOVEMENT], 255);
    movementTick = millis() + 50;
  } else {
    analogWrite(motorsE[SIDE], 255);
    sideTick = millis() + 250;
  }
}

void readRCReceiver() {
  ch1Value = pulseIn(rcCh1Pin, HIGH);
  ch2Value = pulseIn(rcCh2Pin, HIGH);
}

bool gotMove(int idMove) {
  if (ch1Value != 0) {
    if (idMove == FRONT && ch1Value > 1700) {
      return true;
    } else if (idMove == BACK && ch1Value < 1100) {
      return true;
    }
  }

  if (ch2Value != 0) {
    if (idMove == RIGHT && ch2Value > 1700) {
      return true;
    } else if (idMove == LEFT && ch2Value  < 1100) {
      return true;
    }
  }

  return false;
}

void setup()
{
  Serial.begin(9600);
  for (int i = 0; i < 2; i++) {
    pinMode(motorsM[i], OUTPUT);
  }
  digitalWrite(motorsM[0], HIGH);
  digitalWrite(motorsM[1], LOW);

  pinMode(rcCh1Pin, INPUT);
  pinMode(rcCh2Pin, INPUT);

  for (int i = 0; i < 2; i++) {
    analogWrite(motorsE[i], 0);
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
  } else if (gotMove(LEFT)) {
    moveRobot(LEFT);
  }

  if (movementTick != 0 && millis() > movementTick) {
    analogWrite(motorsE[MOVEMENT], 0);
    movementTick = 0;
  }

  if (sideTick != 0 && millis() > sideTick) {
    analogWrite(motorsE[SIDE], 0);
    sideTick = 0;
  }
}
