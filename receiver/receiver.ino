#include <SoftwareSerial.h>
SoftwareSerial BTSerial(4, 5);  // (Tx, Rx)

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
    analogWrite(IN1, 255);
  } else if (pwmL < 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(IN2, 255);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }
  
  // right motor direction
  if (pwmR > 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(IN3, 255);
  } else if (pwmR < 0) {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(IN4, 255);
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
  Serial.begin(9600);
  BTSerial.begin(9600);

  // set all the motor pins OUTPUT mode
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  //pinMode(6, OUTPUT);
  //pinMode(7, OUTPUT);
  //pinMode(8, OUTPUT);
}

void loop() {
  //String str =  BTSerial.read();

  //Serial.println(var);
  int flag = BTSerial.parseInt();
  int vrx = BTSerial.parseInt();
  int vry = BTSerial.parseInt();
  int vrz = BTSerial.parseInt();

  int pwmX = convertToPWM(vrx);
  int pwmL = pwmX;
  int pwmR = pwmX;
  
  // adjust left and right speed according to the y axis
  int pwmY = convertToPWM(vry);
  pwmL = pwmL + pwmY;
  pwmR = pwmR - pwmY;
    
  // happy drive!
  drive(pwmL, pwmR);

  /*
  if (vrx < 200){
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
  }
  if (vrx >= 200 && vrx < 700){
    digitalWrite(7, HIGH);
    digitalWrite(6, LOW);
    digitalWrite(8, LOW);
  }
  if (vrx > 700){
    digitalWrite(8, HIGH);
    digitalWrite(7, LOW);
    digitalWrite(6, LOW);
  }
*/
  Serial.print(flag);
  Serial.print(',');
  Serial.print(vrx);
  Serial.print(',');
  Serial.print(vry);
  Serial.print(',');
  Serial.println(vrz);
  

  /*
    if (BTSerial.available())
    Serial.write(BTSerial.read());
    //if (Serial.available())
    //BTSerial.write(Serial.read());*/
}
