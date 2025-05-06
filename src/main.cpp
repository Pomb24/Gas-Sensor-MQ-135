#include <Arduino.h>                              // Include the Arduino library

#define DUST_LED_PIN PA8 
#define DUST_ANALOG_PIN PA7

const int samplingTime = 280; 
const float sensitivity = 0.005; 
const float vCleanAir = 0.9;  
unsigned long lastDataTime = 0;
const int samplingInterval = 1000;

float readDustDensity() {
  float voMeasured = 0;
  float calcVoltage = 0;
  float dustDensity = 0;

  digitalWrite(DUST_LED_PIN, LOW);
  delayMicroseconds(samplingTime);
  voMeasured = analogRead(DUST_ANALOG_PIN);
digitalWrite(DUST_LED_PIN, HIGH);
  calcVoltage = voMeasured * (5.0 / 1024.0);
  dustDensity = (calcVoltage - vCleanAir) / sensitivity;
  if (dustDensity < 0) dustDensity = 0;
  return dustDensity;
}
void setup(){
  pinMode(DUST_LED_PIN, OUTPUT);
  digitalWrite(DUST_LED_PIN, HIGH);
  pinMode(DUST_ANALOG_PIN, INPUT);

  Serial.begin(115200);
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastDataTime >= samplingInterval) {
    lastDataTime = currentTime;

        float dustDensity = readDustDensity();
        Serial.print(dustDensity, 2);
        Serial.println();
  }
}