// The Echelon Services
static BLEUUID     deviceUUID("0bf669f0-45f2-11e7-9598-0800200c9a66");
static BLEUUID    connectUUID("0bf669f1-45f2-11e7-9598-0800200c9a66");
static BLEUUID      writeUUID("0bf669f2-45f2-11e7-9598-0800200c9a66");
static BLEUUID     sensorUUID("0bf669f4-45f2-11e7-9598-0800200c9a66");

static BLEAdvertisedDevice * devices[20];
static int device_count = 0;

// Add a device to our device list (deduplicate in the process)
void addDevice(BLEAdvertisedDevice * device) {
  for (uint8_t i = 0; i < device_count; i++) {
    if(device->getName() == devices[i]->getName()){
      return;
    }
  }
  devices[device_count] = device;
  device_count++;
}

// Select a device and return it
BLEAdvertisedDevice * selectDevice(void) {
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->setLogBuffer(5, 50);
  for (uint8_t i = 0; i < device_count; i++) {
    Heltec.display->clear();
    // Print to the screen
    Heltec.display->println(devices[i]->getName().c_str());
    // Draw it to the internal screen buffer
    Heltec.display->drawLogBuffer(0, 0);
    // Display it on the screen
    Heltec.display->display();
    delay(500);
  }

  // Delay, return the first device that matches for testing
  delay(4000);
  for (uint8_t i = 0; i < device_count; i++) {
    if(devices[i]->haveServiceUUID() && devices[i]->isAdvertisingService(deviceUUID)) {
      return devices[i];
    }
  }
  return nullptr;
}
