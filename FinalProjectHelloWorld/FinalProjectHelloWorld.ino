
//include libraries
#include <SPI.h>
#include <MFRC522.h>
#include <AccelStepper.h>

// RFID libraries accessories
#define RST_PIN 9
#define SS_PIN 10

// Stepper accessories
#define directionPin 3
#define stepperPin 4
#define motorInterfaceType 1

//RFID goodies
MFRC522 mfrc522(SS_PIN, RST_PIN);

// name the stepper
AccelStepper theStepper = AccelStepper(motorInterfaceType, stepperPin, directionPin);

// card variables
boolean openSesame = true;


void setup() {

  // monitor
  Serial.begin(9600);

  // RFID sensor reader
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println(F("Scan Card"));

  // stepper stuff
  theStepper.setMaxSpeed(1000);
  theStepper.setSpeed(1000);
  theStepper.setCurrentPosition(0);

  //  theStepper.moveTo(4096);
  //  theStepper.runToPosition();

}


void loop() {


  // SCAN THE CARD AND IF DOOR IS CLOSED THEN UNLOCK THE DOOR
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  if (openSesame == true) {
    // if the card is scanned make one revolution of the stepper
    while (theStepper.currentPosition() != 125) {

      theStepper.setSpeed(500);
      theStepper.runSpeed();

    }
    theStepper.setCurrentPosition(125);
    openSesame = false;
  }



  else {

    // set the current position to equal the open configuration
    theStepper.setCurrentPosition(125);

    while (theStepper.currentPosition() != 0) {
      theStepper.setSpeed(-500);
      theStepper.runSpeed();

    }

    theStepper.setCurrentPosition(0);
    openSesame = true;
  }
}
