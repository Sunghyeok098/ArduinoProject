#include <SoftwareSerial.h>
SoftwareSerial BTSerial(4, 5);  // (Tx, Rx)

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop() {
  //String str =  BTSerial.read();

  //Serial.println(var);
  int vrx = BTSerial.parseInt();
  int vry = BTSerial.parseInt();
  int vrz = BTSerial.parseInt();

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
