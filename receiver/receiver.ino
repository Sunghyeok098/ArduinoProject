#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 5); //블루투스의 Tx, Rx핀을 4번 5번핀으로 설정

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

//LED pin
#define LED_R 2       // R의 핀 번호 2
#define LED_G 3       // G의 핀 번호 3
#define LED_B 12       // B의 핀 번호 12

//alcohol senser pin
#define alco_pin (A0) // !!! alcohol sensor pin number 

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

  Serial.begin(9600);
  mySerial.begin(9600); // 

  // set all the motor pins OUTPUT mode
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // set pin mode 
  pinMode(LED_B, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);

  // set alcohol pins INPUT mode
  pinMode(alco_pin, INPUT);
}



void loop() {

  if (mySerial.available()) { 

    int flag = mySerial.parseInt(); //bmp control value
    int vrx = mySerial.parseInt();
    int vry = mySerial.parseInt();

    Serial.print(flag);
    Serial.print(",");
    Serial.print(vrx);
    Serial.print(",");
    Serial.println(vry);
  
    // calculate speed of both motors according to the x axis
    int pwmX = convertToPWM(vrx);
    int pwmL = pwmX;
    int pwmR = pwmX;

    // adjust left and right speed according to the y axis
    int pwmY = convertToPWM(vry);
    pwmL = pwmL + pwmY;
    pwmR = pwmR - pwmY;

    if (flag == 0) { //bpm 이 정상범위가 아니면
      digitalWrite(LED_R, HIGH);      //LED red
      digitalWrite(LED_G, LOW);
      digitalWrite(LED_B, LOW);
    }

    else if (flag == 2) {

      digitalWrite(LED_R, LOW);
      digitalWrite(LED_G, LOW);
      digitalWrite(LED_B, HIGH);     // LED blue
      delay(500);

      int alco_value = analogRead(alco_pin); //alcohol senser pin read
      Serial.println(analogRead(alco_pin));
      
      if (alco_value < 80) { //alcohol sensor value is normal, drive RC car
        digitalWrite(LED_G, HIGH);     //LED green
        digitalWrite(LED_B, LOW);
        digitalWrite(LED_R, LOW);
        drive(pwmL, pwmR);
      }

      else {
        digitalWrite(LED_R, HIGH);      //LED red
        digitalWrite(LED_G, LOW);
        digitalWrite(LED_B, LOW);

      }
    }

    else { //bpm 정상범위이면, drive RC car
      digitalWrite(LED_G, HIGH);     //LED green
      digitalWrite(LED_R, LOW);
      digitalWrite(LED_B, LOW);
      drive(pwmL, pwmR);
    }

  }

}