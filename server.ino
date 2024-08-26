/*
  Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
  Ported to Arduino ESP32 by Evandro Copercini
  updated by chegewara and MoThunderz
*/
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include <Arduino.h>
unsigned long startTime;

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
BLEDescriptor *pDescr;
BLE2902 *pBLE2902;

bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

// Definições dos pinos
const int triggerPin = 13;  // Pino de Trigger do HC-SR04
const int echoPin = 14;    // Pino de Echo do HC-SR04

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

// Função para calcular a distância
float calcularDistancia() {
  // Variáveis para armazenar o tempo
  long duration;
  float distance;

  // Gera um pulso de trigger
  digitalWrite(triggerPin, LOW);  // Garante que o trigger está em LOW
  delayMicroseconds(2);           // Espera 2 microssegundos
  digitalWrite(triggerPin, HIGH); // Gera um pulso de 10 microssegundos
  delayMicroseconds(10);          
  digitalWrite(triggerPin, LOW);  // Desliga o trigger

  // Lê o tempo que o pulso leva para voltar
  duration = pulseIn(echoPin, HIGH);

  // Calcula a distância em centímetros
  distance = (duration / 2.0) * 0.0344;

  return distance;
}

void setup() {
  Serial.begin(115200);

  // Create the BLE Device
  BLEDevice::init("ESP32");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );                   

  // Create a BLE Descriptor
  
  pDescr = new BLEDescriptor((uint16_t)0x2901);
  pDescr->setValue("A very interesting variable");
  pCharacteristic->addDescriptor(pDescr);
  
  pBLE2902 = new BLE2902();
  pBLE2902->setNotifications(true);
  pCharacteristic->addDescriptor(pBLE2902);

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");

   startTime = millis();

  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  

    // notify changed value
    if (deviceConnected) {
        //value = analogRead(34);
        pCharacteristic->setValue(value);
        pCharacteristic->notify();
        //valor = 1;
        // Chama a função para calcular a distância
        float distancia = calcularDistancia();
        value = distancia;
        Serial.println(distancia);
        Serial.println(value);
        unsigned long elapsedTime = millis() - startTime;
        Serial.print("Tempo decorrido: ");
        Serial.print(elapsedTime);
        Serial.println(" ms");
        delay(1000);
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
  //valor = 1;
}
