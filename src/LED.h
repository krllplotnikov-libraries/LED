#ifndef LED_H_
#define LED_H_

#define LED_INDICATIONS_MAX_SIZE       16
#define LED_INDICATION_INFINITE_REPEAT 255

#include <stdio.h>
#include <stdint.h>

typedef enum LED_Indications LED_Indications_et;
typedef enum LED_State       LED_State_et;
typedef enum LED_State       LED_State_et;

typedef struct LED_Indication LED_Indication_st;
typedef struct LED_Descriptor LED_Descriptor_st;
typedef struct LED            LED_st;

typedef void     (*LED_IndicationIsOverCallback) (LED_st* _led, LED_Indications_et _indication, uint8_t _repeatCounter);
typedef uint32_t (*LED_GetTick)                  ();
typedef void     (*LED_SetState)                 (LED_st* _led, LED_State_et _state);

enum LED_Indications{
	LED_INDICATION_OFF,
	LED_INDICATION_ON,
	LED_INDICATION_SINGLE_BLINK,
	LED_INDICATION_DOUBLE_BLINK,
	LED_INDICATION_TRIPLE_BLINK,
	LED_INDICATION_CUSTOM
};

enum LED_State{
	LED_STATE_OFF,
	LED_STATE_ON
};

struct LED_Indication{
	uint8_t  states[LED_INDICATIONS_MAX_SIZE];
	uint32_t durations[LED_INDICATIONS_MAX_SIZE];
	uint8_t  size;
};

struct LED_Descriptor{
	LED_SetState setState;
	LED_GetTick  getTick;
	LED_IndicationIsOverCallback indicationIsOverCallback;
	void* context;
};

struct LED{
	LED_Descriptor_st descriptor;
	LED_Indications_et currentIndication;
	LED_Indication_st* customIndication;
	uint8_t indicationRepeatCount;
	uint8_t indicationRepeatCounter;
	uint8_t indicationDescriptorCounter;
	uint32_t currentTime;
};

#ifdef __cplusplus
extern "C" {
#endif

void LED_Init(LED_st* _led, LED_Descriptor_st* _descriptor);
void LED_Run(LED_st* _led);

void LED_SetIndication(LED_st* _led, LED_Indications_et _indication, uint8_t _repeatCount);
void LED_SetCustomIndication(LED_st* _led, LED_Indication_st* _descriptor, uint8_t _repeatCount);

LED_Indications_et LED_GetIndication(LED_st* _led);
LED_Indication_st* LED_GetIndicationDescriptor(LED_st* _led);

#ifdef __cplusplus
}
#endif

#endif
