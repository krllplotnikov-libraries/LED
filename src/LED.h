#ifndef LED_H_
#define LED_H_

#define LED_INDICATIONS_MAX_SIZE 16
#define LED_INDICATION_INFINITE_REPEAT 255

#include <stdio.h>
#include <stdint.h>

enum LedIndications{
	LED_INDICATION_OFF,
	LED_INDICATION_ON,
	LED_INDICATION_SINGLE_BLINK,
	LED_INDICATION_DOUBLE_BLINK,
	LED_INDICATION_TRIPLE_BLINK,
	LED_INDICATION_CUSTOM
};

enum LedState{
	LED_STATE_OFF,
	LED_STATE_ON
};

typedef enum LedIndications LedIndications_et;
typedef enum LedState LedState_et;
typedef enum LedState LedState_et;

typedef struct LedIndication LedIndication_st;
typedef struct LedUserFunctions LedUserFunctions_st;
typedef struct LED LED_st;

typedef void (*LedIndicationIsOverCallback)(LED_st* led, LedIndications_et indication, uint8_t repeatCounter);
typedef uint32_t (*LedGetTick)();
typedef void (*LedSetState)(LED_st* led, LedState_et state);

struct LedIndication{
	uint8_t states[LED_INDICATIONS_MAX_SIZE];
	uint32_t durations[LED_INDICATIONS_MAX_SIZE];
	uint8_t size;
};

struct LedUserFunctions{
	LedSetState setState;
	LedGetTick getTick;

	LedIndicationIsOverCallback indicationIsOverCallback;
};

struct LED{
	LedUserFunctions_st userFunctions;
	LedIndications_et currentIndication;
	LedIndication_st* customIndication;
	uint8_t indicationRepeatCount;
	uint8_t indicationRepeatCounter;
	uint8_t indicationDescriptorCounter;
	uint32_t currentTime;
};

#ifdef __cplusplus
extern "C" {
#endif

void LED_Init(LED_st* led, LedUserFunctions_st* userFunctions);
void LED_Run(LED_st* led);

void LED_SetIndication(LED_st* led, LedIndications_et indication, uint8_t repeatCount);
void LED_SetCustomIndication(LED_st* led, LedIndication_st* descriptor, uint8_t repeatCount);

LedIndications_et LED_GetIndication(LED_st* led);
LedIndication_st* LED_GetIndicationDescriptor(LED_st* led);

#ifdef __cplusplus
}
#endif

#endif
