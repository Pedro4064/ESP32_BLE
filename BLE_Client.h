#pragma once

#include <Arduino.h>
#include <BLEDevice.h>

#include <string>

namespace BLE_Client {

// Callback to events about device connectivity
class ClientCallbacks : public BLEClientCallbacks {
    void onConnect(BLEClient* client);

    void onDisconnect(BLEClient* client);
};

// Callback to events about scanning
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice);
};

class Client {
   public:
    /**
     * @brief Method that initializes the BLE on the ESP32 with the desired device name
     * 
    */
    void begin();

    /**
     * @brief Set the Device Name
     * 
     * @param name 
    */
    void setDeviceName(std::string name);

    /**
     * @brief Set the Service UUID 
     * 
     * @param uuid 
    */
    void setServiceUUID(const char* uuid);

    /**
     * @brief Set the Characteristic UUID 
     * 
     * @param uuid 
    */
    void setCharacteristicUUID(const char* uuid);

    /**
     * @brief Set the Target Device Address, normally we don't hard code the address
     * since it varies from device to device, we normally know the desired service UUID and the
     * desired characteristc UUID and scan devices with those.
     * 
     * @param address 
     */
    void setTargetServerAddress(BLEAddress address);

    /**
     * @brief Connect to the desired device with the BLE server
     * 
     */
    void connectToServer();

    /**
     * @brief Start the scan looking for servers with the desired service UUID
     * and the desired characteristic UUID
     * 
     */
    void startScan();

    // Public variables
    bool connected_to_server = false;
    bool new_server_message = false;

   private:
    /**
     * @brief The method that will be called when the server sends a notify signal with new data.
     * The new data will be stored in the private member std::string received_message_ for future parsing
     * 
     * @param pBLERemoteCharacteristic  pointer to the remote characteristic
     * @param pData data received 
     * @param length length of the data received
     * @param isNotify rather it was with the notify flag?
    */
    void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);

    // Private variables
    std::string device_name_;
    std::string received_message_;
    const char* service_UUID_;
    const char* characteristic_UUID_;                 //! Maybe we will have to change it to std::string
    BLERemoteCharacteristic* remote_characteristic_;  // Characteristic object to interface with the server
    BLEAdvertisedDevice* target_server_;              // Device object refering to the target server
};

}  // namespace BLE_Client
