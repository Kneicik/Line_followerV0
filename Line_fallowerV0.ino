#include <QTRSensors.h>
#include <WiFi.h>
#include <AsyncUDP.h>

float Kp = 0.5; 
float Ki = 0.0;  
float Kd = 1;
float MaxSpeed = 80; 
float BaseSpeed = 50;
float TurnSpeed = 70;
// float Kp = 0.77; 
// float Ki = 0.0;  
// float Kd = 5.25;
// float MaxSpeed = 75; 
// float BaseSpeed = 140;
// float TurnSpeed = 140;
#define NUM_SENSORS  6    
#define EMITTER_PIN   4     
#define LEFT_MOTOR_FORWARD 12
#define LEFT_MOTOR_BACKWARD 14
#define RIGHT_MOTOR_FORWARD 25
#define RIGHT_MOTOR_BACKWARD 26

const char* ssid = "LF";
const char* password = "Karolina2137";

AsyncUDP udp;
QTRSensors qtr;

uint16_t sensorValues[NUM_SENSORS];
int ready = 0;

void setup()
{
  pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);
  analogReadResolution(12);

  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){39, 34, 35, 32, 33, 36}, NUM_SENSORS);
  qtr.setEmitterPin(EMITTER_PIN);

  Serial.begin(9600);

// if you want esp32 to be the access point

  WiFi.softAP(ssid, password);

  // if you want to connect to existing network

  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(1000);
  //   Serial.println("Łączenie z WiFi...");
  // }

  if(udp.listen(1234)) {
      Serial.print("UDP Listening on IP: ");
      Serial.println(WiFi.localIP());
  }

  udp.onPacket([](AsyncUDPPacket packet) {
      char* tmpStr = (char*) malloc(packet.length() + 1);
      memcpy(tmpStr, packet.data(), packet.length());
      tmpStr[packet.length()] = '\0'; // ensure null termination
      String message = String(tmpStr);
      String firstTwoLetters = message.substring(0, 2);
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
          if(message == "Sensors"){
              request_sensorsRaw();
          }
          if(message == "Params"){
              request_params();
          }
          if(firstTwoLetters == "Kp"){
            String numericalPart = message.substring(4);
            float numericalValue = numericalPart.toFloat();
            Kp = numericalValue;
          }
          if(firstTwoLetters == "Ki"){
            String numericalPart = message.substring(4);
            float numericalValue = numericalPart.toFloat();
            Ki = numericalValue;
          }
          if(firstTwoLetters == "Kd"){
            String numericalPart = message.substring(4);
            float numericalValue = numericalPart.toFloat();
            Kd = numericalValue;
          }
          if(firstTwoLetters == "Ma"){
            String numericalPart = message.substring(4);
            float numericalValue = numericalPart.toFloat();
            MaxSpeed = numericalValue;
          }
          if(firstTwoLetters == "Ba"){
            String numericalPart = message.substring(4);
            float numericalValue = numericalPart.toFloat();
            BaseSpeed = numericalValue;
          }
          if(firstTwoLetters == "Tu"){
            String numericalPart = message.substring(4);
            float numericalValue = numericalPart.toFloat();
            TurnSpeed = numericalValue;
          }
    });
    
}

int lastError = 0;
int rightMotorSpeed = 0;
int leftMotorSpeed = 0;
int last_sighted = 0;
int lost;
int lost_threshold = 450;
int lost_sensors;

void loop()
{
  int position = qtr.readLineBlack(sensorValues); // get calibrated readings along with the line position, refer to the QTR Sensors Arduino Library for more details on line position.
  if(position <= 1800 || sensorValues[0] >=800){
    if(lost == 0){
      last_sighted = 1;
    }
  } 
  else if(position >= 3000 || sensorValues[5] >= 800){
    if(lost == 0){
      last_sighted = 2;
    }
  }
  lost_sensors = 0;
  lost = 0;
  for(int i = 0; i < NUM_SENSORS; i++){
    if(sensorValues[i] <= lost_threshold){
      lost_sensors += 1;
    }
  }
  if(lost_sensors >= (NUM_SENSORS)){
    lost = 1;
  }

  int error = position - 2500;

  int motorSpeed = Kp * error + Kd * (error - lastError);
  lastError = error;

  rightMotorSpeed = BaseSpeed + motorSpeed;
  leftMotorSpeed = BaseSpeed - motorSpeed;
  
  if (rightMotorSpeed > MaxSpeed ) rightMotorSpeed = MaxSpeed; 
  if (leftMotorSpeed > MaxSpeed ) leftMotorSpeed = MaxSpeed; 
  if (rightMotorSpeed < 0) rightMotorSpeed = 0; 
  if (leftMotorSpeed < 0) leftMotorSpeed = 0; 
  
  static unsigned long lastMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - lastMillis >= 200) { 
    lastMillis = currentMillis;
    String pos = "Position: " + String(position); 
    udp.broadcast(pos.c_str()); 
    udp.broadcast("!"); 
    request_sensorsRaw();
  }
  if(ready == 1){
    if(lost == 1 && last_sighted == 1){
      analogWrite(RIGHT_MOTOR_FORWARD, TurnSpeed); 
      analogWrite(LEFT_MOTOR_BACKWARD, TurnSpeed); 
      delay(10);
      analogWrite(LEFT_MOTOR_FORWARD, 0);
      analogWrite(RIGHT_MOTOR_BACKWARD, 0);
    }
    else if(lost == 1 && last_sighted == 2){
      analogWrite(RIGHT_MOTOR_FORWARD, 0); 
      analogWrite(LEFT_MOTOR_BACKWARD, 0); 
      delay(10);
      analogWrite(LEFT_MOTOR_FORWARD, TurnSpeed);
      analogWrite(RIGHT_MOTOR_BACKWARD, TurnSpeed);
    }
    else{
      analogWrite(RIGHT_MOTOR_BACKWARD, 0);
      analogWrite(LEFT_MOTOR_BACKWARD, 0);
      analogWrite(LEFT_MOTOR_FORWARD, rightMotorSpeed);
      analogWrite(RIGHT_MOTOR_FORWARD, leftMotorSpeed);
    }
  }
  else{
    analogWrite(LEFT_MOTOR_FORWARD, 0);
    analogWrite(RIGHT_MOTOR_FORWARD, 0);
    analogWrite(LEFT_MOTOR_BACKWARD, 0);
    analogWrite(RIGHT_MOTOR_BACKWARD, 0);
  }
}

void calibrate(){
  qtr.resetCalibration();
  for (uint16_t i = 0; i < 400; i++){
    qtr.calibrate();
  }
  // for (uint8_t i = 0; i < NUM_SENSORS; i++){
    // Serial.print(qtr.calibrationOn.minimum[i]);
    // udp.broadcast(qtr.calibrationOn.minimum[i]);
    // Serial.print(' ');
  // }
  udp.broadcast("C");
  udp.broadcast("C");
  udp.broadcast("C");
}

void request_sensorsRaw(){
  String sensors = "Sensor ";
  for (uint8_t i = 0; i < NUM_SENSORS; i++)
  {
    sensors += String(sensorValues[i]) + " ";
    // Serial.print(sensorValues[i]);
    // Serial.print(" ");
    // Serial.print('\t');
  }
  sensors += "Last: " + String(last_sighted) + " Lost: " + String(lost);
  // Serial.println("");
  udp.broadcast(sensors.c_str());
}

void request_params(){
  String params = "Kp: " + String(Kp) + " Ki: " + String(Ki) + " Kd: " + String(Kd) + " Max: " + String(MaxSpeed) + " Base: " + String(BaseSpeed) + " Turn: " + String(TurnSpeed);
  // Serial.println(params);
  udp.broadcast(params.c_str());
}
