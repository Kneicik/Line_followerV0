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
#define Kp 0.05 // experiment to determine this, start by something small that just makes your bot follow the line at a slow speed
#define Kd 3 // experiment to determine this, slowly increase the speeds and adjust this value. ( Note: Kp < Kd) 
#define rightMaxSpeed 100 // max speed of the robot
#define leftMaxSpeed 100 // max speed of the robot
#define rightBaseSpeed 60 // this is the speed at which the motors should spin when the robot is perfectly on the line
#define leftBaseSpeed 60  // this is the speed at which the motors should spin when the robot is perfectly on the line
#define NUM_SENSORS  8     // number of sensors used
#define EMITTER_PIN   4     // emitter is controlled by digital pin 2
#define LEFT_MOTOR_FORWARD 14
#define LEFT_MOTOR_BACKWARD 12
#define RIGHT_MOTOR_FORWARD 25
#define RIGHT_MOTOR_BACKWARD 26

QTRSensors qtr;

uint16_t sensorValues[NUM_SENSORS];

void setup()
{
  pinMode(12, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(25, OUTPUT);
  // configure the sensors
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){36, 39, 34, 35, 32, 33, 27, 13}, NUM_SENSORS);
  qtr.setEmitterPin(EMITTER_PIN);

  Serial.begin(9600);

    /* comment this part out for automatic calibration 
  if ( i  < 25 || i >= 75 ) // turn to the left and right to expose the sensors to the brightest and darkest readings that may be encountered
     turn_right();  
   else
     turn_left(); */ 
  qtr.calibrate();   
  delay(20);
  
  delay(10000); // wait for 10s to position the bot before entering the main loop 
  Serial.println("Calibration done!");
    
    // comment out for serial printing
    
    // Serial.begin(9600);
    // for (int i = 0; i < NUM_SENSORS; i++)
    // {
    //   Serial.print(qtr.calibratedMinimumOn[i]);
    //   Serial.print(' ');
    // }
    // Serial.println();

    // for (int i = 0; i < NUM_SENSORS; i++)
    // {
    //   Serial.print(qtr.calibratedMaximumOn[i]);
    //   Serial.print(' ');
    // }
    // Serial.println();
    // Serial.println();
}

int lastError = 0;

void loop()
{
  unsigned int sensors[8];
  int position = qtr.readLineBlack(sensorValues); // get calibrated readings along with the line position, refer to the QTR Sensors Arduino Library for more details on line position.
  int error = position - 3500;

  int motorSpeed = Kp * error + Kd * (error - lastError);
  lastError = error;

  int rightMotorSpeed = rightBaseSpeed + motorSpeed;
  int leftMotorSpeed = leftBaseSpeed - motorSpeed;
  
    if (rightMotorSpeed > rightMaxSpeed ) rightMotorSpeed = rightMaxSpeed; // prevent the motor from going beyond max speed
  if (leftMotorSpeed > leftMaxSpeed ) leftMotorSpeed = leftMaxSpeed; // prevent the motor from going beyond max speed
  if (rightMotorSpeed < 0) rightMotorSpeed = 0; // keep the motor speed positive
  if (leftMotorSpeed < 0) leftMotorSpeed = 0; // keep the motor speed positive
  
   {
  // move forward with appropriate speeds
  Serial.print(leftMotorSpeed);
  Serial.print(" ");
  Serial.print(rightMotorSpeed);
  Serial.print(" ");
  Serial.print(position);
  Serial.println("");
  analogWrite(LEFT_MOTOR_BACKWARD, rightMotorSpeed); //only works if "DRV8835MotorShield motors;" is defined in the beginning
  analogWrite(RIGHT_MOTOR_FORWARD, leftMotorSpeed); //only works if "DRV8835MotorShield motors;" is defined in the beginning

   }
}



  // void loop()
// {
//   // read raw sensor values
//   qtr.read(sensorValues);

//   // print the sensor values as numbers from 0 to 1023, where 0 means maximum
//   // reflectance and 1023 means minimum reflectance
//   for (uint8_t i = 0; i < SensorCount; i++)
//   {
//     Serial.print(sensorValues[i]);
//     Serial.print('\t');
//   }
//   Serial.println();

//   delay(250);
// }
