void applyCommands()
{  
  if (flagAnalyzeByte_b == true)
  {
    // Reset coming byte flag
    flagAnalyzeByte_b = false;
    
    uint8_t   message_ID    = channel_st.received.message_id_u8;
    flagGoingByte_u8[10]  = {0};  // Clean going bytes
    //_USB.println(message_ID);
    
    switch (message_ID)
    {
      case (uint8_t)_digitalWrite:
        digitalWrite_F();
        break;

      case (uint8_t)_digitalRead:
        digitalRead_F();
        break;

      case (uint8_t)_analogWrite:
        analogWrite_F();
        break;

      case (uint8_t)_analogRead:
        analogRead_F();
        break;

      case (uint8_t)_pianoButton:
        pianoButton_F();
        break;

      case (uint8_t)_servo:
        servo_F();
        break;
        
      case (uint8_t)_hcsr04:
        HCSR04_F();
        break;

      case (uint8_t)_ws2812b:
        WS2812B_F();
        break;

      case (uint8_t)_initTwin:
        initTwin();
        break;

      case (uint8_t) _getFirmwareVersion:
        getFirmwareVersion();
        break;

      case (uint8_t) _playNote:
        playNote();
        break;

      case (uint8_t) _dcMotor:
        dcMotor();
        break;
      case (uint8_t) _raspiComm:
        raspiComm();
        break; 
      case (uint8_t)_dht11:
        DHT11_F();
        break;
      case (uint8_t) _doubledc:
        double_dc_F();
        break; 
    } 
  }
}
 
