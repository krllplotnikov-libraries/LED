#ifndef LED_H_
#define LED_H_

#define LED_INDICATIONS_MAX_SIZE 16

#include <stdio.h>
#include <stdint.h>

typedef enum{
	CONTINIOUS_MODE,
	DISCONTINIOUS_MODE
}LedIndicationMode_et;

typedef enum{
	INDICATION_OFF,
	INDICATION_ON,
	INDICATION_BLINK,
	INDICATION_DOUBLEBLINK
}LedIndications_et;

typedef enum{
	LED_STATE_OFF,
	LED_STATE_ON
}LedState_et;

typedef uint32_t (*GetTick)();
typedef void (*SetState)(LedState_et state);

typedef struct{
	uint8_t states[LED_INDICATIONS_MAX_SIZE];
	uint32_t durations[LED_INDICATIONS_MAX_SIZE];
	uint8_t size;
}LedIndication_st;

typedef struct{
	SetState setState;
	GetTick getTick;
	LedIndications_et currentIndication;
	LedIndication_st* customIndication;
	uint8_t indicationMode;
	uint32_t indicationCounter;
	uint32_t currentTime;
}LED_st;

#ifdef __cplusplus
extern "C" {
#endif

void LED_Init(LED_st* led, SetState setState, GetTick getTick);
void LED_SetIndication(LED_st* led, LedIndications_et indication, LedIndicationMode_et mode);
void LED_SetCustomIndication(LED_st* led, LedIndication_st* descriptor, LedIndicationMode_et mode);
void LED_Run(LED_st* led);

#ifdef __cplusplus
}
#endif

#endif
