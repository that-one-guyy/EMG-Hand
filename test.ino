#include <Wire.h>
#include <Servo.h>
#define Fingers_PIN 8              //Pin where the servo for the 4 main fingers are
#define Thumb_PIN 9                //Pin where the servo for the thumb is
#define GRIPPER_MINIMUM_STEP 5     // 5 degree dead zone

Servo Fingers;                     //Servo for the main 4 fingers
Servo Thumb;                       //Servo for the thumb

int potentiometer = A0;      //Pin where potentiometeriometer is located
int pressurePin1 = A3;       //Pin where our pressure pad is located.
int pressurePin2 = A6;       //Pin where second pressure pad is located
int pressurePin3 = A9;       //Pin where third pressure pad is located
int pressurePin4 = A12;       //Pin where fourth pressure pad is located
int pressurePin5 = A15;       //Pin where fifth pressure pad is located
const int buttonPin = 3;           //sets pin for the button

int buttonState = 0;               //reads if button is pressed or not
int buttonCount = 0;               //amount of times the button has been pressed
int val;                           //potentiometer reading
unsigned long oldTime = 0;         //timestamp of last servo update
int pressureReading;               //average pressure on all 5 pads
int noPressure = 0;                //max value for no pressure on the pad
int lightPressure = 0;             //max value for light pressure on the pad
int mediumPressure = 0;            //max value for medium pressure on the pad
int highPressure = 0;              //max value for high pressure on the pad
int lightDegree = 0;               //servo degree for light pressure
int mediumDegree = 0;              //servo degree for medium pressure
int highDegree = 0;                //servo degree for high pressure
int oldDegrees = 0;                //old stored value for servo
int newDegree = 0;                 //new servo degree for each reading
int analogReadings = 0;

unsigned long triggerTime = 0;
unsigned long lastTrigger = 0;


//-------------------------------------------------
//Boots up screen
//Clears screen
//Sets the button as an input
//Sets the servo pins
//Begins I2C communication
//-------------------------------------------------
void setup(){
  Serial.begin(9600);
  // Wire.begin(9);
  delay (100);
  /*
  Serial.write (0xFE);
  Serial.write (0x51);
  delay(3);
  Serial.print ("H");
  Serial.write (0xFE);
  Serial.write (0x45);
  Serial.write (0x40);
  delay (100);
  Serial.print ("I");
  */

  Serial.println("Test");
  
  Fingers.attach(Fingers_PIN);
  Thumb.attach(Thumb_PIN);
  Wire.begin();
  pinMode(buttonPin, INPUT);
  pinMode(potentiometer, INPUT);

  pinMode(pressurePin1, INPUT);
  pinMode(pressurePin2, INPUT);
  pinMode(pressurePin3, INPUT);
  pinMode(pressurePin4, INPUT);
  pinMode(pressurePin5, INPUT);
  
  int q = 0;
  attachInterrupt(digitalPinToInterrupt(buttonPin), pin_ISR, CHANGE);
  delay (2000);
}

int count = 0;
void loop() {
  // Basic servo testing
  /*
  Serial.println(count);
  Fingers.write(100);
  Thumb.write(180);
  delay (2000);
  Fingers.write(145);
  Thumb.write(135);
  delay (2000);
  Fingers.write(200);
  Thumb.write(90);
  delay (2000);
  count = count + 1;
  */

  /*
  // I2C Communication testing
  delay(1);
  int bytesRecieved = Wire.requestFrom(9,5);
  if (bytesRecieved < 5) {
    Serial.print("Error");
  }
  while (Wire.available()){
    analogReadings = Wire.read();
    // Serial.println("Wire available");
    delay(1);
  }
  // analogReadings = Wire.read();
  Serial.println("I2C Reading:");
  Serial.println(analogReadings);
  delay(2000);
  */

  // Button testing
  /*
  buttonState = digitalRead(buttonPin);
  if(buttonState == HIGH) {
   // Serial.println("Button is pressed");
   delay(1000);
  }
  else {
   
  }
  */
  
  // Potentiometer testing
  /*
  int potVal = analogRead(A0);
  if (potVal < 256) {
   Serial.println("Potentiometer under 25% of full range");
  }
  else if (potVal >= 256 && potVal < 512) {
   Serial.println("potentiometer between 25% and 50%");
  }
  else if (potVal >= 512 && potVal < 768) {
   Serial.println("potentiometer between 50% and 75%");
  }
  else if (potVal >= 768 && potVal < 1024) {
   Serial.println("potentioneter above 75% of full range");
  }
  else {
   Serial.println("Range error.");
  }
  delay(1000);
  */

  // Servo + Pot testing
  // /*
  int val = analogRead(A0);
  int val1 = map(val,0, 1023, 75, 0);
  int val2 = map(val, 0, 1023, 15, 75);
  Thumb.write(val1);
  Fingers.write(val2);
  // Serial.println(val);
  delay(200);
  // */

  // Pressure sensor testing
  // /* 
  
  Serial.println(analogRead(pressurePin1));
  Serial.println(analogRead(pressurePin2));
  Serial.println(analogRead(pressurePin3));
  Serial.println(analogRead(pressurePin4));
  Serial.println(analogRead(pressurePin5));
  Serial.println();
  
  int sensorAmount = 5;
  int avgForce = 0;
  sensorAmount = pressureCheck(pressurePin1, sensorAmount);
  sensorAmount = pressureCheck(pressurePin2, sensorAmount);
  sensorAmount = pressureCheck(pressurePin3, sensorAmount);
  sensorAmount = pressureCheck(pressurePin4, sensorAmount);
  sensorAmount = pressureCheck(pressurePin5, sensorAmount);

  if (sensorAmount == 0) {
    avgForce = 0;
  }
  else {
    avgForce = (analogRead(pressurePin1) + analogRead(pressurePin2) + analogRead(pressurePin3) + analogRead(pressurePin4) + analogRead(pressurePin5)) / sensorAmount;
  }
  Serial.println(avgForce);
  Serial.println();
  // delay(1700);
  // */

  /*
 int pos = analogRead(A0);
 int pos1 = map(pos,0,1023,5,175);
 int pos2 = map(pos,0,1023,175,5);
 Fingers.write(pos1);
 Thumb.write(pos2);
 */
 
 delay(100);
   
}

void pin_ISR(){
  triggerTime = millis();
  if ((triggerTime - lastTrigger) > 200) {
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH){
      buttonCount++;
      // Serial.println(buttonCount);
      // Serial.println();
    }
  }
  lastTrigger = triggerTime;
}

int pressureCheck(int pressurePin, int sensorAmount) {
  int indvForce = analogRead(pressurePin);

  if (indvForce < 30) {
    sensorAmount = sensorAmount - 1;
  }
  return sensorAmount;
}
