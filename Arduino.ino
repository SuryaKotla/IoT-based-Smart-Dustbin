#include <LiquidCrystal.h> // includes the LiquidCrystal Library
#include <SoftwareSerial.h>

const int IRin = 10;  // IR sensor OUT value
const int IRout = 11;  // DC Motor for opening Dustbin lid
const int trigPin = 9; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 8; // Echo Pin of Ultrasonic Sensor
const int full = 2;
SoftwareSerial s(12,13); // Pins 12 (Rx) & 13 (Tx) for serial cmmunication with NodeMCU ESP8266
LiquidCrystal lcd(1, 3, 4, 5, 6, 7); // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)

long duration; // variable for the duration of sound wave travel
float distance; // variable for the distance measurement
int DBstatus; // variable for status of Dustbin in percentage

void setup() {
  pinMode(IRout,OUTPUT);  // Sets IRout pin as an OUTPUT
  pinMode(IRin,INPUT);    // Sets the IRin Pin as an OUTPUT
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  s.begin(115200);  // Serial Communication with NodeMCU is starting with 115200 of baudrate speed
  Serial.begin(9600);// Serial Communication is starting with 9600 of baudrate speed
  lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display
}

void loop() {
  
  /* US sensor code */
  digitalWrite(trigPin, LOW);   // Clears the trigPin condition
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  duration = pulseIn(echoPin, HIGH);   // Reads the echoPin, returns the sound wave travel time in microseconds
  
  distance = (duration * 0.034)/ 2;      // Calculating the distance
                                       // Speed of sound wave divided by 2 (go and back)

 
  if(distance<=30){                 //size of Dustbin set to 30cm
      DBstatus=((distance/30)*100);
      DBstatus=100-DBstatus;
    }
  else{
      DBstatus=0;
    }

  Serial.print("Distance of garbage from lid : ");
  Serial.print(distance);
  Serial.print("\n");
  Serial.print("Dustbin Status : ");
  Serial.print(DBstatus);         // Displays the DBstatus on the Serial Monitor
  Serial.print("% \n");
  
  lcd.setCursor(0,0);     // Prints string "Dustbin ID" on the LCD
  lcd.print("Bin ID: DB001"); // display ID in first line
  
  if(DBstatus>90){
      lcd.setCursor(0,1);     // Sets the location at which subsequent text written to the LCD will be displayed
      lcd.print("Dustbin Full!");  // Prints string "Dustbin Full!" on the LCD
      delay(500);
    }
    else{
      IRsensor();
      
      lcd.setCursor(0,1);     // Sets the location at which subsequent text written to the LCD will be displayed
      lcd.print("Status: ");  // Prints string "Status" on the LCD
      lcd.print(DBstatus);    // Prints the Status percentage value
      lcd.print("%");
      delay(500);
    }

  if(DBstatus>90){
    digitalWrite(full,HIGH);
  }
  else{
    digitalWrite(full,LOW);
  }
  
  if(s.available()>0)
    {
     s.write(DBstatus);
     delay(500);
    }
}

void IRsensor(){
      /* IR Sensor code */
    if (digitalRead(IRin)== LOW)   // read from sensor
    {
      digitalWrite(IRout,HIGH);    // control motor
      delay(10);
    }
    else 
    {
      digitalWrite(IRout,LOW);
      delay(10); 
  }
}
