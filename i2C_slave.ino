#include <Wire.h>
volatile byte dataRecieved[8];
int first;
int second;
int third;
int potVal;

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  Serial.println("Setup done");
}

void loop() {
  Wire.requestFrom(8, 8);

  while (Wire.available()) { // peripheral may send less than requested
    for (byte i=0; i<8; i++) {
      dataRecieved[i] = Wire.read();
    }
    
    first = dataRecieved[0];
    first = first << 8 | dataRecieved[1];
    second = dataRecieved[2];
    second = second << 8 | dataRecieved[3];
    third = dataRecieved[4];
    third = third << 8 | dataRecieved[5];

    potVal = dataRecieved[6];
    potVal = potVal << 8 | dataRecieved[7];
    
    Serial.println(first);
    Serial.println(second);
    Serial.println(third);
    Serial.println(potVal);
    Serial.println();
  }

  delay(2000);
}
