#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include <Arduino.h>
unsigned long startTime;

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
BLEDescriptor* pDescr;
BLE2902* pBLE2902;

bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value[3] = { 0, 0, 0 };

// Definições dos pinos
const int triggerPinF = 23;  // Pino de Trigger do HY-SRF05
const int echoPinF = 22;     // Pino de Echo do HY-SRF05
const int triggerPinD = 5;   // Pino de Trigger do HY-SRF05
const int echoPinD = 18;     // Pino de Echo do HY-SRF05
const int triggerPinE = 4;   // Pino de Trigger do HY-SRF05
const int echoPinE = 2;      // Pino de Echo do HY-SRF05
const int piezo = 27;        // Pino do buzzer bengala
const int piezo2 = 12;        // Pino do buzzer bengala
const int vibraE = 26;       // Pino do vibracall esquerdo
const int vibraD = 25;       // Pino do vibracall direito

float distancia = 0;
float distanciaD = 0;
float distanciaE = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};

// Função para calcular a distância
float calcularDistancia(int triggerPin, int echoPin) {
  // Variáveis para armazenar o tempo
  long duration;
  float distance;
  // Gera um pulso de trigger
  digitalWrite(triggerPin, LOW);   // Garante que o trigger está em LOW
  delayMicroseconds(2);            // Espera 2 microssegundos
  digitalWrite(triggerPin, HIGH);  // Gera um pulso de 10 microssegundos
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);  // Desliga o trigger

  // Lê o tempo que o pulso leva para voltar
  duration = pulseIn(echoPin, HIGH);

  // Calcula a distância em centímetros
  distance = (duration / 2.0) * 0.0344;

  return distance;
}

void labirinto() {
  if (distancia >= 35) {
    digitalWrite(piezo, LOW);
    digitalWrite(piezo2, LOW);
  } else {
    digitalWrite(piezo, HIGH);
    digitalWrite(piezo2, HIGH);
  }
  if (distanciaD >= 35) {
    dacWrite(vibraD, 0);
  } else if (distanciaD < 35 && distanciaD > 10) {
    dacWrite(vibraD, 125);
  } else {
    dacWrite(vibraD, 255);
  }
  if (distanciaE >= 35) {
    dacWrite(vibraE, 0);
  } else {
    dacWrite(vibraE, 125);
  }
}

void setup() {
  Serial.begin(115200);

  // Create the BLE Device
  BLEDevice::init("ESP32");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService* pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_NOTIFY);

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
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");

  startTime = millis();

  pinMode(triggerPinF, OUTPUT);
  pinMode(echoPinF, INPUT);
  pinMode(triggerPinD, OUTPUT);
  pinMode(echoPinD, INPUT);
  pinMode(triggerPinE, OUTPUT);
  pinMode(echoPinE, INPUT);
  pinMode(piezo, OUTPUT);
  pinMode(piezo2, OUTPUT);
  pinMode(vibraE, OUTPUT);
}

void loop() {


  // Chama a função para calcular a distância
  distancia = calcularDistancia(triggerPinF, echoPinF);
  distanciaD = calcularDistancia(triggerPinD, echoPinD);
  distanciaE = calcularDistancia(triggerPinE, echoPinE);
  value[0] = distancia;
  value[1] = distanciaD;
  value[2] = distanciaE;
  Serial.println(value[0]);
  Serial.println(value[1]);
  Serial.println(value[2]);

  labirinto();
  // notify changed value
  if (deviceConnected) {

    uint8_t buffer[sizeof(value)];
    // Copie os dados do vetor de uint32_t para o buffer de uint8_t
    memcpy(buffer, value, sizeof(value));
    // Agora use o buffer de uint8_t com a função setValue
    pCharacteristic->setValue(buffer, sizeof(buffer));
    //pCharacteristic->setValue(value, 3);
    pCharacteristic->notify();
    delay(10);
  }
  // disconnecting
  if (!deviceConnected && oldDeviceConnected) {
    delay(500);                   // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising();  // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected) {
    // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
  }
}
