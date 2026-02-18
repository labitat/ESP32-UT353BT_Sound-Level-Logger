#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

BLEScan* pBLEScan;
String previousValue = ""; // Variable to store the previous value

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
    bool deviceFound = false;

    void onResult(BLEAdvertisedDevice advertisedDevice) {
      if (!deviceFound) {
        String deviceName = advertisedDevice.getName().c_str();
        if (deviceName == "UT353BT") {
          deviceFound = true;
          //BLEAddress addr = advertisedDevice.getAddress();
          //Serial.println(addr.toString().c_str());

          // Get the payload of the advertising packet
          uint8_t* payload = advertisedDevice.getPayload();
          int payloadLength = advertisedDevice.getPayloadLength();
          if (payloadLength >= 35) {
            char chars[9]; // Buffer for characters + '\0'
            for (int i = 0; i < 8; i++) {
              chars[i] = payload[24 + i];
            }
            chars[8] = '\0'; // Null-terminate the string
            String output = String(chars);

            // Print the value only if it's different from the previous one
            if (output != previousValue) {
              Serial.println(output);
              previousValue = output;
            }
          }
          pBLEScan->stop(); // Stop scanning once the device is found
        }
      }
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE Scan...");
  BLEDevice::init("");
}

void loop() {
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5);  //Scan for 5 seconds
  delay(50);
}
