#include <Arduino.h>
#include <stdint.h>
#include "myRTOS.h"
#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "BluetoothSerial.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "DHT20.h"
#include "esp_now.h"

#define NodeID 0
#define GrSensor 32
#define Photosensor 27
#define Pump 19
#define LED 33
#define Servo 15
