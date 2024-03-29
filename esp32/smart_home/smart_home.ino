               
#include <OneWire.h>
#include <DallasTemperature.h>

// Pin donde se conecta el bus 1-Wire
const int pin_temp = 25;
const int D1 = 0;
const int D2 = 4;
const int D3 = 17;
const int D4 = 16;
const int clockPin = 2;
const int latchPin = 32;
const int dataPin = 33;
const long tempInterval = 10000;
const long lcdInterval = 4;
const int warm_pin = 15;
const int tooCold = 18;

 
// Instancia a las clases OneWire y DallasTemperature
OneWire oneWireObjeto(pin_temp);
DallasTemperature sensorDS1820(&oneWireObjeto);
unsigned long previousMillis = 0;  // will store last time temp was updated
unsigned long previousLCDMillis = 0;


int count = 0;
int numbers[4] ;
int cathodePins[] = {D1, D2, D3, D4};
double temp = 0;
unsigned long whenCheck = 0;

int num1 = 0;
int num2 = 0;
int num3 = 0;
int num4 = 0; 

byte table[10] {B00000011, B10011111, B00100101, B00001101, B10011001, B01001001, B01000001, B00011111, B00000001, B00001001};

//byte table[10] {B11111100, B01100000, B00000001, B11110010, B01100110, B10110110, B10111110, B11100000, B11111110, B11110110};

int tabled[10] {3,159,37,13,153,73,65,31,1,9};

void setup() {
  // Iniciamos la comunicación serie
  Serial.begin(115200);
  // Iniciamos el bus 1-Wire
  sensorDS1820.begin(); 
  pinMode(D4, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(warm_pin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  screenOff();
  digitalWrite(warm_pin, LOW);
}

void loop() {
  unsigned long currentMillis = millis();
  updateTemp(currentMillis);
  displayTemp(temp);
  if(temp>tooCold){
    digitalWrite(warm_pin, LOW);
  }else{
    digitalWrite(warm_pin, HIGH);
  }
}

void displayTemp(double temp){
  unsigned long currentMillis = millis();
  if (currentMillis - previousLCDMillis >= lcdInterval) {
    previousLCDMillis = currentMillis;
    Display();
  }
}

void updateTemp(unsigned long currentMillis){
  if (currentMillis - previousMillis >= tempInterval) {
      Serial.println("Fetching temp");
      sensorDS1820.setWaitForConversion(false);  // makes it async
      sensorDS1820.requestTemperatures();
      sensorDS1820.setWaitForConversion(true);
      previousMillis = currentMillis;
      whenCheck=currentMillis;
  }
  if(currentMillis > whenCheck+1000){
    temp = sensorDS1820.getTempCByIndex(0);
    
    if(temp>50 || temp < 3) {
      temp = 33.33; 
    }
    
    Serial.print("Temp: ");
    Serial.println(temp);
    int t = long(temp*100);
    separate(t);
    whenCheck=currentMillis+tempInterval;
  }
}

void separate(long num) {
  num1 = num / 1000;
  numbers[0] = num1;
  int num1_remove = num - (num1 * 1000);
  num2 = num1_remove / 100;
  numbers[1] = num2;
  int num2_remove = num1_remove - (num2 * 100);
  num3 = num2_remove / 10;
  numbers[2] = num3;
  num4 = num2_remove - (num3 * 10);
  numbers[3] = num4;
}

void Display() {
  screenOff(); 
  digitalWrite(latchPin, LOW);
  int wr = tabled[numbers[count]]; 
  if (count==1) {
    wr = wr-1;
  }
  shiftOut(dataPin, clockPin, LSBFIRST, wr); 
  digitalWrite(cathodePins[count], HIGH); 
  digitalWrite(latchPin, HIGH); 
  count++; 
  if (count == 4) { 
    count = 0;
  }
}

void screenOff() { 
  digitalWrite(D4, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D1, LOW);
}
