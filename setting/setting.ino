#include <SoftwareSerial.h>

// HM-10 pins
#define RX 4
#define TX 5

// HM-10 module instance
SoftwareSerial bleSerial(RX, TX);

void setup() {
  Serial.begin(9600);
  bleSerial.begin(9600);
}

// new line characters
#define CR '\r'
#define LF '\n'

// flag to print new line character
boolean newLine = true;

void loop() {
  // read from HM-10 module and send to the serial monitor
  if (bleSerial.available() > 0) {
    char c = bleSerial.read();
    Serial.write(c);
  }

  // read from the serial monitor and send to the HM-10 module
  if (Serial.available() > 0) {
    char c = Serial.read();

    // HM-10 does not accept CR and LF character
    if (c != CR && c != LF) {
      bleSerial.write(c);
    }

    // echo user input to the main window
    if (newLine) {
      Serial.print("\r\n>");
      newLine = false;
    }
    Serial.write(c);
    if (c == LF) {
      newLine = true;
    }
  }
}
