#ifndef boradConst_h
#define boradConst_h

//I2C
const char I2C_ADDR_SW  = 0x20; //MCP27017:スイッチマトリクス
const char I2C_ADDR_LED = 0x21; //MCP27017:LEDマトリクス
const char I2C_ADDR_EEPROM = 0x50; //24LC256

const char I2C_WIRE0_SDA = 0; //Wire.setSDA
const char I2C_WIRE0_SCL = 1; //Wire.setSCL

//GPIO PIN番号
const int PIN_GATE	=	18;		//GATE
const int PIN_ACCENT	=	19;	//ACCENT
const int PIN_SLIDE	=	20;		//SLIDE
const int PIN_CV = 21; //CV(PWM)

#endif
