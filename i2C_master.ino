#include <Wire.h>
byte dataSent[8];
int first = 1020;
int second = 1200;
int third = 30;

int potentiometerPin = A0;
int potVal;

void setup() {
  Serial.begin(9600);
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
}

void loop() {
  delay(100);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  potVal = analogRead(A0);
  dataSent[0] = (first >> 8) & 0xFF;
  dataSent[1] = first & 0xFF;
  dataSent[2] = (second >> 8) & 0xFF;
  dataSent[3] = second & 0xFF;
  dataSent[4] = (third >> 8) & 0xFF;
  dataSent[5] = third & 0xFF;

  dataSent[6] = (potVal >> 8) & 0xFF;
  dataSent[7] = potVal & 0xFF;
  Wire.write(dataSent,8);
}
