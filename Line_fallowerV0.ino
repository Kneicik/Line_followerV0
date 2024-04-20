#include <QTRSensors.h>
#include <WiFi.h>
#include <AsyncUDP.h>
#include <Adafruit_NeoPixel.h>

float Kp = 0.5; 
float Ki = 0.0;  
float Kd = 1;
float MaxSpeed = 80; 
float BaseSpeed = 50;
float TurnSpeed = 70;
float lost_threshold = 450;
// float Kp = 0.77; 
// float Ki = 0.0;  
// float Kd = 5.25;
// float MaxSpeed = 75; 
// float BaseSpeed = 140;
// float TurnSpeed = 140;
#define NUM_SENSORS  8    
#define EMITTER_PIN   16     
#define LEFT_MOTOR_FORWARD 11
#define LEFT_MOTOR_BACKWARD 12
#define RIGHT_MOTOR_FORWARD 13
#define RIGHT_MOTOR_BACKWARD 14
#define NEOPIXEL_PIN 48
#define NUM_PIXELS 1

const char* ssid = "LF";
const char* password = "Karolina2137";

AsyncUDP udp;
QTRSensors qtr;

uint16_t sensorValues[NUM_SENSORS];
int ready = 0;

Adafruit_NeoPixel pixels(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  pixels.begin();

// Set motor PWM pins as OUTPUT

  pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);
  analogReadResolution(12);

// Setup the QTR Sensor

  qtr.setTypeAnalog();
  // qtr.setSensorPins((const uint8_t[]){ 7, 4, 5, 10, 6, 8, 3, 9}, NUM_SENSORS);
  qtr.setSensorPins((const uint8_t[]){ 9, 3, 8, 6, 10, 5, 4, 7}, NUM_SENSORS);
  qtr.setEmitterPin(EMITTER_PIN);

// For debugging only

  Serial.begin(9600);

// if you want esp32 to be the access point

  WiFi.softAP(ssid, password);

  // if you want to connect to existing network

  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(1000);
  //   Serial.println("Łączenie z WiFi...");
  // }

// Otwarcie socketu UDP na porcie 1234 do nasłuchiwania

  // if(udp.listen(5005)) {
  if(udp.listen(1234)) {
      Serial.print("UDP Listening on IP: ");
      Serial.println(WiFi.localIP());
  }

// Handler otrzymanych pakietów

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
          if(firstTwoLetters == "Th"){
            String numericalPart = message.substring(4);
            float numericalValue = numericalPart.toFloat();
            lost_threshold = numericalValue;
          }
    });
    
}

int lastError = 0;
int rightMotorSpeed = 0;
int leftMotorSpeed = 0;
int last_sighted = 0;
int lost;
int lost_sensors;
int last_detection_time = 0;

void loop()
{
  // Czytaj pozycję czarnej linii

  int position = qtr.readLineBlack(sensorValues);

  // Zapamiętywanie ostatniej pozycji linii

if (sensorValues[0] >= 800 && sensorValues[NUM_SENSORS-1] < 800) {
    if (last_sighted != 1 && millis() - last_detection_time >= 100) {
      last_sighted = 1;
      last_detection_time = millis();
      pixels.setPixelColor(0, pixels.Color(0, 255, 0)); // Zielony dla lewej
      pixels.show();
    }
  } else if (sensorValues[0] < 800 && sensorValues[NUM_SENSORS-1] >= 800) {
    if (last_sighted != 2 && millis() - last_detection_time >= 100) {
      last_sighted = 2;
      last_detection_time = millis();
      pixels.setPixelColor(0, pixels.Color(255, 0, 0)); // Czerwony dla prawej
      pixels.show();
    }
  } 

  // if(position <= 1800 || sensorValues[0] >= 800){
  //   if(lost == 0){
  //     last_sighted = 1;
  //     pixels.setPixelColor(0, pixels.Color(0, 255, 0));
  //     pixels.show();
  //   }
  // } 
  // else if(position >= 5500 || sensorValues[NUM_SENSORS-1] >= 800){
  //   if(lost == 0){
  //     last_sighted = 2;
  //     pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  //     pixels.show();
  //   }
  // }

  // Sprawdzanie czy robot się zgubił

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

  // Regulator PID. Error nalezy ustawić w zalezności od ilości czujników np. dla 6 czujników max pozycja to 5000 dlatego srodek linii to 2500 a dla 8 czujników max pozycja to 7000 więc środek linii to 3500

  int error = position - 3500;

  int motorSpeed = Kp * error + Kd * (error - lastError);
  lastError = error;

  rightMotorSpeed = BaseSpeed + motorSpeed;
  leftMotorSpeed = BaseSpeed - motorSpeed;
  
  if (rightMotorSpeed > MaxSpeed ) rightMotorSpeed = MaxSpeed; 
  if (leftMotorSpeed > MaxSpeed ) leftMotorSpeed = MaxSpeed; 
  if (rightMotorSpeed < 0) rightMotorSpeed = 0; 
  if (leftMotorSpeed < 0) leftMotorSpeed = 0; 

  // Wysyłanie danych czujników co ustalony interwał czasu (ms)
  
  int interval = 200;
  static unsigned long lastMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - lastMillis >= interval) { 
    lastMillis = currentMillis;
    String pos = "Position: " + String(position); 
    udp.broadcast(pos.c_str()); 
    udp.broadcast("!"); 
    request_sensorsRaw();
  }

  // Algorytm jazdy

  if(ready == 1){
    if(lost == 1 && last_sighted == 1){
      analogWrite(RIGHT_MOTOR_FORWARD, TurnSpeed); 
      analogWrite(LEFT_MOTOR_BACKWARD, TurnSpeed); 
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
      delay(10);
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

// Funkcja kalibracji. "C" jest uywane przez aplikację na telefonie do określenia czy kalibracja się skończyła

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

// Wysyła wartości kazdego czujnika do aplikacji

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

// Wysyła aktualne parametry do aplikacji

void request_params(){
  String params = "Kp: " + String(Kp) + " Ki: " + String(Ki) + " Kd: " + String(Kd) + " Max: " + String(MaxSpeed) + " Base: " + String(BaseSpeed) + " Turn: " + String(TurnSpeed) + " Lost_th: " + String(lost_threshold);
  // Serial.println(params);
  udp.broadcast(params.c_str());
}
