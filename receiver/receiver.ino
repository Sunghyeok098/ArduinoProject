#include <SoftwareSerial.h>
SoftwareSerial BTSerial(4, 5);  // (Tx, Rx)

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);

  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
}

void loop() {
  //String str =  BTSerial.read();

  //Serial.println(var);
  int flag = BTSerial.parseInt();
  int vrx = BTSerial.parseInt();
  int vry = BTSerial.parseInt();
  int vrz = BTSerial.parseInt();

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
