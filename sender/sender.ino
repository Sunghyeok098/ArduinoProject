#include <SoftwareSerial.h>
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   
#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.

//  Variables
const int PulseWire = A0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
// Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
// Otherwise leave the default "550" value.

PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"

// HM-10 pins
#define BLE_RX (4)
#define BLE_TX (5)

// joystick pins
#define VRX (A1)
#define VRY (A2)
#define VRZ (A3)

// joystick sensor values
#define VR_MIN_VALUE (0)
#define VR_MAX_VALUE (1023)
#define VR_MID_VALUE (512)
#define VR_MID_UPPER (564)
#define VR_MID_LOWER (460)

SoftwareSerial BTSerial(BLE_RX, BLE_TX);
const int delayMsec = 60; // 100msec per sample

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

  // Configure the PulseSensor object, by assigning our variables to it.
  pulseSensor.analogInput(PulseWire);
  pulseSensor.setThreshold(Threshold);

  // Double-check the "pulseSensor" object was created and "began" seeing a signal.
  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.
  }

}

void loop() {

  int vrx = analogRead(VRX); //analogRead(A1)
  int vry = analogRead(VRY); //analogRead(A2)
  
  vrx = adjustJoystickValue(vrx);
  vry = adjustJoystickValue(vry);

  int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
  // "myBPM" hold this BPM value now.

  if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened".
    Serial.println("♥  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
    Serial.print("BPM: ");                        // Print phrase "BPM: "
    Serial.println(myBPM);                        // Print the value inside of myBPM.
  }

                 
  //Value print
  //Serial.print(vrx);
  //Serial.print(',');
  //Serial.println(vry);

  /*
  Sending data to RC car 
  pulse control and control value RC car
  */

  //Bmp 정상범위, send 1 with vrx and vry
  if(myBPM < 100){

    BTSerial.print(1);
    BTSerial.print(",");
    BTSerial.print(vrx);
    BTSerial.print(",");
    BTSerial.println(vry);
  }

  //Bmp 정상범위 X, send 0 with vrx and vry
  else{
    BTSerial.print(0);
    BTSerial.print(",");
    BTSerial.print(vrx);
    BTSerial.print(",");
    BTSerial.println(vry);
  }
  

  delay(1000);  

}
