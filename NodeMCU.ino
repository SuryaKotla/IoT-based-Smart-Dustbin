#define BLYNK_PRINT Serial
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

SoftwareSerial s(12,14);           // Pins D6 (Rx) & D5 (Tx) for serial cmmunication with Arduino uno
int data;
char auth[] = "P0t6XPmIUKahdiiEY6PtRP8NUxUFAEnu";     // get Auth Token of the Blynk server.

char ssid[] = "JioFi";   // Your WiFi credentials.
char pass[] = "Surya@170121044";      // Set password to "" for open networks.

BlynkTimer timer;

void sendData()
{
  s.write(100);
  if (s.available()>0)
  {
    data=s.read();
  }
  Serial.print("Dustbin Status : ");
  Serial.print(data);
  Serial.print("%\n");
  delay(1000);

  Blynk.virtualWrite(V6, data);   // send Dustbin status to V6
}

void setup(){
  s.begin(115200);
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);

  timer.setInterval(500L, sendData);  // function will be called every second
}

void loop(){
  Blynk.run();
  timer.run();     // Initiates BlynkTimer
}
