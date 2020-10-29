#include "Arduino.h"
#include "heltec.h"
#include "BLEDevice.h"
#include "icons.h"

// The Echelon Services
static BLEUUID     deviceUUID("0bf669f0-45f2-11e7-9598-0800200c9a66");
static BLEUUID    connectUUID("0bf669f1-45f2-11e7-9598-0800200c9a66");
static BLEUUID      writeUUID("0bf669f2-45f2-11e7-9598-0800200c9a66");
static BLEUUID     sensorUUID("0bf669f4-45f2-11e7-9598-0800200c9a66");

static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = true;

static BLERemoteCharacteristic* writeCharacteristic;
static BLERemoteCharacteristic* sensorCharacteristic;
static BLEAdvertisedDevice* device;
static BLEClient* client;
static BLEScan* scanner;

static int runtime = 0;
static int cadence = 0;
static int resistance = 0;

const int debug = 0;
const int maxResistance = 32;

static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* data, size_t length, bool isNotify) {
  switch(data[1]) {
    case 0xD1:
      runtime = int((data[7] << 8) + data[8]);
      cadence = int((data[9] << 8) + data[10]);
      break;
    case 0xD2:
      resistance = int(data[3]);
      break;
  }
  
  if(debug) {
    Serial.print("CALLBACK(");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(":");
    Serial.print(length);
    Serial.print("):");
    for(int x = 0; x < length; x++) {
      if(data[x] < 16) {
        Serial.print("0");
      }
      Serial.print(data[x], HEX);
    }
    Serial.println();
  }
}

class ClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
    digitalWrite(LED,HIGH);
    Serial.println("Connected");
  }
  void onDisconnect(BLEClient* pclient) {
    connected = false;
    digitalWrite(LED,LOW);
    Serial.println("Disconnect");
  }
};

class AdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(deviceUUID)) {
      BLEDevice::getScan()->stop();
      delay(200);
      device = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;
    }
  }
};

void updateDisplay() {
  Heltec.display->clear();
  
  // Runtime
  Heltec.display->setFont(ArialMT_Plain_24);
  char buf[5];
  const int minutes = int(runtime / 60);
  itoa(minutes, buf, 10);
  Heltec.display->setTextAlignment(TEXT_ALIGN_RIGHT);
  Heltec.display->drawString(48, 0, buf);
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->drawXbm(0, 4, clock_icon_width, clock_icon_height, clock_icon);
  Heltec.display->drawString(48, 0, ":");
  const int seconds = int(runtime % 60);
  if(seconds < 10) {
    buf[0] = '0';
    itoa(seconds, &buf[1], 10);  
  } else {
    itoa(seconds, buf, 10);  
  }
  Heltec.display->drawString(54, 0, buf);

  // Cadence
  Heltec.display->drawXbm(0, 26, cadence_icon_width, cadence_icon_height, cadence_icon);
  itoa(cadence, buf, 10);
  Heltec.display->drawString(22, 22, buf);

  // Power
  Heltec.display->drawXbm(66, 26, power_icon_width, power_icon_height, power_icon);
  itoa(cadence, buf, 10);
  Heltec.display->drawString(86, 22, buf);

  // Resistance
  Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
  itoa(resistance, buf, 10);
  Heltec.display->drawString(116, 42, buf);
  Heltec.display->drawXbm(0, 52, resistance_icon_width, resistance_icon_height, resistance_icon);
  Heltec.display->drawProgressBar(23, 49, 78, 14, uint8_t((100 * resistance) / maxResistance));

  // Echelon Icon
  Heltec.display->drawXbm(100, 3, echelon_icon_width, echelon_icon_height, echelon_icon);

  Heltec.display->display();
}

bool connectToServer() {
  Serial.print("Forming a connection to ");
  Serial.println(device->getAddress().toString().c_str());

  client = BLEDevice::createClient();
  client->setClientCallbacks(new ClientCallback());
  
  // Connect to the remove BLE Server.
  client->connect(device);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
  Serial.println(" - Connected to server");

  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* remoteService = client->getService(connectUUID);
  if (remoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(connectUUID.toString().c_str());
    client->disconnect();
    return false;
  }
  Serial.println(" - Found our service");

  // Obtain a reference to the characteristic in the service of the remote BLE server.
  sensorCharacteristic = remoteService->getCharacteristic(sensorUUID);
  if (sensorCharacteristic == nullptr) {
    Serial.print("Failed to find sensor characteristic UUID: ");
    Serial.println(sensorUUID.toString().c_str());
    client->disconnect();
    return false;
  }
  Serial.println(" - Found sensor");
  sensorCharacteristic->registerForNotify(notifyCallback);

  // Obtain a reference to the characteristic in the service of the remote BLE server.
  writeCharacteristic = remoteService->getCharacteristic(writeUUID);
  if (writeCharacteristic == nullptr) {
    Serial.print("Failed to find write characteristic UUID: ");
    Serial.println(writeUUID.toString().c_str());
    client->disconnect();
    return false;
  }
  // Enable status updates
  byte message[] = {0xF0, 0xB0, 0x01, 0x01, 0xA2};
  writeCharacteristic->writeValue(message, 5);
  Serial.println("Enabled logging");

  connected = true;
  Heltec.display->init();
  Heltec.display->flipScreenVertically();
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->clear();
  return true;
}

void setup() {
  Serial.begin(115200);
  Serial.flush();
  delay(50);
    
  Heltec.display->init();
  Heltec.display->flipScreenVertically();
  Heltec.display->clear();
  
  pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW);

  Serial.println("Starting!");
  
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0, 0, "Starting!");
  Heltec.display->display();
  
  BLEDevice::init("");
  scanner = BLEDevice::getScan();
  scanner->setAdvertisedDeviceCallbacks(new AdvertisedDeviceCallbacks());
  scanner->setInterval(1349);
  scanner->setWindow(449);
  scanner->setActiveScan(true);
}

void loop() {

  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are 
  // connected we set the connected flag to be true.
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }

  if(!connected && doScan){
    scanner->start(5, false);
  }

  delay(200); // Delay a second between loops.
  updateDisplay();
}
