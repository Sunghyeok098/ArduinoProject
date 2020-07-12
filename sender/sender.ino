// arduino-rc-car-ble-controller.ino
// BLE RC controller using HM-10 module
//
// Making a rc-car using Arduino
// http://myoungjinkim.github.io
//

#include <SoftwareSerial.h>

// joystick pins
#define VRX (A0)
#define VRY (A1)

// MQ-3 pins
#define Mq3Pin (A5)

// joystick sensor values
#define VR_MIN_VALUE (0)
#define VR_MAX_VALUE (1023)
#define VR_MID_VALUE (512)
#define VR_MID_UPPER (564)
#define VR_MID_LOWER (460)

/*
   Adjust value read from a joystick sensor.

   The joystick I use is not so accurate that it produces values that
   are slightly off mid values even though it is placed in the middle.
   This causes the car to move around without any operation. So I set
   10% range that is considered in the middle.

   @param value the value read from the joystick
   @returns adjusted integer value between 0 and 1023
*/
int adjustJoystickValue(int value) {
  value = constrain(value, VR_MIN_VALUE, VR_MAX_VALUE);
  if (VR_MID_LOWER <= value && value <= VR_MID_UPPER)
    return VR_MID_VALUE;
  else
    return value;
}

// HM-10 pins
#define BLE_RX (4)
#define BLE_TX (5)

// HM-10 module instance
SoftwareSerial bleSerial(BLE_RX, BLE_TX);

int redPin = 7;
int greenPin = 6;
int bluePin = 5;

void setup() {
  bleSerial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  // read joystick values
  int vrx = analogRead(VRX); //analogRead(A0)
  int vry = analogRead(VRY); //analogRead(A1)
  //int read_mq3 = analogRead(Mq3Pin); //analogRead(A5)

  //if (read_mq3 < 80) {
  // ignore 10% error around 512 since it not accurate
  vrx = adjustJoystickValue(vrx);
  vry = adjustJoystickValue(vry);

  //Light values
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 255);

  // send data over BLE
  bleSerial.print(vrx);
  bleSerial.print(",");
  bleSerial.println(vry);
  //}
  /*
    else{

      analogWrite(redPin, 255);
      analogWrite(greenPin, 0);
      analogWrite(bluePin, 0);
    }
  */
}
