#include "DHT.h"
#include <Arduino.h>
#include <TheThingsNetwork.h>

#define debugSerial Serial
#define loraSerial Serial1

#define freqPlan TTN_FP_EU868

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

const char *appEui = "70B3D57ED0040954";
const char *appKey = "B7C8C3E89EC84162090AE290272A27A4";

DHT dht;
#define soilSensor A5

void setup()
{
  loraSerial.begin(57600);
  debugSerial.begin(9600);
  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000);

  debugSerial.println("-- STATUS");
  ttn.showStatus();
  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);

  dht.setup(6); // data pin 2
}

void loop()
{
 

   uint8_t humidity = dht.getHumidity();
   uint8_t temperature = dht.getTemperature();
   uint8_t soil_moisture = analogRead(soilSensor);

  uint8_t payload[6];

  payload[0] = highByte(humidity);
  payload[1] = lowByte(humidity);
  payload[2] = highByte(temperature);
  payload[3] = lowByte(temperature);
  payload[4] = highByte(soil_moisture);
  payload[5] = lowByte(soil_moisture);


  debugSerial.print(dht.getStatusString());
  debugSerial.print("\t");
  debugSerial.print(humidity, 1);
  debugSerial.print("\t\t");
  debugSerial.print(temperature, 1);
  debugSerial.print("\t\t");
  debugSerial.println(dht.toFahrenheit(temperature), 1);
  debugSerial.print("\t\t");
  debugSerial.print(soil_moisture);

   ttn.sendBytes(payload, sizeof(payload));
   delay(900000);


}
