#include <SoftwareSerial.h>
#include <Wire.h>                     // i2C 통신을 위한 라이브러리
#include <LiquidCrystal_I2C.h>        // LCD 2004 I2C용 라이브러리
LiquidCrystal_I2C lcd(0x27, 16, 2);   // 접근주소: 0x3F or 0x27
#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library. 
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"

// joystick pins
#define VRX (A1)
#define VRY (A2)

//  Variables
const int PulseWire = A0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.

// joystick sensor values
#define VR_MIN_VALUE (0)
#define VR_MAX_VALUE (1023)
#define VR_MID_VALUE (512)
#define VR_MID_UPPER (564)
#define VR_MID_LOWER (460)

int adjustJoystickValue(int value) {
  value = constrain(value, VR_MIN_VALUE, VR_MAX_VALUE);
  if (VR_MID_LOWER <= value && value <= VR_MID_UPPER)
    return VR_MID_VALUE;
  else
    return value;
}


SoftwareSerial mySerial(4, 5); 
const int delayMsec = 60; // 100msec per sample
int a = 0;
const int pulse_btn = 6;
const int emer_btn = 7;
float avg = 0;
int sum = 0;
int count = 0;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600); // baud rate 9600으로 설정

  // set btn mode
  pinMode(pulse_btn, INPUT);
  pinMode(emer_btn, INPUT);

  lcd.init();                      // LCD 초기화
  lcd.backlight();                // 백라이트 켜기

  // Configure the PulseSensor object, by assigning our variables to it.
  pulseSensor.analogInput(PulseWire);
  pulseSensor.setThreshold(Threshold);

  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.
  }

}



void loop() {

  int vrx = analogRead(VRX);
  int vry = analogRead(VRY);

  // ignore 10% error around 512 since it not accurate
  vrx = adjustJoystickValue(vrx);
  vry = adjustJoystickValue(vry);

  int btn1 = digitalRead(pulse_btn);

  int myBPM = pulseSensor.getBeatsPerMinute();

  if (btn1 == 0) {

    delay(1000);
    sum = 0;
    avg = 0;

    for (int i = 0; i < 20; i++) {

      int myBPM = pulseSensor.getBeatsPerMinute();
      sum = sum + myBPM;
      delay(200);

    }

    delay(1000);

  }

  avg = sum / 20;
  Serial.println(avg);

  if (avg >= 30 && avg <= 150) {
    lcd.clear();
    lcd.setCursor(0, 0);          // 첫번째 줄 문자열 출력
    lcd.print("BPM : ");
    lcd.print(avg);
    lcd.display();

  }

  else {
    lcd.setCursor(0, 0);          // 첫번째 줄 문자열 출력
    lcd.print("BPM NOT ESTIMATE");
    lcd.setCursor(0, 1);          // 첫번째 줄 문자열 출력
    lcd.display();

  }

  if (avg >= 30 && avg <= 100) {

    a = 1;
    lcd.setCursor(0, 1);
    lcd.print("Normal status");

    mySerial.print(a);
    mySerial.print(",");
    mySerial.print(vrx);
    mySerial.print(",");
    mySerial.println(vry);

    //Value print
    Serial.print(myBPM);
    Serial.print(',');
    Serial.print(a);
    Serial.print(',');
    Serial.print(vrx);
    Serial.print(',');
    Serial.println(vry);

  }

  //Bmp 정상범위 X, send 0 with vrx and vry
  else {

    if (avg != 0) {

      int btn2 = digitalRead(emer_btn);
      Serial.println(btn2);

      if (btn2 == 0) {

        count++;
      }

      if (count % 2 == 0) {
        a = 0;

        lcd.setCursor(0, 1);
        lcd.print("Abnormal status");

        mySerial.print(a);
        mySerial.print(",");
        mySerial.print(vrx);
        mySerial.print(",");
        mySerial.println(vry);

        Serial.print(myBPM);
        Serial.print(',');
        Serial.print(a);
        Serial.print(',');
        Serial.print(vrx);
        Serial.print(',');
        Serial.println(vry);

      }

      else {
        
        a = 2;

        lcd.setCursor(0, 1);
        lcd.print("Emergency status");

        mySerial.print(a);
        mySerial.print(",");
        mySerial.print(vrx);
        mySerial.print(",");
        mySerial.println(vry);

        Serial.print(myBPM);
        Serial.print(',');
        Serial.print(a);
        Serial.print(',');
        Serial.print(vrx);
        Serial.print(',');
        Serial.println(vry);

      }
    }
  }

  delay(500);
}