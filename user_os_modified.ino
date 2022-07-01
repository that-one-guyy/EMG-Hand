#include <Wire.h>
#include <Servo.h>
#define servoPin1 8              //  Pin where the servo for the 4 main fingers are
#define servoPin2 9                //  Pin where the servo for the thumb is
#define buttonPin 3
#define potentiometerPin A0
#define pressurePin1 A3
#define pressurePin2 A6
#define pressurePin3 A9
#define pressurePin4 A12
#define pressurePin5 A15

#define GRIPPER_MINIMUM_STEP 5     //  5 degree dead zone
#define Complete 14


Servo Fingers;                     //  Servo for the main 4 fingers
Servo Thumb;                       //  Servo for the thumb

int buttonState = 0;               // reads if button is pressed or not
int calibrationStage = 1;
int potVal = 0;                           // potentiometer reading
int servoOneVal = 0;
int servoTwoVal = 0;
int avgPressure;                   // average pressure on all 5 pads
int noPressure = 0;                // max value for no pressure on the pad
int lightPressure = 0;             // max value for light pressure on the pad
int mediumPressure = 0;            // max value for medium pressure on the pad
int highPressure = 0;              // max value for high pressure on the pad
int lightDegree = 0;               // servo degree for light pressure
int mediumDegree = 0;              // servo degree for medium pressure
int highDegree = 0;                // servo degree for high pressure
int oldDegrees = 0;                // old stored value for servo
int newDegree = 0;                 // new servo degree for each reading
int analogReadings = 0;

int sensorAmount = 5;

unsigned long triggerTime = 0;
unsigned long lastTrigger = 0;

volatile byte dataRecieved[2];

// -------------------------------------------------
// Boots up screen
// Clears screen
// Sets the button as an input
// Sets the servo pins
// Begins I2C communication
// -------------------------------------------------
void setup(){
  Serial.begin(9600);
  delay (100);
  Serial.println("Hello");
  Fingers.attach(servoPin1);
  Thumb.attach(servoPin2);
  Wire.begin();
  pinMode(buttonPin, INPUT);
  int q = 0;
  attachInterrupt(digitalPinToInterrupt(buttonPin), pin_ISR, CHANGE);

  Fingers.write(15);
  Thumb.write(75);
  
  Calibration();
}

// -------------------------------------------------
// Manually sets up pressure levels
// Sets pressure levels
// Sets servo position
// resets the screen
// tells user what to do
// Manual movement control is desired
// -------------------------------------------------
        
void Calibration(){
  unsigned long timing = millis();
  if (calibrationStage == 1){
    Serial.println("Calibration?");
    delay(100);
    Serial.println("Click for yes");
    delay(300);
    if (millis() - timing > 15000 && calibrationStage == 1){
      Serial.println("Timed out");
      return;
    }
  }
  
  timing = millis();
  if (calibrationStage == 2) {
    Serial.println();
    Serial.println("Adjust grip to light pressure with knob");
    delay(100);
    Serial.println("Push button when done");
    delay(500);
    timing = millis();
    calibrationStage++;
    
    delay(300);
    if (millis() - timing > 20000 && (calibrationStage == 2)){
      Serial.println("Timed out");
      return;
    }
  }
  
  timing = millis();
  if (calibrationStage == 4){
    lightDegree = potVal;
    sensorAmount = 5;
    sensorAmount = pressureCheck(pressurePin1, sensorAmount);
    sensorAmount = pressureCheck(pressurePin2, sensorAmount);
    sensorAmount = pressureCheck(pressurePin3, sensorAmount);
    sensorAmount = pressureCheck(pressurePin4, sensorAmount);
    sensorAmount = pressureCheck(pressurePin5, sensorAmount);

    if (sensorAmount == 0) {
      lightPressure = 0;
    }
    else {
      lightPressure = (analogRead(pressurePin1) + analogRead(pressurePin2) + analogRead(pressurePin3) + analogRead(pressurePin4) + analogRead(pressurePin5)) / sensorAmount;
    }
    // lightPressure = (analogRead(pressurePin1) + analogRead(pressurePin2) + analogRead(pressurePin3) + analogRead(pressurePin4) + analogRead(pressurePin5))/5;
    Serial.println();
    Serial.print("Average light pressure: ");
    Serial.println(lightPressure);
    delay(100);
    calibrationStage++;
  }
  
  timing = millis();
  if (calibrationStage == 5){
    Serial.println();
    Serial.println("Adjust grip to medium pressure with knob");
    delay(100);
    Serial.println("Push button when done");
    delay(200);
    timing = millis();
    calibrationStage++;
    
    delay(300);             
    if (millis() - timing > 20000 && (calibrationStage == 4)){
      Serial.println("Timed out");
      return;
    }
  }
  
  timing = millis();
  if (calibrationStage == 7){
    mediumDegree = potVal;
    if (mediumDegree <= lightDegree) {
      Serial.println();
      Serial.println("Medium pressure is lighter than light pressure");
      Serial.println("Press button to readjust medium pressure");
      calibrationStage = 4;
      timing = millis();
      return;
    }
    
    mediumDegree = potVal;
    sensorAmount = 5;
    sensorAmount = pressureCheck(pressurePin1, sensorAmount);
    sensorAmount = pressureCheck(pressurePin2, sensorAmount);
    sensorAmount = pressureCheck(pressurePin3, sensorAmount);
    sensorAmount = pressureCheck(pressurePin4, sensorAmount);
    sensorAmount = pressureCheck(pressurePin5, sensorAmount);

    if (sensorAmount == 0) {
      mediumPressure = 0;
    }
    else {
      mediumPressure = (analogRead(pressurePin1) + analogRead(pressurePin2) + analogRead(pressurePin3) + analogRead(pressurePin4) + analogRead(pressurePin5)) / sensorAmount;
    }
   
    Serial.println();
    Serial.print("Average medium pressure: ");
    Serial.println(mediumPressure);
    delay(100);              
    calibrationStage++;
  }
  
  timing = millis();
  if (calibrationStage == 8){
    Serial.println();
    Serial.println("Adjust grip to high pressure with knob");
    delay(100);
    Serial.println("Push button when done");
    delay(200);
    timing = millis();
    calibrationStage++;
    delay(300);                           
    if (millis() - timing > 20000 && (calibrationStage == 6)){
      Serial.println("Timed out");
      return;
    }
  }
  
  timing = millis();
  if (calibrationStage == 10){
    highDegree = potVal;
    if (highDegree <= mediumDegree) {
      Serial.println();
      Serial.println("High pressure is lighter than medium pressure");
      Serial.println("Press button to readjust high pressure");
      calibrationStage = 7;
      timing = millis();
      return;
    }
    
    highDegree = potVal;
    sensorAmount = 5;
    sensorAmount = pressureCheck(pressurePin1, sensorAmount);
    sensorAmount = pressureCheck(pressurePin2, sensorAmount);
    sensorAmount = pressureCheck(pressurePin3, sensorAmount);
    sensorAmount = pressureCheck(pressurePin4, sensorAmount);
    sensorAmount = pressureCheck(pressurePin5, sensorAmount);

    if (sensorAmount == 0) {
      highPressure = 0;
    }
    else {
      highPressure = (analogRead(pressurePin1) + analogRead(pressurePin2) + analogRead(pressurePin3) + analogRead(pressurePin4) + analogRead(pressurePin5)) / sensorAmount;
    }
    
    Serial.println();
    Serial.print("Average highpressure: ");
    Serial.println(highPressure);
    delay(100);
    Serial.println();
    Serial.println("Calibration complete");
    delay(100);
    Serial.println("Mode selection:");
    delay(100);
    Serial.println();
    Serial.println("Push button once for manual");
    delay(100);
    Serial.println("Push button twice for automatic");
    timing = millis();
    delay(300);                               
    if (millis() - timing > 10000 && (calibrationStage == 7)){
      Serial.println("Timed out");
      return;
    }
  }
  
  timing=millis();
  if (calibrationStage == 11){
    Serial.println();
    Serial.println("Manual control");
    delay(100);
    Serial.println("Push to exit");
    delay(200);                                        
    calibrationStage++;
  }
  
  timing=millis();
  if (calibrationStage == 13){
    calibrationStage == Complete;
    Serial.println();
    // Serial.println("Calibration finished");
    delay(100);
    Serial.println("Press button once to enable EEG/EMG control");
    delay(200);
    Serial.println("Press button twice to terminate program");
  }   
  if (calibrationStage == 15) {
    Fingers.write(15);
    Thumb.write(75);
    Serial.println("Program terminated.");
    Serial.println("Please reset to restart program");
    calibrationStage++;
  }                                
}

// -------------------------------------------------
// constantly transmitts the pressure variables
// reads the pads and takes an average
// displays pressure category and reading
// resets the screen
// -------------------------------------------------

void loop() {
  while (calibrationStage == 3 || calibrationStage == 6 || calibrationStage == 9 || calibrationStage == 12) {
    potVal = analogRead(potentiometerPin);
    servoOneVal = map(potVal,0,1023,15,85);
    servoTwoVal = map(potVal,0,1023,75,0);
    Fingers.write(servoOneVal);
    Thumb.write(servoTwoVal);
    if (calibrationStage == 12) {
      sensorAmount = 5;
      sensorAmount = pressureCheck(pressurePin1, sensorAmount);
      sensorAmount = pressureCheck(pressurePin2, sensorAmount);
      sensorAmount = pressureCheck(pressurePin3, sensorAmount);
      sensorAmount = pressureCheck(pressurePin4, sensorAmount);
      sensorAmount = pressureCheck(pressurePin5, sensorAmount);
    
      if (sensorAmount == 0) {
        Serial.println("Average pressure: 0");
      }
      else {
        Serial.print("Average pressure: ");
        Serial.println((analogRead(pressurePin1) + analogRead(pressurePin2) + analogRead(pressurePin3) + analogRead(pressurePin4) + analogRead(pressurePin5)) / sensorAmount);
      }
      delay(200);
    }
  }
   
  while (calibrationStage == Complete) {
    delay(300);
    Wire.requestFrom(9,2);
    if (Wire.available()) {
      for (byte i=0; i<2;i++) {
        dataRecieved[i] = Wire.read();
      }
      analogReadings = dataRecieved[0];
      analogReadings = analogReadings << 8 | dataRecieved[1];
      Serial.println();
      Serial.println(analogReadings);
    }
    else if (!Wire.available()) {
      Serial.println("I2C transmission error");
      Serial.println("Retrying in 5 seconds");
      analogReadings = 0;
      delay(5000);
    }
    
    if (analogReadings <= 200) {
      
      newDegree = 0;
      avgPressure = (analogRead(pressurePin1) + analogRead(pressurePin2) + analogRead(pressurePin3) + analogRead(pressurePin4) + analogRead(pressurePin5))/5; // reading the pad
  
      if (avgPressure < noPressure) {
        Serial.println("None            ");  // displays none if there if no pressure being applied
      }
      else if (avgPressure < lightPressure) {
        Serial.println("Light                     "); // displays light if the pressure is within the range
        delay(1);
        Serial.println (avgPressure);  // displays pressure reading
      } 
      else if (avgPressure < mediumPressure) {
        Serial.println("Medium                      "); // displays medium if the pressure is within the range
        delay(1);
      } 
      else{
        Serial.println("High                     "); // displays high if the pressure is above the medium range
        delay(1);
        Serial.println (avgPressure);  // displays pressure reading
      }
    }
  
    else if (200 < analogReadings <= 520){
    
      newDegree = lightDegree;
      avgPressure = (analogRead(pressurePin1) + analogRead(pressurePin2) + analogRead(pressurePin3) + analogRead(pressurePin4) + analogRead(pressurePin5))/5; // reading the pad
  
      if (avgPressure < noPressure) {
        Serial.println("None            ");  // displays none if there if no pressure being applied
      } 
      else if (avgPressure < lightPressure) {
        Serial.println("Light                     "); // displays light if the pressure is within the range
        delay(1);
        Serial.println (avgPressure);  // displays pressure reading
      } 
      else if (avgPressure < mediumPressure) {
        Serial.println("Medium                      "); // displays medium if the pressure is within the range
        delay(1);
        Serial.println (avgPressure);  // displays pressure reading
      } 
      else{
        Serial.println("High                     "); // displays high if the pressure is above the medium range;
        delay(1);
        Serial.println (avgPressure);  // displays pressure reading
      }
    }
    else if (520 < analogReadings <= 8400){
    
      newDegree = mediumDegree;
      avgPressure = (analogRead(pressurePin1) + analogRead(pressurePin2) + analogRead(pressurePin3) + analogRead(pressurePin4) + analogRead(pressurePin5))/5; // reading the pad
  
      if (avgPressure < noPressure) {
        Serial.println("None            ");  // displays none if there if no pressure being applied
      } 
      else if (avgPressure < lightPressure) {
        Serial.println("Light                     "); // displays light if the pressure is within the range
        delay(1);
        Serial.println (avgPressure);  // displays pressure reading
      } 
      else if (avgPressure < mediumPressure) {
        Serial.println("Medium                      "); // displays medium if the pressure is within the range
        delay(1);
        Serial.println (avgPressure);  // displays pressure reading
      } 
      else{
        Serial.println("High                     "); // displays high if the pressure is above the medium range
        delay(1);
        Serial.println (avgPressure);  // displays pressure reading
      }
    }
    else if (800 < analogReadings <= 1000){
    
      newDegree = highDegree;
      avgPressure = (analogRead(pressurePin1) + analogRead(pressurePin2) + analogRead(pressurePin3) + analogRead(pressurePin4) + analogRead(pressurePin5))/5; // reading the pad
  
      if (avgPressure < noPressure) {
        Serial.println("None            ");  // displays none if there if no pressure being applied
      } 
      else if (avgPressure < lightPressure) {
        Serial.println("Light                     "); // displays light if the pressure is within the range
        delay(1);
        Serial.print (avgPressure);  // displays pressure reading
      } 
      else if (avgPressure < mediumPressure) {
        Serial.println("Medium                      "); // displays medium if the pressure is within the range
        delay(1);
        Serial.println (avgPressure);  // displays pressure reading
      } 
      else{
        Serial.println("High                     "); // displays high if the pressure is above the medium range
        delay(1);
        Serial.println (avgPressure);  // displays pressure reading
      }
    }
    
    // delay(1000);
                  
    if(abs(newDegree-oldDegrees) > GRIPPER_MINIMUM_STEP){
      servoOneVal = map(newDegree, 40, 350, 15, 75);
      servoTwoVal = map(newDegree, 40, 350, 75, 0);
      
      // sets the servo angle
      Fingers.write(servoOneVal);
      Thumb.write(servoTwoVal); 
    }
    
    oldDegrees = newDegree;
  }
}


void pin_ISR(){
  triggerTime = millis();
  if ((triggerTime - lastTrigger) > 250) {
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH){
      calibrationStage++;
      // Serial.print("Stage:");
      // Serial.println(calibrationStage);
      Calibration();
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
