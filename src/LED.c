#include "LED.h"

const LedIndication_st indicationOff = {{0},{1000},1};
const LedIndication_st indicationOn = {{1},{1000},1};
const LedIndication_st indicationSingleBlink = {{0,1,0},{250,75,1250},3};
const LedIndication_st indicationDoubleBlink = {{0,1,0,1,0},{250,75,250,75,1250},5};
const LedIndication_st indicationTripleBlink = {{0,1,0,1,0,1,0},{250,75,250,75,250,75,1250},7};

LedIndication_st indications[] = {
	indicationOff,
	indicationOn,
	indicationSingleBlink,
	indicationDoubleBlink,
	indicationTripleBlink
};

void LED_Init(LED_st* led, LedUserFunctions_st* userFunctions){
	led->userFunctions.setState = userFunctions->setState;
	led->userFunctions.getTick = userFunctions->getTick;
	led->userFunctions.indicationIsOverCallback = userFunctions->indicationIsOverCallback;

	led->currentIndication = LED_INDICATION_OFF;
	led->customIndication = NULL;
	
	led->indicationRepeatCount = 1;
	led->indicationRepeatCounter = 0;

	led->indicationDescriptorCounter = 0;
	led->currentTime = led->userFunctions.getTick();
}

void LED_SetIndication(LED_st* led, LedIndications_et indication, uint8_t repeatCount){
	led->customIndication = NULL;
	led->currentIndication = indication;
	led->indicationRepeatCount = repeatCount;
	led->indicationRepeatCounter = 0;
	led->indicationDescriptorCounter = 0;
	led->currentTime = led->userFunctions.getTick();
}

void LED_SetCustomIndication(LED_st* led, LedIndication_st* descriptor, uint8_t repeatCount){
	led->customIndication = descriptor;
	led->currentIndication = LED_INDICATION_CUSTOM;
	led->indicationRepeatCount = repeatCount;
	led->indicationRepeatCounter = 0;
	led->indicationDescriptorCounter = 0;
	led->currentTime = led->userFunctions.getTick();
}

LedIndications_et LED_GetIndication(LED_st* led){
	return led->currentIndication;
}

void LED_Run(LED_st* led){
	if(led->customIndication == NULL){
		if(led->indicationRepeatCounter < led->indicationRepeatCount){
			led->userFunctions.setState(led, indications[led->currentIndication].states[led->indicationDescriptorCounter]);
			if(led->userFunctions.getTick() - led->currentTime > indications[led->currentIndication].durations[led->indicationDescriptorCounter]){
				if(led->indicationDescriptorCounter < indications[led->currentIndication].size){
					led->currentTime = led->userFunctions.getTick();
					led->indicationDescriptorCounter++;
				}
				else{
					led->indicationDescriptorCounter = 0;
					if(led->indicationRepeatCount != 255)
						led->indicationRepeatCounter++;

					if(led->userFunctions.indicationIsOverCallback != NULL)
						led->userFunctions.indicationIsOverCallback(led, led->currentIndication, led->indicationRepeatCounter);
				}
			}
		}
	}
	else{
		if(led->indicationRepeatCounter < led->indicationRepeatCount){
			led->userFunctions.setState(led, led->customIndication->states[led->indicationDescriptorCounter]);
			if(led->userFunctions.getTick() - led->currentTime > led->customIndication->durations[led->indicationDescriptorCounter]){
				if(led->indicationDescriptorCounter < led->customIndication->size){
					led->currentTime = led->userFunctions.getTick();
					led->indicationDescriptorCounter++;
				}
				else{
					led->indicationDescriptorCounter = 0;
					if(led->indicationRepeatCount != 255)
						led->indicationRepeatCounter++;

					if(led->userFunctions.indicationIsOverCallback != NULL)
						led->userFunctions.indicationIsOverCallback(led, led->currentIndication, led->indicationRepeatCounter);
				}
			}
		}
	}
}
