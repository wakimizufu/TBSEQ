#ifndef boradConst_h
#define boradConst_h

//I2Cアドレス
const char I2C_ADDR_SW  = 0x20; //MCP27017:マトリクススイッチ
const char I2C_ADDR_LED = 0x21; //MCP27017:LEDスイッチ
const char I2C_ADDR_EEPROM = 0x50; //24LC256

//GPIOピン
const int PIN_GATE	=	18;		//GATE
const int PIN_ACCENT	=	19;	//ACCENT
const int PIN_SLIDE	=	20;		//SLIDE
const int PIN_CV = 21; //CV(PWM)

#endif
