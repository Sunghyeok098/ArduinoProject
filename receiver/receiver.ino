#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 5); //블루투스의 Tx, Rx핀을 2번 3번핀으로 설정

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

//alcohol senser pin
#define alco_pin (A0) // !!! alcohol sensor pin number 지정!!

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

  mySerial.begin(9600); // baud rate 9600으로 설정

  // set all the motor pins OUTPUT mode
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // set alcohol pins INPUT mode
  pinMode(alco_pin, INPUT);
}



void loop() {

  if (mySerial.available()) { // 넘어온 데이터가 존재하면

    int flag = mySerial.parseInt(); //bmp control value
    //int flag = 1; //bmp control value
    int vrx = mySerial.parseInt();
    int vry = mySerial.parseInt();
    Serial.print(flag);
    Serial.print(",");
    Serial.print(vrx);
    Serial.print(",");
    Serial.println(vry);
    //Serial.write(mySerial.read()); // 시리얼에 출력

    // calculate speed of both motors according to the x axis
    int pwmX = convertToPWM(vrx);
    int pwmL = pwmX;
    int pwmR = pwmX;

    // adjust left and right speed according to the y axis
    int pwmY = convertToPWM(vry);
    pwmL = pwmL + pwmY;
    pwmR = pwmR - pwmY;

    if (flag == 0) { //bpm 이 정상범위가 아니면
      //LED red
    }

    else if (flag == 2) {

      int alco_value = analogRead(alco_pin); //alcohol senser pin read

      //LED 불 하는거 blue

      Serial.println(123221);

      //범위 알아서 지정
      if (alco_value < 50) { //alcohol sensor value is normal, drive RC car
        //LED green
        drive(pwmL, pwmR);
      }
    }

    else { //bpm 정상범위이면, drive RC car
      //LED green
      drive(pwmL, pwmR);
    }

  }



}
