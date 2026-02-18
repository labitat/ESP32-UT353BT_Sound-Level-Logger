#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// Define the UUIDs of the service and characteristic
#define SERVICE_UUID        "0000ff12-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_UUID "0000ff01-0000-1000-8000-00805f9b34fb"
#define NOTIFICATION_UUID   "0000FF02-0000-1000-8000-00805f9b34fb"
#define CCCD                "00002902-0000-1000-8000-00805f9b34fb"

// Create a BLEClient object
BLEClient* pClient;

const uint8_t data_MAX[]  = {0xAA, 0xBB, 0x04, 0x30, 0x3F, 0x01, 0xD8};
const uint8_t data_MIN[]  = {0xAA, 0xBB, 0x04, 0x30, 0x40, 0x01, 0xD9};
const uint8_t data_AVG[]  = {0xAA, 0xBB, 0x04, 0x30, 0x41, 0x01, 0xDA};
const uint8_t data_HOLD[] = {0xAA, 0xBB, 0x04, 0x30, 0x42, 0x01, 0xDB};
const uint8_t data_FAST[] = {0xAA, 0xBB, 0x04, 0x30, 0x43, 0x01, 0xDC};
const uint8_t data_SLOW[] = {0xAA, 0xBB, 0x04, 0x30, 0x44, 0x01, 0xDD};
const uint8_t poll[]   = {0x5E};

// Definition of connectToServer() function
void connectToServer() {
  // Scan for BLE servers
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setActiveScan(true);
  BLEScanResults foundDevices = pBLEScan->start(5);

  // Connect to the server with the specified name
  for (int i = 0; i < foundDevices.getCount(); i++) {
    if (foundDevices.getDevice(i).haveName() && foundDevices.getDevice(i).getName() == "UT353BT") {
      Serial.println("Found device: UT353BT");
      BLEAdvertisedDevice device = foundDevices.getDevice(i);
      pClient->connect(&device);
      break;
    }
  }
}

/*
void writeRXCharacteristic() {
  uint8_t bytes[] = {94}; // 0x5E
  
  if (pClient->isConnected()) {
    BLERemoteService* RxService = pClient->getService(SERVICE_UUID);
    if (RxService == nullptr) {
      Serial.println("RX service not found!");
      pClient->disconnect(); // Disconnect from the BLE server if service  not found
      return;
    }
    BLERemoteCharacteristic* RxChar = RxService->getCharacteristic(CHARACTERISTIC_UUID);
    if (RxChar == nullptr) {
      Serial.println("RX characteristic not found!");
      pClient->disconnect(); // Disconnect from the BLE server if  characteristic not found
      return;
    }
    // Set value to the characteristic
    RxChar->writeValue(bytes, sizeof(bytes));
    Serial.println("0x5E written");
  }
}
*/
/*
void enableTXNotification() {
  if (pClient->isConnected()) {
    // Get the service
    BLERemoteService* RxService = pClient->getService(SERVICE_UUID);
    if (RxService == nullptr) {
      Serial.println("RX service not found!");
      pClient->disconnect(); // Disconnect from the BLE server if service  not found
      return;
    }

    // Get the characteristic
    BLERemoteCharacteristic* TxChar = RxService->getCharacteristic(NOTIFICATION_UUID);
    if (TxChar == nullptr) {
      Serial.println("TX characteristic not found!");
      pClient->disconnect(); // Disconnect from the BLE server if  characteristic not found
      return;
    }

    // Enable notifications
    if (TxChar->canNotify()) {
      TxChar->registerForNotify(notifyCallback);
    } else {
      Serial.println("Notifications not supported!");
      return;
    }

    // Write the CCCD descriptor to enable notifications
    BLERemoteDescriptor* Descriptor = TxChar->getDescriptor(CCCD);
    if (Descriptor != nullptr) {
      uint8_t enableNotificationValue[] = {BLE_SIG_CLIENT_CHARACTERISTIC_CONFIGURATION_UUID, 0x01, 0x00};
      Descriptor->writeValue(enableNotificationValue, sizeof(enableNotificationValue));
      Serial.println("Notifications enabled.");
    } else {
      Serial.println("Descriptor not found!");
    }
  } else {
    Serial.println("Error: Not connected to server.");
  }
}
*/

void writeToCharacteristic(const uint8_t* data, size_t dataLength) {
  // Create a non-const copy of the data array
  uint8_t dataArray[dataLength];
  memcpy(dataArray, data, dataLength);

  // Print the data array to terminal
  Serial.print("Data Array: ");
  for (size_t i = 0; i < dataLength; i++) {
    Serial.print(dataArray[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  if (pClient->isConnected()) {
    BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID);
    if (pRemoteService != nullptr) {
      BLERemoteCharacteristic* pRemoteCharacteristic = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID);
      if (pRemoteCharacteristic != nullptr) {
        // Write data to the characteristic
        pRemoteCharacteristic->writeValue(dataArray, dataLength);
      }
    }
  } else {
    Serial.println("Error: connection lost");
  }
}

// Callback for receiving data from the characteristic
class MyClientCallback : public BLEClientCallbacks {
    void onConnect(BLEClient* pClient) {
      Serial.println("Connected to device.");
    }

    void onDisconnect(BLEClient* pClient) {
      Serial.println("Disconnected from device.");
      connectToServer();
    }

    void onServicesDiscovered(BLEClient* pClient) {
      Serial.println("Discovered GATT services.");
      writeRXCharacteristic();
    }
};

void send(const uint8_t* data, size_t dataLength) {
  writeToCharacteristic(data, dataLength);
  delay(75);
  writeToCharacteristic(poll, 1);
  delay(75);
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  delay(1000);
  Serial.println("Starting BLE Client...");

  BLEDevice::init("ESP32");
  pClient = BLEDevice::createClient();
  pClient->setClientCallbacks(new MyClientCallback());

  connectToServer();
}

void loop() {
  if (!pClient->isConnected()) {

    // not connected
    Serial.println("Connecting...");
    connectToServer();
    delay(100); // Wait for 100 milliseconds before trying to reconnect

  } else {

    // connected
    send(data_MIN, 7);
    send(data_MAX, 7);
    delay(700);

/*
    delay(50);
    writeToCharacteristic(poll, 1);
    delay(950);
    writeToCharacteristic(data_MAX, 7);
    delay(50);
    writeToCharacteristic(poll, 1);
    delay(950);
    writeToCharacteristic(data_MIN, 7);
    delay(50);
    writeToCharacteristic(poll, 1);
    delay(950);
    writeToCharacteristic(data_MIN, 7);
    delay(50);
    writeToCharacteristic(poll, 1);
    delay(50);
    writeToCharacteristic(data_HOLD, 7);
    delay(50);
    writeToCharacteristic(poll, 1);
    delay(850);
    writeToCharacteristic(data_HOLD, 7);
    delay(50);
    writeToCharacteristic(poll, 1);
    delay(50);
    writeToCharacteristic(data_FAST, 7);
    delay(50);
    writeToCharacteristic(poll, 1);
    delay(850);
    writeToCharacteristic(data_SLOW, 7);
*/
  }
}
