#include "LED.h"

const LED_Indication_st indicationOff = {{0},{1000},1};
const LED_Indication_st indicationOn = {{1},{1000},1};
const LED_Indication_st indicationSingleBlink = {{0,1,0},{250,75,1250},3};
const LED_Indication_st indicationDoubleBlink = {{0,1,0,1,0},{250,75,250,75,1250},5};
const LED_Indication_st indicationTripleBlink = {{0,1,0,1,0,1,0},{250,75,250,75,250,75,1250},7};

LED_Indication_st indications[] = {
	indicationOff,
	indicationOn,
	indicationSingleBlink,
	indicationDoubleBlink,
	indicationTripleBlink
};

void LED_Init(LED_st* _led, LED_Descriptor_st* _descriptor){
	_led->descriptor.setState = _descriptor->setState;
	_led->descriptor.getTick = _descriptor->getTick;
	_led->descriptor.indicationIsOverCallback = _descriptor->indicationIsOverCallback;

	_led->currentIndication = LED_INDICATION_OFF;
	_led->customIndication = NULL;
	
	_led->indicationRepeatCount = 1;
	_led->indicationRepeatCounter = 0;

	_led->indicationDescriptorCounter = 0;
	_led->currentTime = _led->descriptor.getTick();
}

void LED_SetIndication(LED_st* _led, LED_Indications_et _indication, uint8_t _repeatCount){
	_led->customIndication = NULL;
	_led->currentIndication = _indication;
	_led->indicationRepeatCount = _repeatCount;
	_led->indicationRepeatCounter = 0;
	_led->indicationDescriptorCounter = 0;
	_led->currentTime = _led->descriptor.getTick();
}

void LED_SetCustomIndication(LED_st* _led, LED_Indication_st* _customIndication, uint8_t _repeatCount){
	_led->customIndication = _customIndication;
	_led->currentIndication = LED_INDICATION_CUSTOM;
	_led->indicationRepeatCount = _repeatCount;
	_led->indicationRepeatCounter = 0;
	_led->indicationDescriptorCounter = 0;
	_led->currentTime = _led->descriptor.getTick();
}

LED_Indications_et LED_GetIndication(LED_st* _led){
	return _led->currentIndication;
}

void LED_Run(LED_st* _led){
	if(_led->customIndication == NULL){
		if(_led->indicationRepeatCounter < _led->indicationRepeatCount){
			_led->descriptor.setState(_led, indications[_led->currentIndication].states[_led->indicationDescriptorCounter]);
			if(_led->descriptor.getTick() - _led->currentTime > indications[_led->currentIndication].durations[_led->indicationDescriptorCounter]){
				if(_led->indicationDescriptorCounter < indications[_led->currentIndication].size - 1){
					_led->currentTime = _led->descriptor.getTick();
					_led->indicationDescriptorCounter++;
				}
				else{
					_led->indicationDescriptorCounter = 0;
					if(_led->indicationRepeatCount != 255)
						_led->indicationRepeatCounter++;

					if(_led->descriptor.indicationIsOverCallback != NULL)
						_led->descriptor.indicationIsOverCallback(_led, _led->currentIndication, _led->indicationRepeatCounter);
				}
			}
		}
	}
	else{
		if(_led->indicationRepeatCounter < _led->indicationRepeatCount){
			_led->descriptor.setState(_led, _led->customIndication->states[_led->indicationDescriptorCounter]);
			if(_led->descriptor.getTick() - _led->currentTime > _led->customIndication->durations[_led->indicationDescriptorCounter]){
				if(_led->indicationDescriptorCounter < _led->customIndication->size - 1){
					_led->currentTime = _led->descriptor.getTick();
					_led->indicationDescriptorCounter++;
				}
				else{
					_led->indicationDescriptorCounter = 0;
					if(_led->indicationRepeatCount != 255)
						_led->indicationRepeatCounter++;

					if(_led->descriptor.indicationIsOverCallback != NULL)
						_led->descriptor.indicationIsOverCallback(_led, _led->currentIndication, _led->indicationRepeatCounter);
				}
			}
		}
	}
}
