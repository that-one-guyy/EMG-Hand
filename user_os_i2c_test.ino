#include <Wire.h>
volatile byte recieved[2];
int data;

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  Serial.println("Setup done");
}

void loop() {
  Wire.requestFrom(9,2);

  while(Wire.available()) {
    for (byte i=0; i<2; i++) {
      recieved[i] = Wire.read();
    }

    data = recieved[0];
    data = data << 8 | recieved[1];
    Serial.println(data);
    
  }
  delay(1000);
}
