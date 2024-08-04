#ifndef boradConst_h
#define boradConst_h

//I2C
const int I2C_CLOCK = 400000; //クロック周波数(400Khz)
//const int I2C_CLOCK = 1000000; //クロック周波数(1Mhz)

const char I2C_ADDR_SW   = 0x20; //MCP27017:スイッチマトリクス
const char I2C_ADDR_LED  = 0x21; //MCP27017:LEDマトリクス
const char I2C_ADDR_SSD1306 = 0x38; //SSD1306:OLED LCD
const char I2C_ADDR_FRAM11  = 0x50; //FRAM:16KByte  スタートアドレス
const char I2C_ADDR_HT16K33 = 0x70; //HT16K33:LED&スイッチマトリクスドライバ

const char I2C_WIRE0_SDA = 4; //Wire.setSDA
const char I2C_WIRE0_SCL = 5; //Wire.setSCL

//GPIO PIN番号
const int PIN_GATE	=	18;		//GATE
const int PIN_ACCENT	=	19;	//ACCENT
const int PIN_SLIDE	=	20;		//SLIDE
const int PIN_CV = 21; //CV(PWM)
const int PIN_TEMPO_ADC = 26;   //TEMPO(ADC) 


#endif
