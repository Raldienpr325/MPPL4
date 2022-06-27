#define BLYNK_PRINT SwSerial
#include <SoftwareSerial.h>
SoftwareSerial SwSerial(2, 3); // RX, TX
#include <BlynkSimpleStream.h>
#include <DHT.h>
#define BLYNK_TEMPLATE_ID "TMPLunVDF9KE"
#define BLYNK_DEVICE_NAME "UTS piranti C"

char auth[] = "6gedXzg-Im3escEuM6iKQb_eDTAbNBp3";

char *status[] = {"CLEAR","NOT CLEAR"};

#define DHTPIN 13          // What digital pin we're connected to

#define DHTTYPE DHT11

#define kpa2atm 0.00986923267

DHT dht(DHTPIN, DHTTYPE);

WidgetLED ledgreen(V3);
WidgetLED ledred(V4);


BlynkTimer timer;

int pressurePin = 1;
int val;
float pkPa; 
float pAtm; 

int red = 10;
int green = 11;


unsigned long time;

void sendSensor()
{
  val = analogRead(pressurePin);
  pkPa = ((float)val / (float)1023 + 0.095) / 0.009;
  pAtm = kpa2atm * pkPa;

  int h = dht.readHumidity();
  int t = dht.readTemperature();
  int tempK = t + 273;

  if (isnan(h) || isnan(t) || isnan(pkPa)) {
    Serial.println("Failed to read from DHT sensor!");
  }
  else if (tempK >= 294 && tempK <= 308 && h >= 75 && h <= 100 && pkPa >= 100 && pkPa <= 102) {
    digitalWrite(green, HIGH);
    digitalWrite(red, LOW);
    ledgreen.on();
    ledred.off();
    Blynk.virtualWrite(V5, "CLEAR");
  }
  else {
    digitalWrite(green, LOW);
    digitalWrite(red, HIGH);
    ledgreen.off();
    ledred.on();
    Blynk.virtualWrite(V5, "NOT CLEAR");
  }

  Blynk.virtualWrite(V0, tempK);
  Blynk.virtualWrite(V1, pkPa);
  Blynk.virtualWrite(V2, h);
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  SwSerial.begin(9600);
  Blynk.begin(Serial, auth);
  dht.begin();
  timer.setInterval(1000L, sendSensor);
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
}

void loop()
{
  Blynk.run();
  timer.run();
}
