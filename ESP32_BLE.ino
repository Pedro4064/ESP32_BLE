#include "BLE.h"
#define log(x) Serial.println(x);

Bluetooth::BLE bluetooth;
// Sensor sensor;




void setup()
{

  // initialize the serial monitor 
  Serial.begin(115200);

  // Add the characteristics that will be used 
  bluetooth.setDeviceName("Pedro's RunPod");
  bluetooth.add_characteristic("Potato", "fbed8ddc-109f-11eb-adc1-0242ac120002");
  bluetooth.add_characteristic("Pudim", "16dc549c-10a1-11eb-adc1-0242ac120002");

  // Initialize the BLE 
  bluetooth.begin();

}
  
void loop() {
  
  delay(1000);
  
  //If a device connected to the ESP32, begin the processes, otherwise keep scanning.
  if(Bluetooth::BLE::deviceConnected )
  {
    log("Device connected!!"); 

    // Counter that send every odd number to the potato characteristic and every even to the Pudim characteristic
    int i = 0;
    while(true){

      i++;
      if(i%2 == 0){
        bluetooth.use_characteristic("Potato"); // Change to the specific characteristic     
      }
      else{
        bluetooth.use_characteristic("Pudim"); // Change to the specific characteristic 
      }
      
      // Send the value of i and notify
      bluetooth.sendDataPoint(i);
      delay(500);
    }
  }
    
  
}
