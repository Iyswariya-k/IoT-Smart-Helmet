#define BLYNK_TEMPLATE_ID "TMPL3gGGWL17Q"
#define BLYNK_TEMPLATE_NAME "project"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <math.h>
#include <DHT.h>
#include <TinyGPS++.h>
#include "MAX30100_PulseOximeter.h"

// ---------------- WIFI ----------------
char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

// ---------------- DHT11 ----------------
#define DHTPIN 27
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ---------------- ADXL345 ----------------
#define ADXL345_ADDR 0x53
#define SDA_PIN 21
#define SCL_PIN 22

#define FALL_THRESHOLD_LOW 0.5
#define FALL_THRESHOLD_HIGH 2.5
#define FALL_DELAY 1000

// ---------------- MQ SENSORS ----------------
#define MQ2_PIN 34
#define MQ135_PIN 35

// ---------------- GPS ----------------
TinyGPSPlus gps;
HardwareSerial GPSserial(2);

// ---------------- MAX30100 ----------------
PulseOximeter pox;

// ---------------- TIMER ----------------
BlynkTimer timer;

// ---------------- VARIABLES ----------------
bool fallDetected = false;
unsigned long lastFallTime = 0;

float temperature = 0;
float humidity = 0;
unsigned long lastDHTRead = 0;

float latitude = 0;
float longitude = 0;

// ---------------- ADXL345 ----------------
void writeRegister(byte reg, byte value) {
  Wire.beginTransmission(ADXL345_ADDR);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

void readADXL345(int16_t &x, int16_t &y, int16_t &z) {
  Wire.beginTransmission(ADXL345_ADDR);
  Wire.write(0x32);
  Wire.endTransmission(false);

  Wire.requestFrom(ADXL345_ADDR, 6);

  if (Wire.available() >= 6) {
    uint8_t xl = Wire.read();
    uint8_t xh = Wire.read();
    uint8_t yl = Wire.read();
    uint8_t yh = Wire.read();
    uint8_t zl = Wire.read();
    uint8_t zh = Wire.read();

    x = (xh << 8) | xl;
    y = (yh << 8) | yl;
    z = (zh << 8) | zl;
  }
}

// ---------------- FALL DETECTION ----------------
void checkFall() {
  int16_t x, y, z;
  readADXL345(x, y, z);

  float ax = x / 256.0;
  float ay = y / 256.0;
  float az = z / 256.0;

  float am = sqrt(ax * ax + ay * ay + az * az);

  if ((am < FALL_THRESHOLD_LOW || am > FALL_THRESHOLD_HIGH) &&
      (millis() - lastFallTime > FALL_DELAY)) {
    fallDetected = true;
    lastFallTime = millis();

    Serial.println("FALL DETECTED");
    Blynk.virtualWrite(V6, "FALL DETECTED");
    Blynk.logEvent("fall_alert", "Fall Detected!");
  }

  if (fallDetected && (millis() - lastFallTime > 5000)) {
    fallDetected = false;
    Serial.println("NORMAL");
    Blynk.virtualWrite(V6, "NORMAL");
  }
}

// ---------------- DHT ----------------
void readDHT() {
  if (millis() - lastDHTRead > 5000) {
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (!isnan(t) && !isnan(h)) {
      temperature = t;
      humidity = h;
    }

    lastDHTRead = millis();
  }
}

// ---------------- GPS ----------------
void readGPS() {
  while (GPSserial.available()) {
    gps.encode(GPSserial.read());
  }

  if (gps.location.isUpdated()) {
    latitude = gps.location.lat();
    longitude = gps.location.lng();

    Blynk.virtualWrite(V7, latitude);
    Blynk.virtualWrite(V8, longitude);

    String link = "https://maps.google.com/?q=" +
                  String(latitude, 6) + "," +
                  String(longitude, 6);

    Blynk.virtualWrite(V10, link);
  }
}

// ---------------- MQ SENSORS ----------------
void readMQ() {
  int mq2 = analogRead(MQ2_PIN);
  int mq135 = analogRead(MQ135_PIN);

  Blynk.virtualWrite(V2, mq2);
  Blynk.virtualWrite(V3, mq135);

  Serial.print("MQ2: ");
  Serial.print(mq2);
  Serial.print(" | MQ135: ");
  Serial.println(mq135);
}

// ---------------- SEND DHT ----------------
void sendData() {
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);

  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" C | Hum: ");
  Serial.println(humidity);
}

// ---------------- MAX30100 ----------------
void onBeatDetected() {
  Serial.println("Beat detected!");
}

void sendPulseData() {
  float heartRate = pox.getHeartRate();
  float spo2 = pox.getSpO2();

  Blynk.virtualWrite(V5, heartRate);
  Blynk.virtualWrite(V4, spo2);

  Serial.print("HR: ");
  Serial.print(heartRate);
  Serial.print(" bpm | SpO2: ");
  Serial.print(spo2);
  Serial.println(" %");
}

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);

  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(100000);

  dht.begin();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  GPSserial.begin(9600, SERIAL_8N1, 16, 17);

  pinMode(MQ2_PIN, INPUT);
  pinMode(MQ135_PIN, INPUT);

  writeRegister(0x2D, 0x08);
  writeRegister(0x31, 0x08);
  writeRegister(0x2C, 0x0A);

  Blynk.virtualWrite(V6, "NORMAL");

  Serial.print("Initializing MAX30100...");
  if (!pox.begin()) {
    Serial.println("FAILED");
    while (1);
  }
  Serial.println("SUCCESS");

  pox.setIRLedCurrent(MAX30100_LED_CURR_24MA);
  pox.setOnBeatDetectedCallback(onBeatDetected);

  timer.setInterval(1000L, sendData);
  timer.setInterval(200L, checkFall);
  timer.setInterval(1000L, readDHT);
  timer.setInterval(1000L, readGPS);
  timer.setInterval(1000L, readMQ);
  timer.setInterval(1000L, sendPulseData);
}

// ---------------- LOOP ----------------
void loop() {
  pox.update();
  Blynk.run();
  timer.run();
}