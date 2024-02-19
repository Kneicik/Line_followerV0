#include <QTRSensors.h>

// This example is designed for use with six analog QTR sensors. These
// reflectance sensors should be connected to analog pins A0 to A5. The
// sensors' emitter control pin (CTRL or LEDON) can optionally be connected to
// digital pin 2, or you can leave it disconnected and remove the call to
// setEmitterPin().
//
// The main loop of the example reads the raw sensor values (uncalibrated). You
// can test this by taping a piece of 3/4" black electrical tape to a piece of
// white paper and sliding the sensor across it. It prints the sensor values to
// the serial monitor as numbers from 0 (maximum reflectance) to 1023 (minimum
// reflectance).

QTRSensors qtr;

const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

void setup()
{
  pinMode(4, OUTPUT);
  pinMode(23, INPUT);
  pinMode(22, INPUT);
  pinMode(21, INPUT);
  pinMode(19, INPUT);
  pinMode(18, INPUT);
  pinMode(5, INPUT);
  pinMode(17, INPUT);
  pinMode(16, INPUT);
  // configure the sensors
  qtr.setTypeAnalog();
  // qtr.setSensorPins((const uint8_t[]){23, 22, 21, 19, 18, 5, 17, 16}, SensorCount);
  qtr.setSensorPins((const uint8_t[]){36, 39, 34, 35, 32, 33, 27, 13}, SensorCount);
  qtr.setEmitterPin(4);

  Serial.begin(9600);
}


void loop()
{
  // read raw sensor values
  qtr.read(sensorValues);

  // print the sensor values as numbers from 0 to 1023, where 0 means maximum
  // reflectance and 1023 means minimum reflectance
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  Serial.println();

  delay(250);
}
