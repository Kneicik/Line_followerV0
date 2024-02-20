#include <QTRSensors.h>
#include <WiFi.h>
#include <AsyncUDP.h>


#define Kp 0.05 // experiment to determine this, start by something small that just makes your bot follow the line at a slow speed
#define Kd 3 // experiment to determine this, slowly increase the speeds and adjust this value. ( Note: Kp < Kd) 
#define rightMaxSpeed 100 // max speed of the robot
#define leftMaxSpeed 100 // max speed of the robot
#define rightBaseSpeed 60 // this is the speed at which the motors should spin when the robot is perfectly on the line
#define leftBaseSpeed 60  // this is the speed at which the motors should spin when the robot is perfectly on the line
#define NUM_SENSORS  8     // number of sensors used
#define EMITTER_PIN   4     
#define LEFT_MOTOR_FORWARD 14
#define LEFT_MOTOR_BACKWARD 12
#define RIGHT_MOTOR_FORWARD 25
#define RIGHT_MOTOR_BACKWARD 26

const char* ssid = "CBS-2G";
const char* password = "Karolina2137";
const IPAddress staticIP(192, 168, 0, 169);
const IPAddress gateway(192, 168, 0, 1);
const IPAddress subnet(255, 255, 255, 0);

AsyncUDP udp;
QTRSensors qtr;

uint16_t sensorValues[NUM_SENSORS];
int ready = 0;

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
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Łączenie z WiFi...");
  }

  // WiFi.config(staticIP, gateway, subnet);
    if(udp.listen(1234)) {
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());
    }
    udp.onPacket([](AsyncUDPPacket packet) {
        char* tmpStr = (char*) malloc(packet.length() + 1);
        memcpy(tmpStr, packet.data(), packet.length());
        tmpStr[packet.length()] = '\0'; // ensure null termination
        String message = String(tmpStr);
        free(tmpStr);
        Serial.println(message);
            if(message == "Cal"){
                calibrate();
            }
            if(message == "Reset"){
                ESP.restart();
            }
            if(message == "Start"){
                ready = 1;
            }
            if(message == "Stop"){
                ready = 0;
            }
    });
    

    /* comment this part out for automatic calibration 
  if ( i  < 25 || i >= 75 ) // turn to the left and right to expose the sensors to the brightest and darkest readings that may be encountered
     turn_right();  
   else
     turn_left(); */ 
    
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
int rightMotorSpeed = 0;
int leftMotorSpeed = 0;
void loop()
{
  unsigned int sensors[8];
  int position = qtr.readLineBlack(sensorValues); // get calibrated readings along with the line position, refer to the QTR Sensors Arduino Library for more details on line position.
  int error = position - 3500;

  int motorSpeed = Kp * error + Kd * (error - lastError);
  lastError = error;

  rightMotorSpeed = rightBaseSpeed + motorSpeed;
  leftMotorSpeed = leftBaseSpeed - motorSpeed;
  
    if (rightMotorSpeed > rightMaxSpeed ) rightMotorSpeed = rightMaxSpeed; // prevent the motor from going beyond max speed
  if (leftMotorSpeed > leftMaxSpeed ) leftMotorSpeed = leftMaxSpeed; // prevent the motor from going beyond max speed
  if (rightMotorSpeed < 0) rightMotorSpeed = 0; // keep the motor speed positive
  if (leftMotorSpeed < 0) leftMotorSpeed = 0; // keep the motor speed positive
  
   {
  // move forward with appropriate speeds
  // Serial.print(leftMotorSpeed);
  // Serial.print(" ");
  // Serial.print(rightMotorSpeed);
  // Serial.print(" ");
  // Serial.print(position);
  // Serial.println("");
  static unsigned long lastMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - lastMillis >= 500) { 
    lastMillis = currentMillis;
    String pos = "Position: " + String(position); 
    udp.broadcast(pos.c_str()); 
    Serial.println("sent");
  }
  if(ready == 1){
    analogWrite(LEFT_MOTOR_BACKWARD, rightMotorSpeed);
    analogWrite(RIGHT_MOTOR_FORWARD, leftMotorSpeed);
  }else{
    analogWrite(LEFT_MOTOR_BACKWARD, 0);
    analogWrite(RIGHT_MOTOR_FORWARD, 0);
  }

   }
}

void calibrate(){
  qtr.calibrate();   
  
  delay(10000);
  Serial.println("Calibration done!");
  udp.broadcast("C");
  udp.broadcast("C");
  udp.broadcast("C");
}
