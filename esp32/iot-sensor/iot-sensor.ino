#include "secrets.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 4 
#define BUTTON 34
#define BACKLIGHT_DELAY 10 //seconds of light after button press
#define AWS_IOT_PUBLISH_TOPIC   "tulsi/status"

const int backlight_delay = 10000; //MilliSeconds the lcd should be on after releasing button
const int mqtt_delay = 300000; //MilliSeconds between mqtt status messages

// Create the lcd object address 0x3F and 16 columns x 2 rows 
LiquidCrystal_I2C lcd (0x27, 16,2);  //

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
long epoch_time=0;

OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);
long last_button_press=0;
long last_temp=0;
long last_mqtt_sent=0;
float temp = 0;

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(256);

 
void  setup () {
  lcdSetup();
  // Initialize Serial Monitor
  Serial.begin(115200);
  //Connect to wifi
  wifiSetup();
  connectAWS();
  // Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.setUpdateInterval(300000);
  pinMode(BUTTON,INPUT);
}
 
void  loop () {
  if(last_temp+30000 < millis()){ //Check temp every 30s
    temp = getTemp();
    lcd.clear();
    lcd.print(temp);
    lcd.print(" C");
    last_temp = millis();
  }
  epoch_time = getTime();
  lcd.setCursor (0, 1); 
  lcd.print (epoch_time);

  if(digitalRead(BUTTON) == HIGH){
    last_button_press = millis();
  }
  if(last_button_press + backlight_delay > millis()) {
    lcd.backlight();
  } else {
    lcd.noBacklight();
  }
  if(last_mqtt_sent + mqtt_delay <  millis() || last_mqtt_sent==0){
    publishMessage();
    last_mqtt_sent = millis();
  }
}

float getTemp()
{
  Serial.print("Requesting temperatures... "); 
  sensors.requestTemperatures();
  float temp1 = sensors.getTempCByIndex(0);
  Serial.println(temp1); 
  return temp1;
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

void connectAWS()
{
  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);
  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.begin(AWS_IOT_ENDPOINT, 8883, net);

  Serial.print("Connecting to AWS IOT");

  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(1000);
  }
  if(!client.connected()){
    Serial.println("AWS IoT Timeout!");
    return;
  }
  Serial.println("AWS IoT Connected!");
}

void publishMessage()
{
  Serial.println("Trying to send iot message");
  StaticJsonDocument<200> doc;
  doc["t"] = temp;
  doc["s"] = 0;
  doc["c"] = "studio-esp32";
  doc["ts"] = epoch_time;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
  if(!client.connected()){
    connectAWS();
  }
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
  bool result = client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
  Serial.print("Message: ");
  Serial.println(jsonBuffer);
  Serial.print("Status: ");
  Serial.println(result);

}

long getTime(){
  bool updating=false;
  if(!timeClient.update()){
    updating=true;
    Serial.print("Updating time");
  }
  while(!timeClient.update()) {
    timeClient.forceUpdate();
    Serial.print(".");
    delay(1000);
  }
  if(updating){
    Serial.println(" Done!");
  }
  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
  long epoch  = timeClient.getEpochTime();
  return epoch;
}
