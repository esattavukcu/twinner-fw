void  bleSetup() {
    testCommand(); // Send command AT to break BLE connection if any
    enableNotificatios(); // Enable notifications for connection status
    bleSetAndCheckName(); // Name the module w.r.t device id
    
}

void testCommand(){
   _Bluetooth.print("AT");
    delay(50); 
    if (_Bluetooth.available()) {
      String test =_Bluetooth.readString();
      //_USB.println(test); 
      //if (test == "OK+LOST") {
        //_USB.println("disconnected from remote device.");                        
     // }
    } 
    //else {
    //_USB.println("No response from Bluetooth module for AT.");
    //}
}


void  enableNotificatios() {
    _Bluetooth.print("AT+NOTI1");
    delay(50);
    if (_Bluetooth.available()) {
        String response = _Bluetooth.readString();
        //_USB.println("Notification Response: " + response);
        //if (response == "OK+Set:1") {
            //_USB.println("Notifications enabled successfully.");        
        //} else {
            //_USB.println("Failed to enable notifications. Response: " + response);
        //}
    //} else {
        //_USB.println("No response from Bluetooth module for notification.");
    }
}

void bleSetAndCheckName(){

  if (EEPROM.read(ID_flagAddress) == 0x01) {
    
    //_USB.println("Device ID is already known");
      
     uint8_t storedID = EEPROM.read(ID_Address);

     deviceName = mapIdToRgb(storedID);
        
     //_USB.print("expected name is : ");
     //_USB.println(deviceName);
        
     _Bluetooth.print("AT+NAME?");
      delay(50);
      if(_Bluetooth.available()){
         bleName = _Bluetooth.readString();     
         //_USB.println(bleName);        
         //if (strcmp(bleName.c_str(), ("OK+NAME:" + deviceName).c_str()) == 0){
            //_USB.println("BLE Name is checked ");     
          //  }
            //else _USB.println("name not checked"); 
         }
         //else _USB.println("no response for name request" ); 
  }
  else {

    deviceId = generateIdFromMac();
    deviceName = mapIdToRgb(deviceId);
    
    _Bluetooth.print("AT+NAME" + deviceName);
     delay(50); 
    if(_Bluetooth.available()){
      bleName = _Bluetooth.readString();
      //_USB.println(bleName);
      if (bleName == "OK+Set:" + deviceName) {
        //_USB.println("BLE name is set as: " + deviceName );
        EEPROM.write(ID_Address, deviceId);
        EEPROM.write(ID_flagAddress, 1);
        //_USB.println("ID is written to EEPROM.");        
      }  
    }
    //else _USB.println("no response for name");
    
    _Bluetooth.print("AT+RESET");
    delay(50); 
    if(_Bluetooth.available()){
      String reset = _Bluetooth.readString();
      
//      if (reset == "OK+RESET") {
//        _USB.println("HM10 reset");
//      }
//      else _USB.println("HM10 not reset");
        
      }
  } 
}




void handleBluetoothState() {

  
   if (bluetoothState != prebluetoothState) {

      if(bluetoothState == CONNECTED){
        twinRGB.setPixelColor(0, 0, 0, 0);
        twinRGB.setPixelColor(1, 0, 0, 0);
        twinRGB.setPixelColor(2, 0, 0, 0);
        twinRGB.show(); 
      }
      else {
        initTwin();
        twinRGB.setPixelColor(0, colors[colorIndex1][0], colors[colorIndex1][1], colors[colorIndex1][2]);
        twinRGB.setPixelColor(1, colors[colorIndex2][0], colors[colorIndex2][1], colors[colorIndex2][2]);
        twinRGB.setPixelColor(2, colors[colorIndex3][0], colors[colorIndex3][1], colors[colorIndex3][2]);
        twinRGB.show();
        
      }
       
           
          
    }
        

    prebluetoothState = bluetoothState; 
}
