#include <Wire.h>
#include <Servo.h>
#define Fingers_PIN 3              //Pin where the servo for the 4 main fingers are
#define Thumb_PIN 4                //Pin where the servo for the thumb is
#define GRIPPER_MINIMUM_STEP 5     // 5 degree dead zone

Servo Fingers;                     //Servo for the main 4 fingers
Servo Thumb;                       //Servo for the thumb

int pressureAnalogPin1 = 0;        //Pin where our pressure pad is located.
int pressureAnalogPin2 = 1;        //Pin where second pressure pad is located
int pressureAnalogPin3 = 2;        //Pin where third pressure pad is located
int pressureAnalogPin4 = 3;        //Pin where fourth pressure pad is located
int pressureAnalogPin5 = 4;        //Pin where fifth pressure pad is located
int potent = 5;                    //Pin where potentiometer is located
const int buttonPin = 3;           //sets pin for the button

int buttonState = 0;               //reads if button is pressed or not
int buttonCount = 0;               //amount of times the button has been pressed
int val;                           //potentiometer reading
unsigned long oldTime = 0;         //timestap of last servo update
int pressureReading;               // average pressure on all 5 pads
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


//-------------------------------------------------
//Boots up screen
//Clears screen
//Sets the button as an input
//Sets the servo pins
//Begins I2C communication
//-------------------------------------------------
void setup(){
  Serial.begin(9600);
  delay (100);
  Serial.write (0xFE);
  Serial.write (0x51);
  delay(3);
  Serial.print ("H");
  Serial.write (0xFE);
  Serial.write (0x45);
  Serial.write (0x40);
  delay (100);
  Serial.print ("I");
  Fingers.attach(Fingers_PIN);
  Thumb.attach(Thumb_PIN);
  Wire.begin();
  pinMode(buttonPin, INPUT);
  int q = 0;
  attachInterrupt(0,pin_ISR, CHANGE);

}




        //-------------------------------------------------
        //Manually sets up pressure levels
        //Sets pressure levels
        //Sets servo position
        //resets the screen
        //tells user what to do
        //Manual movement control is desired
        //-------------------------------------------------
        
        void Calibration(){
          unsigned long timing = millis();
          int q = 0;
          while (buttonCount == 1){
            if (q == 0){
              Serial.println("Calibration?");
              Serial.write(0xFE);
              Serial.write(0x45);
              Serial.write(0x40);
              delay(1);
              Serial.print("Click for yes");
              q++;
            }
 
            delay(300);
            Serial.write(0xFE);
            Serial.write(0x51);
            if (millis() - timing > 15000 && buttonCount == 1){
              return;
            }
          }
          q = 0;
          timing = millis();
                while (buttonCount == 2) {
                  Serial.println("Adjust grip with knob");
                  Serial.write(0xFE);
                  Serial.write(0x45);
                  Serial.write(0x40);
                  delay(1);
                  Serial.print("Push button when done");
                  delay(200);
                  val = analogRead(potent);
                  val = map(val,0,1023,0,180);
                  Fingers.write(val);
                  Thumb.write(val);
                  timing = millis();
                  delay(300);
                  Serial.write(0xFE);
              if (millis() - timing > 20000 && (buttonCount == 2)){
                  return;
                  }
                }
              q=0;
              timing = millis();
                    while(buttonCount == 3){
                      lightDegree = val;
                      lightPressure = (analogRead(pressureAnalogPin1) + analogRead(pressureAnalogPin2) + analogRead(pressureAnalogPin3) + analogRead(pressureAnalogPin4) + analogRead(pressureAnalogPin5))/5;
                      delay(100);
                      Serial.println("push to continue");
                      timing = millis();
                      delay(300);
                      Serial.write(0xFE);
                      Serial.write(0x51);
                  if (millis() - timing > 10000 && (buttonCount == 3)){
                      return;
                      }
                    }
                  q=0;
                  timing = millis();
                        while(buttonCount == 4){
                          Serial.println("Adjust grip with knob");
                          Serial.write(0xFE);
                          Serial.write(0x45);
                          Serial.write(0x40);
                          delay(1);
                          Serial.print("Push button when done");
                          delay(200);
                          val = analogRead(potent);
                          val = map(val,0,1023,0,180);
                          Fingers.write(val);
                          Thumb.write(val);
                          timing = millis();
                          delay(300);                       
                          Serial.write(0xFE);
                          Serial.write(0x51);                    
                      if (millis() - timing > 20000 && (buttonCount == 4)){
                        return;
                          }
                        }
                      q=0;
                      timing = millis();
                            while(buttonCount == 5){
                              mediumDegree = val;
                              mediumPressure = (analogRead(pressureAnalogPin1) + analogRead(pressureAnalogPin2) + analogRead(pressureAnalogPin3) + analogRead(pressureAnalogPin4) + analogRead(pressureAnalogPin5))/5;
                              delay(100);
                              Serial.println("push to continue");
                              timing = millis();
                              delay(300);                            
                              Serial.write(0xFE);
                              Serial.write(0x51);                      
                          if (millis() - timing > 10000 && (buttonCount == 5)){
                              return;
                              }
                            }
                          q=0;
                          timing = millis();
                                while(buttonCount == 6){
                                  Serial.println("Adjust grip with knob");
                                  Serial.write(0xFE);
                                  Serial.write(0x45);
                                  Serial.write(0x40);
                                  delay(1);
                                  Serial.print("Push button when done");
                                  delay(200);
                                  val = analogRead(potent);
                                  val = map(val,0,1023,0,180);
                                  Fingers.write(val);
                                  Thumb.write(val);
                                  timing = millis();
                                  delay(300);                       
                                  Serial.write(0xFE);
                                  Serial.write(0x51);                            
                              if (millis() - timing > 20000 && (buttonCount == 6)){
                                  return;
                                  }
                                }
                              q=0;
                              timing = millis();
                                    while(buttonCount == 7){
                                      highDegree = val;
                                      highPressure = (analogRead(pressureAnalogPin1) + analogRead(pressureAnalogPin2) + analogRead(pressureAnalogPin3) + analogRead(pressureAnalogPin4) + analogRead(pressureAnalogPin5))/5;
                                      delay(100);
                                      Serial.println("push for manual");
                                      Serial.write(0xFE);
                                      Serial.write(0x45);
                                      Serial.write(0x40);
                                      delay(1);
                                      Serial.print("Push twice for emg");
                                      timing = millis();
                                      delay(300);                                      
                                      Serial.write(0xFE);
                                      Serial.write(0x51);                                
                                  if (millis() - timing > 10000 && (buttonCount == 7)){
                                      return;
                                      }
                                    }
                                    q=0;
                                    timing=millis();
                                        while(buttonCount == 8){
                                          Serial.println("Push to exit");
                                          Serial.write(0xFE);
                                          Serial.write(0x45);
                                          Serial.write(0x40);
                                          delay(1);
                                          Serial.print("Manual control");
                                          delay(200);
                                          val = analogRead(potent);
                                          val = map(val,0,1023,0,180);
                                          Fingers.write(val);
                                          Thumb.write(val);
                                          timing = millis();
                                          delay(300);                                        
                                          Serial.write(0xFE);
                                          Serial.write(0x51);                                         
                                            if (millis() - timing > 30000 && (buttonCount == 8)){
                                              return;
                                              }
                                            }
                                            q=0;
                                            timing=millis();
                                            while(buttonCount == 9){
                                              buttonCount == 0;
                                            }
                                     
          }





            //-------------------------------------------------
            //constantly transmitts the pressure variables
            //reads the pads and takes an average
            //displays pressure category and reading
            //resets the screen
            //-------------------------------------------------
            
            void loop() {
              delay(300);
              Wire.requestFrom(9,5);
              while (Wire.available()){
                analogReadings = Wire.read();
              }
                    if (analogReadings < 200){
                      
                        newDegree = 0;
                        pressureReading = (analogRead(pressureAnalogPin1) + analogRead(pressureAnalogPin2) + analogRead(pressureAnalogPin3) + analogRead(pressureAnalogPin4) + analogRead(pressureAnalogPin5))/5; //reading the pad
                  
                      if (pressureReading < noPressure) {
                        Serial.println(" None            ");  //displays none if there if no pressure being applied
                      } 
                      else if (pressureReading < lightPressure) {
                        Serial.println(" Light                     "); //displays light if the pressure is within the range
                        Serial.write(0xFE);  //moves down to the second line
                        Serial.write(0X45);
                        Serial.write(0x40);
                        delay(1);
                        Serial.print (pressureReading);  //displays pressure reading
                      } 
                      else if (pressureReading < mediumPressure) {
                        Serial.println(" Medium                      "); //displays medium if the pressure is within the range
                        Serial.write(0xFE);  //moves down to the second line
                        Serial.write(0X45);
                        Serial.write(0x40);
                        delay(1);
                        Serial.print (pressureReading);  //displays pressure reading
                      } 
                      else{
                        Serial.println(" High                     "); //displays high if the pressure is above the medium range
                        Serial.write(0xFE); //moves down to the second line
                        Serial.write(0X45);
                        Serial.write(0x40);
                        delay(1);
                        Serial.print (pressureReading);  //displays pressure reading
                  }
                  }
            
                        if (200 < analogReadings <= 520){
                        
                          newDegree = lightDegree;
                          pressureReading = (analogRead(pressureAnalogPin1) + analogRead(pressureAnalogPin2) + analogRead(pressureAnalogPin3) + analogRead(pressureAnalogPin4) + analogRead(pressureAnalogPin5))/5; //reading the pad
                    
                          if (pressureReading < noPressure) {
                            Serial.println(" None            ");  //displays none if there if no pressure being applied
                          } 
                          else if (pressureReading < lightPressure) {
                            Serial.println(" Light                     "); //displays light if the pressure is within the range
                            Serial.write(0xFE);  //moves down to the second line
                            Serial.write(0X45);
                            Serial.write(0x40);
                            delay(1);
                            Serial.print (pressureReading);  //displays pressure reading
                          } 
                          else if (pressureReading < mediumPressure) {
                            Serial.println(" Medium                      "); //displays medium if the pressure is within the range
                            Serial.write(0xFE);  //moves down to the second line
                            Serial.write(0X45);
                            Serial.write(0x40);
                            delay(1);
                            Serial.print (pressureReading);  //displays pressure reading
                          } 
                          else{
                            Serial.println(" High                     "); //displays high if the pressure is above the medium range
                            Serial.write(0xFE); //moves down to the second line
                            Serial.write(0X45);
                            Serial.write(0x40);
                            delay(1);
                            Serial.print (pressureReading);  //displays pressure reading
                      }
                      }
                            if (520 < analogReadings <= 8400){
                            
                              newDegree = mediumDegree;
                              pressureReading = (analogRead(pressureAnalogPin1) + analogRead(pressureAnalogPin2) + analogRead(pressureAnalogPin3) + analogRead(pressureAnalogPin4) + analogRead(pressureAnalogPin5))/5; //reading the pad
                        
                              if (pressureReading < noPressure) {
                                Serial.println(" None            ");  //displays none if there if no pressure being applied
                              } 
                              else if (pressureReading < lightPressure) {
                                Serial.println(" Light                     "); //displays light if the pressure is within the range
                                Serial.write(0xFE);  //moves down to the second line
                                Serial.write(0X45);
                                Serial.write(0x40);
                                delay(1);
                                Serial.print (pressureReading);  //displays pressure reading
                              } 
                              else if (pressureReading < mediumPressure) {
                                Serial.println(" Medium                      "); //displays medium if the pressure is within the range
                                Serial.write(0xFE);  //moves down to the second line
                                Serial.write(0X45);
                                Serial.write(0x40);
                                delay(1);
                                Serial.print (pressureReading);  //displays pressure reading
                              } 
                              else{
                                Serial.println(" High                     "); //displays high if the pressure is above the medium range
                                Serial.write(0xFE); //moves down to the second line
                                Serial.write(0X45);
                                Serial.write(0x40);
                                delay(1);
                                Serial.print (pressureReading);  //displays pressure reading
                          }
                          }
                                if (800 < analogReadings <= 1000){
                                
                                  newDegree = highDegree;
                                  pressureReading = (analogRead(pressureAnalogPin1) + analogRead(pressureAnalogPin2) + analogRead(pressureAnalogPin3) + analogRead(pressureAnalogPin4) + analogRead(pressureAnalogPin5))/5; //reading the pad
                            
                                  if (pressureReading < noPressure) {
                                    Serial.println(" None            ");  //displays none if there if no pressure being applied
                                  } 
                                  else if (pressureReading < lightPressure) {
                                    Serial.println(" Light                     "); //displays light if the pressure is within the range
                                    Serial.write(0xFE);  //moves down to the second line
                                    Serial.write(0X45);
                                    Serial.write(0x40);
                                    delay(1);
                                    Serial.print (pressureReading);  //displays pressure reading
                                  } 
                                  else if (pressureReading < mediumPressure) {
                                    Serial.println(" Medium                      "); //displays medium if the pressure is within the range
                                    Serial.write(0xFE);  //moves down to the second line
                                    Serial.write(0X45);
                                    Serial.write(0x40);
                                    delay(1);
                                    Serial.print (pressureReading);  //displays pressure reading
                                  } 
                                  else{
                                    Serial.println(" High                     "); //displays high if the pressure is above the medium range
                                    Serial.write(0xFE); //moves down to the second line
                                    Serial.write(0X45);
                                    Serial.write(0x40);
                                    delay(1);
                                    Serial.print (pressureReading);  //displays pressure reading
                              }
                              }
                                  delay(1000);
                                  Serial.write(0xFE);
                                  Serial.write(0x51);
                            
                                      if(abs(newDegree-oldDegrees) > GRIPPER_MINIMUM_STEP)
                                      {
                                          //sets the servo angle
                                          Fingers.write(newDegree);
                                          Thumb.write(newDegree); 
                                      }
            
                                    oldTime = millis();
                                    oldDegrees = newDegree;
              
            }


 void pin_ISR(){
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH){
    buttonCount++;
  }
 }
