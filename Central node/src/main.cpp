#include <Arduino.h>
#include "myRTOS.h"
#include "WiFi.h"
#include "Wire.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "esp_now.h"

#define NodeID 0
#define SubnodeAmount 1
#define WifiName "ACLAB"
#define WifiPass "ACLAB2023"

#define MQTT_Server "io.adafruit.com"
#define MQTT_Port 1883
#define AIO_Usernamme "H4554N"
#define AIO_Key "aio_prvq26134XZZTkZy04gV9BUaBEMu"

bool SubnodeDataReceive = false;

typedef struct 
{
  char command[10];
}DataToSubnode;

/* String CommandFromAdafruit;
 */
typedef struct 
{
  int nodeid;
  int temp;
  int light;
  int airm;
  int soil;
}DataFromSubnode;

int nodeidFromSubnode;
int tempFromSubnode;
int lightFromSubnode;
int airmFromSubnode;
int soilFromSubnode;

/* uint8_t MACAddress[][6]{
  {}        //Add MAC address of peers
}; */

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

esp_now_peer_info_t peerInfo;
/* void OnDataSent(String CommandFromAdafruit, uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Data send status: ");
  DataToSubnode sendData;
  sendData.id = NodeID;
  sendData.command = CommandFromAdafruit;
  for(int i = 1; i < SubnodeAmount; i++){
    if(i == NodeID)  esp_now_send(MACAddress[i], (uint8_t *)&sendData, sizeof(sendData));
  }
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
} 

DataFromSubnode OnDataRecv(const uint8_t *mac, const uint8_t *incommingData, int len){
  DataFromSubnode receiveData;
  memcpy(&receiveData, incommingData, sizeof(receiveData));
  return receiveData;
}
 */

void onSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Gửi dữ liệu: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Thành công" : "Thất bại");
}

void onReceive(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
  DataFromSubnode receiveData;
  memcpy(&receiveData, incomingData, sizeof(receiveData));
  Serial.print("Nhận dữ liệu");
  nodeidFromSubnode = receiveData.nodeid;
  tempFromSubnode = receiveData.temp;
  soilFromSubnode = receiveData.soil;
  airmFromSubnode = receiveData.airm;
  lightFromSubnode = receiveData.light;
  SubnodeDataReceive = true;
}


void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
/*   WiFi.begin(WifiName, WifiPass); Serial.print("Wifi Connected1");
 */
  //Config STA mode
  WiFi.mode(WIFI_MODE_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed!");
    return;
  }

  esp_now_register_send_cb(onSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(esp_now_recv_cb_t(onReceive));
  WiFiClient WiFiClient;
/*   Adafruit_MQTT_Client mqtt(&WiFiClient, MQTT_Server, MQTT_Port, AIO_Usernamme, AIO_Key);

  Adafruit_MQTT_Publish *tempFeeds[SubnodeAmount];
  Adafruit_MQTT_Publish *lightFeeds[SubnodeAmount];
  Adafruit_MQTT_Publish *airmFeeds[SubnodeAmount];
  Adafruit_MQTT_Publish *soilFeeds[SubnodeAmount];
 
  Adafruit_MQTT_Subscribe *pumpFeeds[SubnodeAmount];
  Adafruit_MQTT_Subscribe *servoFeeds[SubnodeAmount];
  for(int i = 1; i <= SubnodeAmount; i++){
    String tempFeedName = String(AIO_Usernamme) + "/feed/temp" + "-" + i;
    String lightFeedName = String(AIO_Usernamme) + "/feed/light" + "-" + i;
    String airmFeedName = String(AIO_Usernamme) + "/feed/airm" + "-" + i;
    String soilFeedName = String(AIO_Usernamme) + "/feed/soil" + "-" + i;
    
    String pumpFeedName = String(AIO_Usernamme) + "/feed/pump" + "-" + i;
    String servoFeedName = String(AIO_Usernamme) + "/feed/servo" + "-" + i;

    //for data feed
    tempFeeds[i] = new Adafruit_MQTT_Publish(&mqtt, tempFeedName.c_str());
    lightFeeds[i] = new Adafruit_MQTT_Publish(&mqtt, lightFeedName.c_str());
    airmFeeds[i] = new Adafruit_MQTT_Publish(&mqtt, airmFeedName.c_str());
    soilFeeds[i] = new Adafruit_MQTT_Publish(&mqtt, soilFeedName.c_str());

    //for control feeds
    pumpFeeds[i] = new Adafruit_MQTT_Subscribe(&mqtt, pumpFeedName.c_str());
    servoFeeds[i] = new Adafruit_MQTT_Subscribe(&mqtt, servoFeedName.c_str());

    Serial.printf("Created feed: %s, %s, %s, %s, %s, %s\n", tempFeedName.c_str(), lightFeedName.c_str(), airmFeedName.c_str(), soilFeedName.c_str(), pumpFeedName.c_str(), servoFeedName.c_str());
  }
 */ 

 Serial.print("Initialization Completed!");
}



void loop() {
  // put your main code here, to run repeatedly:
  DataToSubnode datatosubnode;
  if(Serial.available() > 0){  
    String a = Serial.readStringUntil('#');
    a.toCharArray(datatosubnode.command, sizeof(datatosubnode.command));
    esp_now_send(broadcastAddress, (uint8_t*)&datatosubnode, sizeof(datatosubnode));
  }
  
  if(SubnodeDataReceive == true){  
    Serial.print("!"); Serial.print(nodeidFromSubnode); Serial.print(":"); Serial.print("temp:"); Serial.print(tempFromSubnode); Serial.print("#");
    Serial.print("!"); Serial.print(nodeidFromSubnode); Serial.print(":"); Serial.print("airm:"); Serial.print(airmFromSubnode); Serial.print("#");
    Serial.print("!"); Serial.print(nodeidFromSubnode); Serial.print(":"); Serial.print("soil:"); Serial.print(soilFromSubnode); Serial.print("#");
    Serial.print("!"); Serial.print(nodeidFromSubnode); Serial.print(":"); Serial.print("light:"); Serial.print(lightFromSubnode); Serial.print("#");
    SubnodeDataReceive = false;
  }
  
}

