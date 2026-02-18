#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// Forward declaration of MyAdvertisedDeviceCallbacks
class MyAdvertisedDeviceCallbacks;

// Define the UUIDs of the service and characteristic
#define SERVICE_UUID        "0000ff12-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_UUID "0000ff01-0000-1000-8000-00805f9b34fb"
#define NOTIFICATION_UUID   "0000FF02-0000-1000-8000-00805f9b34fb"
#define CCCD                "00002902-0000-1000-8000-00805f9b34fb"

// Create a BLEClient object
BLEClient* pClient;

// Store the MAC address of the connected device
BLEAddress connectedDeviceAddress = BLEAddress("00:00:00:00:00:00"); // Placeholder MAC address

const uint8_t data_MAX[]  = {0xAA, 0xBB, 0x04, 0x30, 0x3F, 0x01, 0xD8};
const uint8_t data_MIN[]  = {0xAA, 0xBB, 0x04, 0x30, 0x40, 0x01, 0xD9};
const uint8_t data_AVG[]  = {0xAA, 0xBB, 0x04, 0x30, 0x41, 0x01, 0xDA}; // unsure what this does for UT353BT
const uint8_t data_HOLD[] = {0xAA, 0xBB, 0x04, 0x30, 0x42, 0x01, 0xDB};
const uint8_t data_FAST[] = {0xAA, 0xBB, 0x04, 0x30, 0x43, 0x01, 0xDC};
const uint8_t data_SLOW[] = {0xAA, 0xBB, 0x04, 0x30, 0x44, 0x01, 0xDD};
const uint8_t poll[]   = {0x5E};

// Callback for receiving advertisement data
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice);
};

void connectToServer() {
  // Scan for BLE servers
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setActiveScan(true);
  BLEScanResults foundDevices = pBLEScan->start(5);

  // Connect to the first device with the specified name
  for (int i = 0; i < foundDevices.getCount(); i++) {
    if (foundDevices.getDevice(i).haveName() && foundDevices.getDevice(i).getName() == "UT353BT") {
      Serial.println("Found device: UT353BT");
      BLEAdvertisedDevice device = foundDevices.getDevice(i);
      pClient->connect(&device);
      connectedDeviceAddress = device.getAddress(); // Save the MAC address
      break;
    }
  }
  // Set advertisement callbacks
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
}

// Definition of onResult() method for MyAdvertisedDeviceCallbacks
void MyAdvertisedDeviceCallbacks::onResult(BLEAdvertisedDevice advertisedDevice) {

  // Check if the advertisement is from the connected device
  if (advertisedDevice.getAddress() == connectedDeviceAddress) {
    Serial.println("Received advertising packet!");
    
    // Get the payload of the advertising packet
    uint8_t* payload = advertisedDevice.getPayload();
    int payloadLength = advertisedDevice.getPayloadLength();
    Serial.printf("Payload length: %d\n", payloadLength);
    if (payloadLength >= 35) {
      char chars[9]; // Buffer for characters + '\0'
      for (int i = 0; i < 8; i++) {
        chars[i] = payload[24 + i];
      }
      chars[8] = '\0'; // Null-terminate the string
      String output = String(chars);

      Serial.println(output);
    } else {
      // print all
      char chars[payloadLength+1]; // Buffer for characters + '\0'
      for (int i = 0; i < payloadLength; i++) {
        chars[i] = payload[i];
      }
      chars[payloadLength] = '\0'; // Null-terminate the string
      String output = String(chars);

      Serial.println(output);
    }
  }
}
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
      writeToCharacteristic(poll, 1);
      delay(75);
    }
};

void send(const uint8_t* data, size_t dataLength) {
  writeToCharacteristic(data, dataLength);
  delay(100);
  writeToCharacteristic(poll, 1);
  delay(100);
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
    /*
    send(data_MIN, 7);
    delay(600);
    send(data_MAX, 7);
    delay(4000);
    /*
/*
    // Keep scanning for advertisement packages
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setActiveScan(true);
    BLEScanResults foundDevices = pBLEScan->start(5);
*/
  }
}

