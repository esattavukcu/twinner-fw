#ifndef TWIN_h
#define TWIN_h


#include "Arduino.h"
#include <stdint.h>
#include <inttypes.h>
#include "../Adafruit_NeoPixel/Adafruit_NeoPixel.h"
#include <Servo.h>
#include "../dht11/dht.h"

// Firmware version 2.4
#define FIRMWARE_VERSION_MAJOR 2
#define FIRMWARE_VERSION_MINOR 4

/*
 * This protocol was created for little endian communicaiton.
 */
#define sync1	0xAA
#define sync2	0x44
#define sync3	0x1C

#define header_length	5
	// 1 Synchron Byte
	// 2 Synchron Byte
	// 3 Synchron Byte
	// 4 Message ID
	// 5 Message Length


Adafruit_NeoPixel 	twinRGB = Adafruit_NeoPixel(3, 7, NEO_GRB + NEO_KHZ800);

Servo				servo6;
Servo				servo9;
Servo				servo10;

typedef enum message_id_
{
	_digitalWrite 	= 1,
	_digitalRead	= 2,
	_analogWrite	= 3,
	_analogRead		= 4,
	_pianoButton	= 5,
	_servo 			= 11,
	_hcsr04 		= 12,
	_ws2812b 		= 13,
	_initTwin 		= 14,

	// Deprecated after version 2.0, use _playNote instead of this
	// _buzzerNote 	= 15,

	// Deprecated after version 2.0, use _playNote instead of this
	// _buzzerMusic 	= 16, 
	
	// Deprecated after version 2.0
	// _stopMusic 		= 17,

	_getFirmwareVersion = 20,
	_playNote = 21,
	_dcMotor = 22,
	_raspiComm = 23,
	_dht11 =24,
	_doubledc =25

}message_id;

typedef enum sent_package_steps_
{
	sync1_set = 0,
	sync2_set,
	sync3_set,
	message_id_set,
	message_length_set,
	message_data_set,
}sent_package_steps;

typedef enum received_package_steps_
{
	sync1_get = 0,
	sync2_get,
	sync3_get,
	message_id_get,
	message_length_get,
	message_data_get,
}received_package_steps;


typedef struct Data_st_
{
	uint8_t	  message_length_u8;
	uint8_t	  message_id_u8;
	uint8_t	  message_data_u8[20];
} Data_st;

typedef struct Channel_st_
{
	Data_st received;
	Data_st sent;
	int8_t	buffer_i8[50];
	int8_t	receivedLength_i8;
}Channel_st;

class TwinDCMotor : public Adafruit_NeoPixel  
{
public:
    #define DC_CW 1
    #define DC_CCW 0

    uint8_t num_serial_motors;

    /* Constructer for coding mode
    / red color output of WS2811 is connected to STM8s ADC1 channel 4. 
    / We are using Adafruit_NeoPixel to communicate with WS2811.
    / n: number of serial motors
    / p: dc motor digital pin
    */
    TwinDCMotor(uint8_t n, uint16_t p) : Adafruit_NeoPixel(n, p, NEO_RGB + NEO_KHZ800) 
    {
		this->num_serial_motors = n;
        begin();
        setBrightness(255);
        show();
    }

	~TwinDCMotor()
    {
        // begin();
        // setBrightness(255);
        // show();

		for (uint8_t mtr_id = 0; mtr_id < this->num_serial_motors; mtr_id++)
		{
			setDirAndSpeedUsingNeopixelLibrary(mtr_id, 0);
		}
    }

    void setDirAndSpeedCoding(uint8_t motorId, uint8_t dir, uint8_t speed)
    {
        if(speed > 100) {
            speed = 100;
        }
		
        
        // left-most bit indicates direction, the other seven speed
        uint8_t m_dir_speed = ((speed)&0x7f) | ((dir & 1) << 7);
        setDirAndSpeedUsingNeopixelLibrary(motorId, m_dir_speed);
    }

private:
    void setDirAndSpeedUsingNeopixelLibrary(uint8_t motorId, uint8_t redColor)
    {
        // try two times
        setPixelColor(motorId, 252 - redColor, 0, 0); 
        show();
        setPixelColor(motorId, 252 - redColor, 0, 0); 
        show();
    }
};

TwinDCMotor* dc_coding_at_pin_06_ptr = NULL;
TwinDCMotor* dc_coding_at_pin_09_ptr = NULL;
TwinDCMotor* dc_coding_at_pin_0A_ptr = NULL;

#endif

