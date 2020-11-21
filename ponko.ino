#include <DFRobotDFPlayerMini.h>
#include <Servo.h>
#include "SoftwareSerial.h"

int buttonToggleStartState = 0;
int buttonNextState = 0;
int buttonToggleDancingState = 0;

int isPlaying = 0;
int isPartyMode = 0;
int isDancing = 0;
int initBeeped = 0;
Servo servoRight;
Servo servoNeck;

SoftwareSerial mySoftwareSerial(12, 13); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

int piezoPin = 8;

void setup()
{
  pinMode(2, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  servoRight.attach(10);
  servoNeck.attach(11);
  Serial.begin(9600);

  servoNeck.write(90);
  servoRight.write(90);
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true);
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(25);  //Set volume value. From 0 to 30
}

void loop() {
  if (initBeeped == 0) {
    beep(500);
    initBeeped = 1;
  }

  readButtons();
  if (buttonToggleStartState == 1) {
    Serial.println("Start music!");
    resetButtons();
    isPlaying = 1;
    playMusic();
    partyMode();
    delay(500);
  }
}

void readButtons() {
  buttonToggleStartState = digitalRead(2);
  buttonNextState = digitalRead(4);
  buttonToggleDancingState = digitalRead(5);
  if (buttonToggleStartState == 1) {
    Serial.println("Toggle start pressed");
  }
  if (buttonNextState == 1) {
    Serial.println("Next pressed");
  }
  if (buttonToggleDancingState == 1) {
    Serial.println("Dancing pressed");
  }
}

void resetButtons() {
  buttonToggleStartState = 0;
  buttonNextState = 0;
  buttonToggleDancingState = 0;
  delay(500);
}

void partyMode() {
  while (isPlaying) {
    readButtons();
    if (buttonToggleStartState == 1) {
      resetButtons();
      if (isDancing == 1) {
        Serial.println("Stop music and dancing");
      } else {
        Serial.println("Stop music");
      }
      isPlaying = 0;
      isDancing = 0;
      servoNeck.write(90);
      servoRight.write(90);
      myDFPlayer.stop();
      resetButtons();
    }
    else if (buttonNextState == 1) {
      Serial.println("Next number");
      nextNumber();
      resetButtons();
    }
    else if (buttonToggleDancingState == 1) {
      if (isDancing == 0) {
        Serial.println("Start dancing");
        isDancing = 1;
      }
      else {
        Serial.println("Stop dancing");
        isDancing = 0;
      }
      resetButtons();
    }

    if (isPlaying == 1 && isDancing == 1) {
      moveNeck();
      moveArms();
    }
  }
}

void playMusic() {
  myDFPlayer.play(1);  //Play the first mp3
}

void nextNumber() {
  myDFPlayer.next();
}

void moveNeck() {
  int position = random(45,135);
  servoNeck.write(position);
  delay(250);
}

void moveArms() {
  int position = random(70, 110);
  servoRight.write(position);
  delay(600);
}

void beep(int delayms) {
  tone(piezoPin, 1000, 500);
  delay(100);
}
