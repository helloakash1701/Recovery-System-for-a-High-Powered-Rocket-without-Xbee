#include <SoftwareSerial.h>
#include <SPI.h>;
#include <TinyGPS.h>
#include <SD.h>;

#include "Adafruit_BMP085.h";
Adafruit_BMP085 mysensor;
#include <Servo.h>
TinyGPS gps; // create gps object
Servo myservo;   // variable to store the servo position
File mydata;
float temp;
float alt;
float alt1;
float latitude = 29.6765, lon = 80.5634; // create variable for latitudeitude and longitude object


void setup() {
  mysensor.begin();
  Serial.begin(9600); // connect serial
  Serial.println("SD Card Initialize");
  myservo.attach(9);
 

}

void recovery(float x, float y){
  if(x<y){
    for(int i=0;i<5;i++){
      myservo.write(180);
      delay(100);
      myservo.write(90);
      delay(100);
      temp = mysensor.readTemperature();
      Serial.print("The Temperature is: ");
      Serial.println(temp);
      mydata.println(temp);
      alt = mysensor.readAltitude();
      Serial.print("The Altitude is : ");
      Serial.println(alt);

    }

  }
}
void loop() {
  mydata = SD.open('SDCARD.txt' ,FILE_WRITE);

  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  temp = mysensor.readTemperature();
  Serial.print("The Temperature: ");
  Serial.println(temp);
  mydata.println(temp);
  alt = mysensor.readAltitude();
  Serial.print("Altitude: ");
  Serial.println(alt);
  mydata.println(alt);
  delay(100);
  alt1 = mysensor.readAltitude();
  recovery(alt1,alt);



  for (unsigned long start = millis(); millis() - start < 500;)
  {
    while (Serial.available())
    {
      char c = Serial.read();
      //Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;  //newData variable is set to true
    }
  }

  if (newData)      //If newData is true
  {
    float flatitude, flon;
    unsigned long age;
    gps.f_get_position(&flatitude, &flon, &age);
    Serial.print("latitudeitude = ");
    Serial.println(flatitude == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flatitude, 6);   //Parse the NMEA data to extract and display the latitudeitude
    Serial.print("Longitude = ");
    Serial.println(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);  //Parse the NMEA data to extract and display the longitude

  }

  mydata.close();
}
