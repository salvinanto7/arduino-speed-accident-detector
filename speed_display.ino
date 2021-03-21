#include <TinyGPS++.h>
#include<SoftwareSerial.h>
#include<Wire.h>
#include<Adafruit_SH1106.h>

#define OLED_ADDRESS 0x3C
#define OLED_RESET -1
#define vibrator_sensor 7

void display_speed();
int lat_val , long_val, hour_val , mins_val, sec_val;
Adafruit_SH1106 display(OLED_RESET);
TinyGPSPlus gps;
SoftwareSerial gpssoft(17,16);  //rx=17 ,tx=16
SoftwareSerial gsm900(19,18);  //rx=19  tx=18

void setup(){
  Serial.begin(9600);
  gpssoft.begin(9600);
  gsm900.begin(9600);
  pinMode(vibrator_sensor,INPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  display.begin(OLED_ADDRESS);      // removed a line SH_1106_SWITCHCAPVACC, from begin  : uswed to gnerae 3.3v INTERNALLY. connect the vcc of ole to 3.3v terminal of 3.3v of nano
  display.clearDisplay();
  display.display();
}

void loop(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(27,2);
  display.print("SPEED (KMS)");
  display.display();
  while(gpssoft.available()){
    if(gps.encode(gpssoft.read())){
      display_speed();
    }
    else if(millis() >5000 && gps.charsProcessed()<10){   //checks whether the gps is runnig for 5 seconds and recieved data less than 10 characters
      display.setTextSize(1);
      display.setCursor(0,4);
      display.print("ERROR IN COLLECTING DATA!!");
      Serial.print("ERROR IN COLLECTING DATA!"); //debug purpose
      display.display();
      while(1);
    }
  }
  int vibration=digitalRead(vibrator_sensor); 
  if(vibration==1){
    getlocation();
    gsm900.println("AT+CMGF=1");
    gsm900.println("AT+CMGS=\"+917994537611\"");  //receipent number 1 
    gsm900.print("PLEASE HELP ,  I MET WITH AN ACCIDENT!");
    gsm900.println("my location:");
    gsm900.print("https://www.google.com/maps/@");
    gsm900.print(lat_val);
    gsm900.print(",");
    gsm900.print(long_val);
    gsm900.println(",14z");
    gsm900.println("time:");
    gsm900.print(hour_val);
    gsm900.print(":");
    gsm900.print(mins_val);
    gsm900.print(":");
    gsm900.print(sec_val);
    gsm900.write((char)26);
    digitalWrite(12,HIGH);   //denotes the end of transimission of ms g to first no.
    delay(100);
    // for testing purpose only
    Serial.println(" my location :");
    Serial.print("https://www.google.com/maps/@");
    Serial.print(lat_val);
    Serial.print(",");
    Serial.print(long_val);
    Serial.println("'14z");
    digitalWrite(12,LOW);
    

    gsm900.println("AT+CMGF=1");
    gsm900.println("AT+CMGS=\"+919400832924\"");  //receipent number 2 
    gsm900.print("PLEASE HELP ,  I MET WITH AN ACCIDENT!");
    gsm900.println("my location:");
    gsm900.print("https://www.google.com/maps/@");
    gsm900.print(lat_val);
    gsm900.print(",");
    gsm900.print(long_val);
    gsm900.println(",14z");
    gsm900.println("time:");
    gsm900.print(hour_val);
    gsm900.print(":");
    gsm900.print(mins_val);
    gsm900.print(":");
    gsm900.println(sec_val);
    gsm900.write((char)26);
    digitalWrite(13,HIGH);    //denotes the end of transmission of msg to 2nd no.
    delay(100);
    digitalWrite(13,LOW);
  }
}

void display_speed(){
  if(gps.speed.isValid()){
    display.setTextSize(1);
    display.setCursor(0,6);
    display.print(gps.speed.kmph());
    Serial.print(gps.speed.kmph());  //debug purpose
    display.display();
  }
  else{
    display.setTextSize(1);
    display.setCursor(35,40);
    display.print("NO DATA!!");
    display.display();
    Serial.print("NO DATA!!");
  }
}

int getlocation(){
  lat_val = gps.location.lat(); 
  long_val= gps.location.lng();
  if (gps.time.isValid()) {
        hour_val = gps.time.hour();
        mins_val = gps.time.minute();
        sec_val  = gps.time.second();
      }
  return lat_val , long_val ,hour_val , mins_val, sec_val;
}