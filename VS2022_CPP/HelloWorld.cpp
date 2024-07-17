//arduino ideでコンパイルする際は以下をコメント解除する
//#define DEBUG_ARDUINO 0

#if defined(DEBUG_ARDUINO)
    #include <Wire.h>
#else
    #include <iostream>
#endif


#include "countTriger.h"
#include "midiClock.h"
#include "tempo.h"
#include "voltage.h"
#include "matrixLED.h"
#include "matrixSwitch.h"
#include "panelManager.h"
#include "sequenceMap.h"
#include "mode.h"
#include "paternPlay.h"

int main(int argc, char const *argv[])
{
  std::cout << "Hello World!" << std::endl;
  midiClock _midiClock(1000,0);
  tempo _tempo(0);
  panelManager _panelManager(0);
  voltage _voltage;
  sequenceMap _sequenceMap;

  presetBitstream _presetBitstream;
  _sequenceMap.setBitstream(_presetBitstream.patern_preset_bitstream);

  char tempBitStream[SEQUENCE_ALLBYTE];
  memset(tempBitStream, 0x00, SEQUENCE_ALLBYTE);
  _sequenceMap.getBitstream(tempBitStream);

  std::cout << "Finish preset Bitstream" << std::endl;


  //mode * _currentMode = new paternPlay( &_panelManager, &_voltage,  &_sequenceMap);
  //delete _currentMode;

  patern tempPatern =_sequenceMap.paterns[0];
  step tempStep =tempPatern.steps[2];
  tempStep.note=static_cast<int>(NOTE_PWM_INDEX::NOTE_A2);
  tempStep.note_on=true;
  tempStep.slide=true;
  tempPatern.steps[2]=tempStep;
  _sequenceMap.paterns[1]=tempPatern;

  _sequenceMap.paterns[0].steps[5].note=static_cast<int>(NOTE_PWM_INDEX::NOTE_CS3);
  _sequenceMap.paterns[0].steps[5].note_on=true;
  _sequenceMap.paterns[0].steps[5].acc=true;

  /*
  voltage _voltage;
  matrixLED _matrixLED;
  matrixSwitch _matrixSwitch;

  _voltage.cv(static_cast<int>(NOTE_PWM_INDEX::NOTE_C2));

  char tempRow;
  bool tempLED;
  tempRow=0x7E;
  _matrixLED.setRow(LED_ROW_0,tempRow);
  _matrixLED.set(static_cast<int>(LED::C),true);
  _matrixLED.set(static_cast<int>(LED::G),true);
  _matrixLED.set(static_cast<int>(LED::DSHARP),false);
  tempRow=_matrixLED.getRow(LED_ROW_0);
  tempLED=_matrixLED.get(static_cast<int>(LED::G));

  tempRow=0xA7;

  for (int scanCnt = static_cast<int>(SwitchScan::Scan1st) ; scanCnt < static_cast<int>(SwitchScan::Finalize) ; scanCnt++){
    _matrixSwitch.setRow( scanCnt ,SW_ROW_0,tempRow);
    _matrixSwitch.nextScan();
    _matrixSwitch.finalize();
  }

  tempRow=_matrixSwitch.getRow(SW_ROW_0);

  bool tempSwitch;
  tempSwitch =_matrixSwitch.get(static_cast<int>(Switch::ASHARP));
*/

  for (int i = 0; i < 5000; i++) {
    //std::cout << "Sample For Loop : " << i << std::endl;
    _midiClock.countUp();
    _tempo.countUp();
    _panelManager.countUp();
    
    if(_midiClock.getCountUp()){
      //std::cout << "midiClock CountUp : " << _midiClock.getCountUp() << std::endl;
      _midiClock.clear();

      //std::cout << "tempo info : TEMPO:" << _tempo.getTempo() << " CountThd:" <<  _tempo.getCountThd() << std::endl;
    }
  
    if(_panelManager.getSequenceUp()){
      std::cout << "panelManager SequenceUp : " << _panelManager.getSequenceUp() << std::endl;
      _panelManager.clear();
    }

  }

  return 0;
}