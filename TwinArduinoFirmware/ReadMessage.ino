void readMessage()
{
  check_Message_From_Bluetooth();
  check_Message_From_USB();
}

void check_Message_From_Bluetooth() {
  static String statusBuf = "";

  // Only read Bluetooth when USB isn’t active
  if (_Bluetooth.available() > 0 && !flagUSB_b) {
    flagBluetooth_b = true;
    

    while (_Bluetooth.available()) {
      char b = _Bluetooth.read();
      

      statusBuf += b;
      if (statusBuf.length() > 16) {
        statusBuf = statusBuf.substring(statusBuf.length() - 16);
      }
      //_USB.println(statusBuf);

      if (statusBuf.indexOf("CONN") != -1) {
        if (bluetoothState != CONNECTED) {
          bluetoothState = CONNECTED;
         // _USB.println("Bluetooth Connected.");
        }
        statusBuf = "";
        continue;
      }

      if (statusBuf.indexOf("LOST")!= -1) {
        if (bluetoothState != DISCONNECTED) {
          bluetoothState = DISCONNECTED;
          //_USB.println("Bluetooth Disconnected.");
          
        }
        statusBuf = "";
        continue;
      }
      catchPackage((uint8_t)b);
    }

  } else {
    flagBluetooth_b = false;
  }
}




void check_Message_From_USB()
{
  if ((_USB.available() > 0) && (flagBluetooth_b == false))
  {
    flagUSB_b = true;

    catchPackage(_USB.read());
  }

  else
  {
    flagUSB_b = false;
  }
}

void catchPackage(uint8_t temp_u8)
{
  switch (current_read_status_u8)
  {
    case sync1_get:
      if (temp_u8 == 0xAA)
      {
        channel_st.receivedLength_i8++;
        current_read_status_u8 = (uint8_t)sync2_get;
      }
      else
      {
        current_read_status_u8 = (uint8_t)sync1_get;
        channel_st.receivedLength_i8 = 0;
      }

      //_USB.write(0x01);  // debug by printing case number
      break;

    case sync2_get:
      if (temp_u8 == 0x44)
      {
        channel_st.receivedLength_i8++;
        current_read_status_u8 = (uint8_t)sync3_get;
      }
      else
      {
        current_read_status_u8 = (uint8_t)sync1_get;
        channel_st.receivedLength_i8 = 0;
      }

      //_USB.write(0x02);  // debug by printing case number
      break;

    case sync3_get:
      if (temp_u8 == 0x1C)
      {
        channel_st.receivedLength_i8++;
        current_read_status_u8 = (uint8_t)message_id_get;
      }
      else
      {
        current_read_status_u8 = (uint8_t)sync1_get;
        channel_st.receivedLength_i8 = 0;
      }

      //_USB.write(0x03);  // debug by printing case number
      break;

    case message_id_get:
      // To complement the package
      channel_st.buffer_i8[0] = 0xAA;
      channel_st.buffer_i8[1] = 0x44;
      channel_st.buffer_i8[2] = 0x1C;

      // Need to check protocol existance

      channel_st.buffer_i8[channel_st.receivedLength_i8] = temp_u8;
      channel_st.receivedLength_i8++;
      current_read_status_u8 = (uint8_t)message_length_get;

      //_USB.write(0x04);  // debug by printing case number
      break;

    case message_length_get:
      channel_st.buffer_i8[channel_st.receivedLength_i8] = temp_u8;
      channel_st.receivedLength_i8++;
      current_read_status_u8 = (uint8_t)message_data_get;

      // Need to check size limits

      //_USB.write(0x05);  // debug by printing case number
      break;

    case message_data_get:
      channel_st.buffer_i8[channel_st.receivedLength_i8] = temp_u8;
      channel_st.receivedLength_i8++;

      if (channel_st.receivedLength_i8 == (channel_st.buffer_i8[4] + header_length))
      {
        // Gelen
        //_USB.write(0x98);
        /*for (int i = 3; i < 5; i++)
          _USB.write(channel_st.buffer_i8[i]);//*/

        analyzePackage();

        // Reset buffer timeout and values
        channel_st.receivedLength_i8 = 0;
        channel_st.buffer_i8[50] = {0};
        current_read_status_u8 = sync1_get;
      }

      //_USB.write(0x06);  // debug by printing case number
      break;
  }
}

void analyzePackage()
{
  int8_t current_step = sync1_get;
  int8_t byteNumber = 0;

  for (byteNumber = 0; byteNumber < channel_st.receivedLength_i8; byteNumber++)
  {
    switch (current_step)
    {
      case sync1_get:
        if (channel_st.buffer_i8[byteNumber] == (int8_t)sync1)
          current_step = sync2_get;
        break;

      case sync2_get:
        if (channel_st.buffer_i8[byteNumber] == (int8_t)sync2)
          current_step = sync3_get;
        break;

      case sync3_get:
        if (channel_st.buffer_i8[byteNumber] == (int8_t)sync3)
          current_step = message_id_get;
        break;

      case message_id_get:
        channel_st.received.message_id_u8 = (uint8_t)channel_st.buffer_i8[byteNumber];
        current_step = message_length_get;
        break;

      case message_length_get:
        channel_st.received.message_length_u8 = (uint8_t)channel_st.buffer_i8[byteNumber];

        current_step = message_data_get;
        break;

      case message_data_get:
        channel_st.received.message_data_u8[8] = {0};
        uint8_t index = 0;

        for (index = 0; index < channel_st.received.message_length_u8; index++)
        {
          channel_st.received.message_data_u8[index] = (uint8_t)channel_st.buffer_i8[byteNumber];
          ++byteNumber;
        }

        // the package is ready to apply
        flagAnalyzeByte_b = true;

        current_step = sync1_get;
        break;
    }
  }
}
