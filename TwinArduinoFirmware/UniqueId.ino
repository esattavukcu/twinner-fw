uint16_t generateIdFromMac() {
    _Bluetooth.print("AT+ADDR?");
    delay(50);
    String macResponse = ""; 
    if (_Bluetooth.available()) {
         macResponse = _Bluetooth.readString();
    } else {
        //_USB.println("No response from Bluetooth module mac req.");
    }
    macResponse.trim();// clear \n  \r
    //_USB.println("MAC Address Response: " + macResponse);
    
    uint8_t addrIndex = macResponse.indexOf("OK+ADDR:");
    //_USB.println(addrIndex);
    if (addrIndex == -1) {
      //_USB.println("index not found");
        return 0xFFFF;
    }
    String address = macResponse.substring(addrIndex + 8);
    //address.replace(":", "");
    //_USB.println(address);

    uint16_t macSum = 0;
    for (uint8_t i = 0; i < address.length(); i += 2) {
        macSum += (uint16_t)strtol(address.substring(i, i + 2).c_str(), NULL, 16);
    }
      
    return (macSum & 0x01FF);
}

String mapIdToRgb(uint16_t id) {
    
 String colorNames[8] = {"R", "G", "B", "W", "O", "P", "Y", "T"};

  colorIndex1 = (id >> 6) & 0x07;
  colorIndex2 = (id >> 3) & 0x07;
  colorIndex3 = id & 0x07;

  twinRGB.setPixelColor(0, colors[colorIndex1][0], colors[colorIndex1][1], colors[colorIndex1][2]);
  twinRGB.setPixelColor(1, colors[colorIndex2][0], colors[colorIndex2][1], colors[colorIndex2][2]);
  twinRGB.setPixelColor(2, colors[colorIndex3][0], colors[colorIndex3][1], colors[colorIndex3][2]);
  twinRGB.show();

  
   return "Twin " + colorNames[colorIndex1] + colorNames[colorIndex2] + colorNames[colorIndex3];
}
