
#include <SoftwareSerial.h>

// HM-10 pins
#define BLE_RX (4)
#define BLE_TX (5)

// HM-10 module instance
SoftwareSerial bleSerial(BLE_RX, BLE_TX);

// joystick sensor values
#define VR_MIN_VALUE (0)
#define VR_MAX_VALUE (1023)

// left motor pins
#define ENA 6
#define IN1 7
#define IN2 8

// right motor pins
#define ENB 11
#define IN3 9
#define IN4 10

#define MOTOR_MIN_PWM (95)
#define MOTOR_MAX_PWM (255)

int convertToPWM(int value) {
  return map(value, VR_MIN_VALUE, VR_MAX_VALUE, -MOTOR_MAX_PWM, MOTOR_MAX_PWM);
}


void drive(int pwmL, int pwmR) {
  // constrain motor speed between -255 and 255
  pwmL = constrain(pwmL, -MOTOR_MAX_PWM, MOTOR_MAX_PWM);
  pwmR = constrain(pwmR, -MOTOR_MAX_PWM, MOTOR_MAX_PWM);
  
  // left motor direction
  if (pwmL > 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else if (pwmL < 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }
  
  // right motor direction
  if (pwmR > 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else if (pwmR < 0) {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }

  // stop motors if the absolute value of pwm is less than MOTOR_MIN_PWM
  if (abs(pwmL) <= MOTOR_MIN_PWM) pwmL = 0;
  if (abs(pwmR) <= MOTOR_MIN_PWM) pwmR = 0;
  
  // pwm for each motor.
  analogWrite(ENA, abs(pwmL));
  analogWrite(ENB, abs(pwmR));
}

void setup() {
  // set up HM-10 module
  bleSerial.begin(9600);

  // set all the motor pins OUTPUT mode
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  // received controll command
  if (bleSerial.available() > 0) {
    // read joystick values from analog pins
    int vrx = bleSerial.parseInt();
    if(vrx > 0){
      
      digitalWrite(ENA, HIGH);
      }
    int vry = bleSerial.parseInt();
  
    // calculate speed of both motors according to the x axis
    int pwmX = convertToPWM(vrx);
    int pwmL = pwmX;
    int pwmR = pwmX;
  
    // adjust left and right speed according to the y axis
    int pwmY = convertToPWM(vry);
    pwmL = pwmL + pwmY;
    pwmR = pwmR - pwmY;
    
    // happy drive!
    drive(pwmL, pwmR);
  }
}
