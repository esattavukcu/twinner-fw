#include "src/Twin/Twin.h"
#include <EEPROM.h> 
#include <SoftwareSerial.h>
SoftwareSerial *mySerial ; // Set up a new SoftwareSerial object for double dc motor

#define _Bluetooth  Serial1
#define _USB        Serial

#define DISCONNECTED  false
#define CONNECTED     true

const char outputpins[] = {6, 9, 10};
    
Channel_st  channel_st;

bool        flagBluetooth_b         = false;
bool        flagUSB_b               = false;
uint8_t     flagGoingByte_u8[2]     = {0};

uint8_t     bufferGoingByte_u8[20]  = {0};
uint8_t     current_read_status_u8  = sync1_get;
bool        flagAnalyzeByte_b       = false;
int32_t     temp_i32                = 0;

uint16_t deviceId = 0;
String deviceName = "";
String bleName = "";

bool bluetoothState = DISCONNECTED;
bool prebluetoothState = DISCONNECTED;    

const uint8_t ID_Address = 0; // device id
const uint8_t ID_flagAddress = 1;

// Color codes
uint8_t colors[8][3] = {
        {255, 0, 0},     // Red
        {0, 255, 0},     // Green
        {0, 0, 255},     // Blue
        {255, 255, 255}, // White
        {255, 120, 0},   // Orange
        {128, 0, 128},   // Purple
        {255, 255, 50},  // Yellow
        {64, 224, 208}   // Turquoise
};
    
uint8_t colorIndex1 = 0;
uint8_t colorIndex2 = 0;
uint8_t colorIndex3 = 0;
    
// Double dc motor variables
uint8_t motor_1_dir = 0;
uint8_t motor_2_dir = 0;
uint8_t motor_1_speed = 0;
uint8_t motor_2_speed = 0;



void setup() {
  _Bluetooth.begin(9600);
  _USB.begin(9600);
   delay(300); // Wait a bit  
   initTwin();
   bleSetup(); 
}

void loop() { 
    handleBluetoothState();
    readMessage();
    applyCommands();
    writeMessage();
}
