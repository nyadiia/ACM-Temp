/*
 * Project ACM-Temp
 * Description: This is a project to monitor the temperature of the ACM server closet
 * Author: Nadia Potteiger
 * Date: 2023-04-17
 */

#include "PietteTech_DHT.h"
#include "VL53L1X.h"

#define SENSOR_PIN D2
#define HEARTBEAT_LED D7
#define DHT_TYPE DHT11
#define POLL_RATE 30000 // read every 30 secs
// #define LED_RATE 2000   // blink at 0.5 hz
#define CRITICAL_TEMP 35

double temp_c, humidity;
unsigned long int poll_time, led_time;
bool LED_state = FALSE;

PietteTech_DHT DHT(SENSOR_PIN, DHT_TYPE);


// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  // Put initialization like pinMode and begin functions here.
  pinMode(HEARTBEAT_LED, OUTPUT);
  Serial.begin(9600);
  DHT.begin();

  // // cloud variables for the webpage
  // Particle.variable("cV_temp", temp_c);
  // Particle.variable("cV_humidity", humidity);

  // non-blocking method but doesn't read the sensor too much
  poll_time = millis() + POLL_RATE;
  // led_time = millis() + LED_RATE;
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  unsigned long int current_time = millis();

  // if the current time is greater than the poll delay, run the code
  if (current_time > poll_time) {
    int result;
    acquire_data(result);
    // if we're actually able to read the sensor
    if (result == DHTLIB_OK) {
      temp_c = DHT.getCelsius();
      humidity = DHT.getHumidity();
      Serial.printf("Humidity: %.0f%%\n", DHT.getHumidity());
      Serial.printf("Temp: %.0fC\n", DHT.getCelsius());

      // publish to the cloud
      Particle.publish("temp", String(temp_c, 0), PRIVATE);

      if (temp_c >= CRITICAL_TEMP) {
        Particle.publish("n_cloud_critical_temp", String(temp_c, 0), PRIVATE);
      }
    }
    // toggle LED
    LED_state = !LED_state;
    digitalWrite(HEARTBEAT_LED, LED_state);

    // add poll rate (ms) to poll time to reset the whole loop
    poll_time += POLL_RATE;
  }

}

// this code is from the example and is helpful for error checking
void acquire_data(int& result) {
  result = DHT.acquireAndWait(2000);

  switch (result) {
  case DHTLIB_OK:
    break;
  case DHTLIB_ERROR_CHECKSUM:
    Serial.println("Error\n\r\tChecksum error");
    break;
  case DHTLIB_ERROR_ISR_TIMEOUT:
    Serial.println("Error\n\r\tISR time out error");
    break;
  case DHTLIB_ERROR_RESPONSE_TIMEOUT:
    Serial.println("Error\n\r\tResponse time out error");
    break;
  case DHTLIB_ERROR_DATA_TIMEOUT:
    Serial.println("Error\n\r\tData time out error");
    break;
  case DHTLIB_ERROR_ACQUIRING:
    Serial.println("Error\n\r\tAcquiring");
    break;
  case DHTLIB_ERROR_DELTA:
    Serial.println("Error\n\r\tDelta time to small");
    break;
  case DHTLIB_ERROR_NOTSTARTED:
    Serial.println("Error\n\r\tNot started");
    break;
  default:
    Serial.println("Unknown error");
    break;
  }
}