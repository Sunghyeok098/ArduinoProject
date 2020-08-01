#include <SoftwareSerial.h>


// HM-10 pins
#define BLE_RX (4)
#define BLE_TX (5)

// joystick pins
#define VRX (A0)
#define VRY (A1)
#define VRZ (A2)

// pluse pins
#define PulsePin (A5)

// joystick sensor values
#define VR_MIN_VALUE (0)
#define VR_MAX_VALUE (1023)
#define VR_MID_VALUE (512)
#define VR_MID_UPPER (564)
#define VR_MID_LOWER (460)

SoftwareSerial BTSerial(BLE_RX, BLE_TX);

int adjustJoystickValue(int value) {
  value = constrain(value, VR_MIN_VALUE, VR_MAX_VALUE);
  if (VR_MID_LOWER <= value && value <= VR_MID_UPPER)
    return VR_MID_VALUE;
  else
    return value;
}

void setup() {
  pinMode(VRX, INPUT);
  pinMode(VRY, INPUT);
  pinMode(VRZ, INPUT);
  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop() {

  int vrx = analogRead(VRX); //analogRead(A0)
  int vry = analogRead(VRY); //analogRead(A1)
  int vrz = analogRead(VRZ); //analogRead(A2)

  vrx = adjustJoystickValue(vrx);
  vry = adjustJoystickValue(vry);
  vrz = adjustJoystickValue(vrz);

  Serial.print(vrx);
  Serial.print(',');
  Serial.print(vry);
  Serial.print(',');
  Serial.println(vrz);
  delay(100);

  BTSerial.print(1);
  BTSerial.print(",");
  BTSerial.print(vrx);
  BTSerial.print(",");
  BTSerial.print(vry);
  BTSerial.print(",");
  BTSerial.println(vrz);

}
