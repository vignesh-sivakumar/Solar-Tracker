#include <TinyGPS++.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(9, 10);

/*
   This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 0, TXPin =1;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  ss.begin(GPSBaud);
  ss.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);
//  Serial.println(F("DeviceExample.ino"));
//  Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
//  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
//  Serial.println(F("by Mikal Hart"));
//  Serial.println();
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      if(ss.available()>0)
      {
      
      }
      displayInfo();
    

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

void displayInfo()
{
  
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    double lat1=gps.location.lat() ;
    Serial.print(F(","));
    Serial.println(gps.location.lng(), 6);
    double long1=gps.location.lng();
    delay(100);
     if (mySerial.available()>0)
     {
       Serial.write(mySerial.read());
     }
  }
  else
  {
    Serial.print(F("INVALID"));
  }
  Serial.println();
}
