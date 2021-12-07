#include "secrets.h"
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 4 
#define BUTTON 34
#define BACKLIGHT_DELAY 10 //seconds of light after button press

const int backlight_delay = 10000; //Seconds the lcd should be on after releasing button

// Create the lcd object address 0x3F and 16 columns x 2 rows 
LiquidCrystal_I2C lcd (0x27, 16,2);  //

// Define NTP Client to get time
//WiFiUDP ntpUDP;
//NTPClient timeClient(ntpUDP);

OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);
long last_button_press = 0;
long last_temp=0;
 
void  setup () {
  lcdSetup();
  // Initialize Serial Monitor
  Serial.begin(115200);
  //Connect to wifi
  wifiSetup();
  // Initialize a NTPClient to get time
  //timeClient.begin();
  //timeClient.setTimeOffset(3600);
  pinMode(BUTTON,INPUT);
}
 
void  loop () {
  if(last_temp+30000 < millis()){ //Check temp every 30s
    float temp = getTemp();
    lcd.clear();
    lcd.print(temp);
    lcd.print(" C");
    last_temp = millis();
  }
  //String timeStamp = getTime();
  //lcd.setCursor (0, 1); 
  //lcd.print (timeStamp);

  if(digitalRead(BUTTON) == HIGH){
    last_button_press = millis();
  }
  if(last_button_press + backlight_delay > millis()) {
    lcd.backlight();
  } else {
    lcd.noBacklight();
  }
}

float getTemp()
{
  Serial.print("Requesting temperatures... "); 
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
  Serial.println(temp); 
  return temp;
}

void lcdSetup(){
  // Initialize the LCD connected 
  lcd.init();
  // Turn on the backlight on LCD. 
  lcd.backlight();
  lcd.setCursor (0, 0);
  lcd.print("Connecting...");
}

void wifiSetup(){
  int i = 0;
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  int st = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (st != WL_CONNECTED && i < 10 ) {
    delay(10000);
    int t_st = WiFi.status();
    if(t_st != st){
      i = 0;
    }
    st = t_st;
    Serial.print(i);
    Serial.print(" - ");
    Serial.println(st);
    i++;
  }
  if (i>= 10) {
    Serial.print("No Wifi. Restarting.");
    lcd.setCursor(0, 0);
    lcd.print("No Wifi");
    lcd.setCursor(0, 1);
    lcd.print("Restarting");
    delay(2000);
    ESP.restart();
  }
  //Connected to wifi
  lcd.setCursor(0, 0);
  lcd.print("Connected       ");
  Serial.println("");
  Serial.println("WiFi connected.");
  delay(1000);

  //Print ip
  IPAddress ip = WiFi.localIP();
  lcd.setCursor(0, 0);
  lcd.print(ip);
  Serial.println("IP address: ");
  Serial.println(ip);
  delay(1000);
}


//String getTime(){
//  Serial.print("Requesting time... ");
//  while(!timeClient.update()) {
//    Serial.print("updating time... ");
//    timeClient.forceUpdate();
//  }
//  // The formattedDate comes with the following format:
//  // 2018-05-28T16:00:13Z
//  // We need to extract date and time
//  String formatedTime  = timeClient.getFormattedTime();
//  Serial.println(timeClient.getFormattedDate());
//  return formatedTime;
//}
