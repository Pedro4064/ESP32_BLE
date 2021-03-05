#ifndef BLE_H
#define BLE_H

#include <Arduino.h>
#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <string.h>

#include <map>
#include <vector>

namespace Bluetooth {

//callback para receber os eventos de conexão de dispositivos
class ServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer);

    void onDisconnect(BLEServer* pServer);
};

//callback para eventos das características
class CharacteristicCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* characteristic);
};

//'wrapper' around the whole BLE api
class BLE {
    // Private Variables
   private:
    const char* SERVICE_UUID = "ab0828b1-198e-4351-b779-901fa0e0371e";

    BLEServer* pserver;    // pointer to the BLE server
    BLEService* pservice;  // pointer to the service

    std::string DeviceName = "ESP32-BLE";  // Default name for the device

    // The name of the characteristic the user wants to use in the moment
    String target_characteristic;

    // map that will hold the infos for each characteristic until they are initialized
    std::map<String, const char*> characteristics_info;

    // map that will hold the info for all the characteristics being used  -- They are initialized on BLE::begin()
    std::map<String, BLECharacteristic*> characteristics_list;

    // Public Variables
   public:
    // Create struct that will hold all essential information about the parsed data
    struct data_information {
        int array_size;
        std::string* data_array;
    };

    // True if there is a device connected, false otherwise
    // Static variable so it is accesable without a BLE instance.
    static bool deviceConnected;

    // Variable that tracks rather there are new messages sent
    static bool new_message;

    // Private Methods
   private:
    // Private method that takes an std::string and slices it into an array of chars
    BLE::data_information parseData(std::string data);

    // Public Methods
   public:
    // Deconstructor
    ~BLE() {
        // ! We need to iterate through the map of characteristic and release memory to the system
        for (auto& characteristic : characteristics_list) {
            delete characteristic.second;
        }
    }

    // Method to be called in void setUp() to initialize the whole BLE
    void begin();

    // Method to set the name which the ESP32 will be discoverable to other devices
    void setDeviceName(std::string name);

    // Method used to change which characteristic the user is interacting with
    void use_characteristic(String characteristic_name);

    // Mehtod that will receive and parse incoming data into an array of doubles and return its pointer
    std::vector<double> receivedDataAsDoubleVector();

    // Mehtod that will receive the string data, parse it and return an int array with all datapoints
    std::vector<int> receivedDataAsIntVector();

    // Mehtod that will receive the string data, parse it and return a float array with all datapoints
    std::vector<float> receivedDataAsFloatVector();

    // Method that will receive and transform the data into a string and return it
    String receivedDataAsString();

    // Mehtod that will receive the name and the uuid for a charactertic, instantiate the ble
    // charactertic class   and save its pointer and name to the map
    void add_characteristic(String characteristic_name, const char* uuid);

    // Method template that will take the data array, convert to string, send and notify the client
    template <typename T>
    void sendDataArray(T data[], int array_size, String delimiter) {
        String data_string = "";  //String variable that will hold the string verion of the Variables

        for (int i = 0; i < array_size; i++) {
            // Attack the new string formatted data to the data_string variable
            data_string += String(data[i]);

            // If its not the last number in the array, add the delimiter
            if (i != array_size - 1) data_string += delimiter;
        }

        // Send the string and notify the cliente
        std::string value(data_string.c_str());
        characteristics_list[target_characteristic]->setValue(value);  //Make the string a c string(char*) to be sent via BLE
        characteristics_list[target_characteristic]->notify();
    }

    // Method template that will send one data point via BLE
    template <typename T>
    void sendDataPoint(T data_point) {
        // Send the data and notify the cliente, setValue has many overloading
        characteristics_list[target_characteristic]->setValue(data_point);
        characteristics_list[target_characteristic]->notify();
    }

    // Release memory allocated dynamically by an array back to the system
    template <typename T>
    void static releaseMemoryToSystem(T* data) {
        delete[] data;
    }
};

}  // namespace Bluetooth
#endif
