#include "LED.h"

const LedIndication_st indicationOff = {{0},{1},1};
const LedIndication_st indicationOn = {{1},{1},1};
const LedIndication_st indicationBlink = {{1,0},{100,1000},2};
const LedIndication_st indicationDoubleBlink = {{1,0,1,0},{100,100,100,1000},4};

LedIndication_st indications[] = {
	indicationOff,
	indicationOn,
	indicationBlink,
	indicationDoubleBlink
};

void LED_Init(LED_st* led, SetState setState, GetTick getTick){
	led->setState = setState;
	led->getTick = getTick;
	led->currentIndication = INDICATION_OFF;
	led->indicationMode = DISCONTINIOUS_MODE;
	led->indicationCounter = 0;
	led->currentTime = led->getTick();
	led->customIndication = NULL;
}

void LED_SetIndication(LED_st* led, LedIndications_et indication, LedIndicationMode_et mode){
	led->currentIndication = indication;
	led->indicationMode = mode;
	led->indicationCounter = 0;
	led->currentTime = led->getTick();
	led->customIndication = NULL;
}

void LED_SetCustomIndication(LED_st* led, LedIndication_st* descriptor, LedIndicationMode_et mode){
	led->customIndication = descriptor;
	led->indicationMode = mode;
	led->indicationCounter = 0;
	led->currentTime = led->getTick();
}

void LED_Run(LED_st* led){
	if(led->customIndication == NULL){
		if(led->indicationCounter >= indications[led->currentIndication].size){
			if(led->indicationMode == CONTINIOUS_MODE){
				led->indicationCounter = 0;
				led->currentTime = led->getTick();
			}
			else{
				return;
			}
		}

		led->setState(indications[led->currentIndication].states[led->indicationCounter]);

		if(led->getTick() - led->currentTime > indications[led->currentIndication].durations[led->indicationCounter]){
			led->indicationCounter++;
			led->currentTime = led->getTick();
		}
	}
	else{
		if(led->indicationCounter >= led->customIndication->size){
			if(led->indicationMode == CONTINIOUS_MODE){
				led->indicationCounter = 0;
				led->currentTime = led->getTick();
			}
			else{
				return;
			}
		}

		led->setState(led->customIndication->states[led->indicationCounter]);

		if(led->getTick() - led->currentTime > led->customIndication->durations[led->indicationCounter]){
			led->indicationCounter++;
			led->currentTime = led->getTick();
		}
	}
}
