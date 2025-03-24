#include <Arduino.h>
#include <string>
#include <iomanip>
#include "myRTOS.h"
#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "BluetoothSerial.h"
/* #include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
 */
#include "DHT20.h"
#include "esp_now.h"
#include  "SoftServo.h"

using namespace std;

#define NodeID 1
#define GrSensor 32
#define Photosensor 27
#define Pump 19
#define LED 33
#define Servo 15

int AirHumidity = 0;
int AirTemp = 0;
int GrHumidity = 0;
int LEDStatus = 0;
int Light = 0;
int PumpStatus = 0;
int ServoStatus = 0;

char commandFromCentral[10];

char Case1[10] = {1};
char Case2[10] = {2};
char Case3[10] = {3};
char Case4[10] = {4};

uint8_t CentralNodeMAC[] = {0xC8, 0xF9, 0xE0, 0xAC, 0x44, 0x54};           //Change this to match the central node MAC address

SoftServo myservo;

DHT20 dht20;

LiquidCrystal_I2C lcd(0x21,16,2);

bool isCommandModeOn = false;

//Struct of Data to send
typedef struct{
  int nodeid;
  int temp;
  int light;
  int airm;
  int soil;
} DataToCentral;

//Struct of Data Receive from central
typedef struct
{
  char command[10];
} DataFromCentral;

// put function definitions here:

void ReadSensor(){
  AirHumidity = dht20.getHumidity();
  AirTemp = dht20.getTemperature();
  GrHumidity = analogRead(GrSensor);
  Light = analogRead(Photosensor);
}

void LCDDisplay(){
  lcd.setCursor(0, 0);
  lcd.print("C:");

  lcd.setCursor(3,0);
  lcd.print(AirTemp);

  lcd.setCursor(5,0);
  lcd.print("A:");

  lcd.setCursor(8,0);
  lcd.print(AirHumidity);

  lcd.setCursor(11,0);
  lcd.print("S:");

  lcd.setCursor(14,0);
  lcd.print(GrHumidity);

  lcd.setCursor(16,0);
  lcd.print("L:");

  lcd.setCursor(2,1);
  lcd.print(Light);
}

void ServoControl(int val){
  myservo.tick();
  if(val < 0 || val > 180) return;
  myservo.write(val);
}
void SendData(){
  DataToCentral data;
  data.light = Light;
  data.airm = AirHumidity;
  data.temp = AirTemp;
  data.soil = GrHumidity;
  data.nodeid = NodeID;
  esp_now_send(CentralNodeMAC, (uint8_t*)&data, sizeof(data));
}

void ProcessCommand(){
  if(strcmp(commandFromCentral, Case1) == 0){
    analogWrite(Pump, 0);
  }
  else if(strcmp(commandFromCentral, Case2) == 0){
    analogWrite(Pump, 250);
  }
  else if(strcmp(commandFromCentral, Case3) == 0){
    ServoControl(180);
  }
  else if(strcmp(commandFromCentral, Case4) == 0){
    ServoControl(90);
  }
}

void NormalMode(){
  ReadSensor();
  LCDDisplay();
  SendData();
}

void OnCommandMode(){
  if(isCommandModeOn == true){
    ProcessCommand();
    isCommandModeOn = false;
  }
}

void LEDOn(){
  LEDStatus = 1;
  digitalWrite(LED ,HIGH);
}

void LEDOff(){
  LEDStatus = 0;
  digitalWrite(LED , LOW);
}

void PumpOn(){
  PumpStatus = 1;
  digitalWrite(Pump, HIGH);
}

void PumpOff(){
  PumpStatus = 0;
  digitalWrite(Pump, LOW);
}


// ESP-NOW Send Callback
void onSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Gửi dữ liệu: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Thành công" : "Thất bại");
}
// ESP-NOW Receive Callback
void onReceive(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
  DataFromCentral receiveData;
  memcpy(&receiveData, incomingData, sizeof(receiveData));
  Serial.print("Nhận dữ liệu");
/*   commandFromCentral = receiveData.command;
 */
  strcpy(commandFromCentral, receiveData.command);
  isCommandModeOn = true;
}


void setup() {
  Serial.begin(115200);
  
  // Initialize WiFi in Station Mode
  WiFi.mode(WIFI_STA);
  
  // Initialize ESP-NOW
  esp_now_init();
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW initialization failed!");
    return;
  }

  // Register receive callback
  esp_now_register_send_cb(onSent);
  esp_now_peer_info_t peerInfo;

  // Register peer
  memcpy(peerInfo.peer_addr, CentralNodeMAC, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(esp_now_recv_cb_t(onReceive));
  // Set pin modes
  pinMode(LED, OUTPUT);
  pinMode(Pump, OUTPUT);
  pinMode(GrSensor, INPUT);
  pinMode(Photosensor, INPUT);
  
  myservo.attach(Servo); // servo nối với cổng 15
  myservo.asyncMode();
  myservo.delayMode();

  myservo.tick();
  myservo.write(90);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  AddTask(OnCommandMode, 0, 500);
  AddTask(NormalMode, 500, 30000);
  Serial.println("Subnode initialized.");
}

void loop() {
  // put your main code here, to run repeatedly:
  DispatchTask();
}