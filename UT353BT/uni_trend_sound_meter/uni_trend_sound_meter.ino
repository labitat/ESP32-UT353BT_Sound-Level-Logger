#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEClient.h>

// Define constants for easier readability
static const char *const TAG = "uni_trend_sound_meter";
static const unsigned short CMD_QUERY = 0x5E;
static const uint16_t CMD_LENGTH = 1;
static const uint8_t MEASUREMENT_VALUE_START_INDEX = 6;
static const uint16_t MEASUREMENT_VALUE_LENGTH = 5;

// Callback class for handling advertised BLE devices
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
public:
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    // Process the scanned device here
  }
};

// Class representing a UnitTrend Sound Meter device
class UnitTrendSoundMeter : public BLEClientCallbacks {
public:
  BLEClient* bleClient; // Bluetooth Low Energy client
  bool connected = false; // Flag indicating if the device is connected
  uint16_t output_handle_; // Handle for the output

  // Constructor
  UnitTrendSoundMeter() {}

  // Callback when device is connected
  void onConnect(BLEClient* pclient) {
    connected = true;
    // Retrieve the list of services
    std::map<std::string, BLERemoteService*>* services = pclient->getServices();
    if (services != nullptr) {
      // Iterate through the services
      for (auto const& service : *services) {
        // Check if the service UUID matches the desired UUID
        if (service.second->getUUID().equals(BLEUUID("0000FF12-0000-1000-8000-00805F9B34FB"))) {
          // Get the characteristic with UUID 0xFF02
          BLERemoteCharacteristic* pCharacteristic = service.second->getCharacteristic(BLEUUID("0000FF02-0000-1000-8000-00805F9B34FB"));
          if (pCharacteristic != nullptr) {
            // Register for notifications
            pCharacteristic->registerForNotify([](BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
              // Print received notification
              Serial.print("Notification: ");
              Serial.write(pData, length);
              Serial.println();
            });
          }
          break; // Exit the loop after finding the service
        }
      }
    }
  }

  // Callback when device is disconnected
  void onDisconnect(BLEClient* pclient) {
    connected = false;
  }
};

// Instantiate the UnitTrendSoundMeter and MyAdvertisedDeviceCallbacks objects
UnitTrendSoundMeter soundMeter;
MyAdvertisedDeviceCallbacks advertisedDeviceCallbacks;

// Setup function
void setup() {
  Serial.begin(115200);
  Serial.println("BLEDevice init");
  BLEDevice::init("");
  Serial.println("BLEDevice init");
  BLEScan* pBLEScan = BLEDevice::getScan();
  Serial.println("BLEDevice getScan");
  pBLEScan->setAdvertisedDeviceCallbacks(&advertisedDeviceCallbacks);
  Serial.println("pBLEScan->setAdvertisedDeviceCallbacks");
  pBLEScan->setActiveScan(true);
  Serial.println("pBLEScan->setActiveScan");
  pBLEScan->start(30);
  Serial.println("pBLEScan->start");

  delay(5000);
  Serial.println("delay 5000");

  // Connect to the specific BLE device
  BLEAddress addr("E8:D0:3C:52:22:C3");
  Serial.println("BLEAddress set");
  soundMeter.bleClient = BLEDevice::createClient();
  Serial.println("soundMeter.bleClient = BLEDevice::createClient");
  soundMeter.bleClient->setClientCallbacks(&soundMeter);
  Serial.println("soundMeter.bleClient->setClientCallbacks");
  soundMeter.bleClient->connect(addr);
  Serial.println("soundMeter.bleClient->connect");
  Serial.println("entering loop");
}

// Loop function
void loop() {
  delay(1000);
}

/*

 
Generic Attribute
UUID: 0x1801
PRIMARY SERVICE
  Service Changed
  UUID: 0x2A05  <--- !!!_ACTIVATE_!!!
  Properties: INDICATE
  Descriptors:
    Client Characteristic Configuration
    UUID: 0x2902
    Value: "Notifications and indications disabled"/"Indications enabled"

Generic Access
UUID: 0x1800
PRIMARY SERVICE
  Device Name
  UUID: 0x2A00
  Properties: READ
  Value: UT353BT

Unknown Service
UUID: 0xFF12
PRIMARY SERVICE
  Characteristic:
    UUID: 0xFF02  <--- !!!_ACTIVATE_!!!
    Properties: NOTIFY
    Descriptors:
      Client Characteristic Configuration
        UUID: 0x2902
        Value: "Notifications and indications disabled"/"Notifications enabled"
      Characteristic User Description
        UUID: 0x2901
        Value: "Data Out"
  Characteristic:
    UUID: 0xFF06
    Properties: READ, WRITE
    Value: (0x) 55-54-33-35-33-42-54, "UT353BT"
    Descriptors:
      Characteristic User Description
        UUID: 0x2901
        Value: BT Name
  Characteristic:
    UUID: 0xFF08
    Properties: READ, WRITE
    Value: (0x) 40-01
    Descriptors:
      Characteristic User Description
        UUID: 0x2901
        Value: Adv. Interval
  Characteristic:
    UUID: 0xFF0B
    Properties: READ, WRITE
    Value: (0x) 00
    Descriptors:
      Characteristic User Description
        UUID: 0x2901
        Value: Mode Control

Unknown Service
UUID: 0000d0ff-3c17-d293-8e48-14fe2e4da212
PRIMARY SERVICE
  Characteristic:
    UUID: 0x
    Properties: 
    Value: 
    Descriptors:
      Client Characteristic Configuration
        UUID: 0x
        Value: 
      Characteristic User Description
        UUID: 0x
        Value: 
  Characteristic:
    UUID: 0x
    Properties: 
    Value: 
    Descriptors:
      Client Characteristic Configuration
        UUID: 0x
        Value: 
      Characteristic User Description
        UUID: 0x
        Value: 
  Characteristic:
    UUID: 0x
    Properties: 
    Value: 
    Descriptors:
      Client Characteristic Configuration
        UUID: 0x
        Value: 
      Characteristic User Description
        UUID: 0x
        Value: 

*/
