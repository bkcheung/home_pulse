/*
 * Project home_pulse
 * Description: Sensor array for home
 * Author: Bonnie Cheung
 * Date: Dec 15 2021
 */

#include "Air_Quality_Sensor.h"
#include <math.h>

#define AQS_PIN A2
#define DUST_SENSOR_PIN D4
#define SENSOR_READING_INTERVAL 30000

AirQualitySensor aqSensor(AQS_PIN);

unsigned long lastInterval;
unsigned long lowpulseoccupancy = 0;
unsigned long last_lpo = 0;
unsigned long duration;

float ratio = 0;
float concentration = 0;

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  Serial.begin(9600);
    
  pinMode(DUST_SENSOR_PIN, INPUT);  
  lastInterval = millis();
  
  if (aqSensor.init())
  {
   Serial.println("Air Quality Sensor ready.");
  }
  else
  {
   Serial.println("Air Quality Sensor ERROR!");
  }  


}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
    duration = pulseIn(DUST_SENSOR_PIN, LOW);
    lowpulseoccupancy = lowpulseoccupancy + duration;

    String quality = getAirQuality();
    Serial.printlnf("Air Quality: %s", quality.c_str());
    
    if ((millis() - lastInterval) > SENSOR_READING_INTERVAL)
    {
       getDustSensorReadings();
    
       lowpulseoccupancy = 0;
       lastInterval = millis();
      }

}

//Dust sensor function
void getDustSensorReadings() 
{
    if (lowpulseoccupancy == 0)
    {
     lowpulseoccupancy = last_lpo;
    }
    else
    {
     last_lpo = lowpulseoccupancy;
}
    ratio = lowpulseoccupancy / (SENSOR_READING_INTERVAL * 10.0);
    concentration = 1.1 * pow(ratio, 3) - 3.8 * pow(ratio, 2) + 520 * ratio + 0.62;

    Serial.printlnf("LPO: %lu", lowpulseoccupancy);
    Serial.printlnf("Ratio: %f%%", ratio);
    Serial.printlnf("Concentrationzzz: %f pcs/L", concentration);
}    

//Air quality function
String getAirQuality()
{
 int quality = aqSensor.slope();
 String qual = "None";

 if (quality == AirQualitySensor::FORCE_SIGNAL)
 {
   qual = "Danger";
 }
 else if (quality == AirQualitySensor::HIGH_POLLUTION)
 {
   qual = "High Pollution";
 }
 else if (quality == AirQualitySensor::LOW_POLLUTION)
 {
   qual = "Low Pollution";
 }
 else if (quality == AirQualitySensor::FRESH_AIR)
 {
   qual = "Fresh Air";
 }
 return qual;
}