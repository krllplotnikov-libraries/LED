# LED library

## Содержание
1. [Описание](#Описание)
2. [Особенности](#Особенности)
3. [Типы данных](#Типы-данных)
4. [Использование](#Использование)
5. [Примеры](#Примеры)
____

## Описание
Библиотека для управления индикацией светодиодов
____

## Особенности
- Аппаратно независимая реализация
- Обработка события окончания индикации через callback функцию
- Возможность создавать свои виды индикации
____

## Типы данных
```C
//структура для создания экземпляра светодиода
struct LED_st;

//структура для определения пользовательских функций
struct LedUserFunctions_st;

//структура описавающая индикацию
struct LedIndication_st;

//перечесление состояний светодиода
enum LedState_et; 

//перечесление стандартных видов индикации
enum LedIndications_et; 

//указатель на функцию обратного вызова на событие окончания индикации
//LED_st* led - указатель на экземпляр структуры светодиода
//LedIndications_et indication - текущая индикация
//uint8_t repeatCounter - количество повторений индикации
void (*LedIndicationIsOverCallback) (LED_st* led, LedIndications_et indication, uint8_t repeatCounter);

//указатель на функцию для установки состояния светодиода
//LED_st* led - указатель на экземпляр структуры светодиода
//LedState_et state - состояние светодиода
void (*LedSetState) (LED_st* led, LedState_et state);

//указатель на функцию для получения времени в миллисекундах с момента запуска
uint32_t (*LedGetTick) (); 
```
____

## Использование
1) Добавить файлы LED.h и LED.c в проект
2) Подключить файл LED.h
```C
#include "LED.h"
```
3) Создать экземпляр структуры LED_st
```C
LED_st ledLeft = {0};
```
4) Определить три функции: получение времени в миллисекундах с момента запуска; установка состояния светодиода; обратный вызов на событие окончания индикации
```C
uint32_t GetTick_Led(){
    //Необходимо вернуть значение времени в миллисекундах
    return esp_timer_get_time() / 1000;
}

void SetState_Led(LED_st* led, LedState_et state){
	if(led == &ledLeft){
        //Необходимо установить состояния пина, в соответствии с управляющем логическим уровнем на пине
		if(state == LED_STATE_OFF) gpio_set_level(LED_LEFT_GPIO_NUM, 1);	
		else                       gpio_set_level(LED_LEFT_GPIO_NUM, 0);
	}
}

void IndicationIsOverCallback_Led(LED_st* led, LedIndications_et indication, uint8_t repeatCounter){
	if(led == &ledLeft){
		if(indication == LED_INDICATION_SINGLE_BLINK && repeatCounter == 5){
            //Закончилось индикация одинарного моргания
		}
		else if(indication == LED_INDICATION_TRIPLE_BLINK && repeatCounter == 2){
			//Закончилось индикация тройного моргания
		}
	}
}
```
5) Создать экземпляр структуры LedUserFunctions_st и присвоить ранее объявленные функции
```C
LedUserFunctions_st ledUserFunctions = {
		.getTick = GetTick_Led,
		.setState = SetState_Led,
		.indicationIsOverCallback = IndicationIsOverCallback_Led
};
```
6) Вызвать функцию инициализации светодиода LED_Init(), аргументами передать ссылку на экземпляр структуры светодиода и ссылку на структуру с указателями на функции
```C
void main(){
    LED_Init(&ledLeft, &ledUserFunctions);
}
```
7) В цикле вызывать функцию LED_Run(), аргументами передать ссылку на экземпляр структуры светодиода
```C
while(1){
    LED_Run(&ledLeft);
}
```
8) По необходимости IndicationIsOverCallback_Led() обрабатывать полученные события

**ВАЖНО!**
- LedIndicationIsOverCallback может равняться NULL, тогда обратный вызов окончания индикации вызываться не будет 
____

## Примеры
- STM32
```C
#include "LED.h"

uint32_t GetTick_Led();
void SetState_Led(LED_st* led, LedState_et state);
void IndicationIsOverCallback_Led(LED_st* led, LedIndications_et indication, uint8_t repeatCounter);

LED_st ledLeft = {0};
LED_st ledRight = {0};
LedUserFunctions_st ledUserFunctions = {
		.getTick = GetTick_Led,
		.setState = SetState_Led,
		.indicationIsOverCallback = IndicationIsOverCallback_Led
};

LedIndication_st indicationCustom = {{1, 0, 1, 0, 1, 0, 1, 0, 1, 0},{50, 50, 150, 150, 250, 250, 350, 350, 450, 450},10};

uint32_t GetTick_Led(){
	return HAL_GetTick();
}

void SetState_Led(LED_st* led, LedState_et state){
	if(led == &ledLeft){
		if(state == LED_STATE_OFF) HAL_GPIO_WritePin(LED_LEFT_GPIO_Port, LED_LEFT_Pin, GPIO_PIN_SET);
		else                       HAL_GPIO_WritePin(LED_LEFT_GPIO_Port, LED_LEFT_Pin, GPIO_PIN_RESET);
	}
	else if(led == &ledRight){
		if(state == LED_STATE_OFF) HAL_GPIO_WritePin(LED_RIGHT_GPIO_Port, LED_RIGHT_Pin, GPIO_PIN_SET);
		else                       HAL_GPIO_WritePin(LED_RIGHT_GPIO_Port, LED_RIGHT_Pin, GPIO_PIN_RESET);
	}
}

void IndicationIsOverCallback_Led(LED_st* led, LedIndications_et indication, uint8_t repeatCounter){
	if(led == &ledLeft){
		if(indication == LED_INDICATION_SINGLE_BLINK && repeatCounter == 5){
			LED_SetIndication(led, LED_INDICATION_TRIPLE_BLINK, 2);
		}
		else if(indication == LED_INDICATION_TRIPLE_BLINK && repeatCounter == 2){
			LED_SetCustomIndication(led, &indicationCustom, 1);
		}
		else if(indication == LED_INDICATION_CUSTOM && repeatCounter == 1){
			LED_SetIndication(led, LED_INDICATION_SINGLE_BLINK, 5);
		}
	}

	else if(led == &ledRight){
		if(indication == LED_INDICATION_CUSTOM && repeatCounter == 6){
			LED_SetIndication(led, LED_INDICATION_SINGLE_BLINK, 5);
		}
		else if(indication == LED_INDICATION_SINGLE_BLINK && repeatCounter == 5){
			LED_SetCustomIndication(led, &indicationCustom, 6);
		}
	}
}

void main(){
	LED_Init(&ledLeft, &ledUserFunctions);
	LED_Init(&ledRight, &ledUserFunctions);

	LED_SetIndication(&ledLeft, LED_INDICATION_SINGLE_BLINK, 5);
	LED_SetCustomIndication(&ledRight, &indicationCustom, 6);

	while(1){
		LED_Run(&ledLeft);
		LED_Run(&ledRight);
	}
}
```

- ESP32
```C
#include "driver/gpio.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"

#include "LED.h"

#define LED_LEFT_GPIO_NUM GPIO_NUM_8
#define LED_RIGHT_GPIO_NUM GPIO_NUM_7

gpio_config_t led_gpio_conf = {
  .mode = GPIO_MODE_OUTPUT,
  .pin_bit_mask = ((1ULL << LED_LEFT_GPIO_NUM) | (1ULL << LED_RIGHT_GPIO_NUM))
};

uint32_t GetTick_Led();
void SetState_Led(LED_st* led, LedState_et state);
void IndicationIsOverCallback_Led(LED_st* led, LedIndications_et indication, uint8_t repeatCounter);

LED_st ledLeft = {0};
LED_st ledRight = {0};
LedUserFunctions_st ledUserFunctions = {
		.getTick = GetTick_Led,
		.setState = SetState_Led,
		.indicationIsOverCallback = IndicationIsOverCallback_Led
};

LedIndication_st indicationCustom = {{1, 0, 1, 0, 1, 0, 1, 0, 1, 0},{50, 50, 150, 150, 250, 250, 350, 350, 450, 450},10};

uint32_t GetTick_Led(){
	return esp_timer_get_time() / 1000;
}

void SetState_Led(LED_st* led, LedState_et state){
	if(led == &ledLeft){
		if(state == LED_STATE_OFF) gpio_set_level(LED_LEFT_GPIO_NUM, 1);	
		else                       gpio_set_level(LED_LEFT_GPIO_NUM, 0);
	}
	else if(led == &ledRight){
		if(state == LED_STATE_OFF) gpio_set_level(LED_LEFT_GPIO_NUM, 1);
		else                       gpio_set_level(LED_LEFT_GPIO_NUM, 0);
	}
}

void IndicationIsOverCallback_Led(LED_st* led, LedIndications_et indication, uint8_t repeatCounter){
	if(led == &ledLeft){
		if(indication == LED_INDICATION_SINGLE_BLINK && repeatCounter == 5){
			LED_SetIndication(led, LED_INDICATION_TRIPLE_BLINK, 2);
		}
		else if(indication == LED_INDICATION_TRIPLE_BLINK && repeatCounter == 2){
			LED_SetCustomIndication(led, &indicationCustom, 1);
		}
		else if(indication == LED_INDICATION_CUSTOM && repeatCounter == 1){
			LED_SetIndication(led, LED_INDICATION_SINGLE_BLINK, 5);
		}
	}

	else if(led == &ledRight){
		if(indication == LED_INDICATION_CUSTOM && repeatCounter == 6){
			LED_SetIndication(led, LED_INDICATION_SINGLE_BLINK, 5);
		}
		else if(indication == LED_INDICATION_SINGLE_BLINK && repeatCounter == 5){
			LED_SetCustomIndication(led, &indicationCustom, 6);
		}
	}	
}

void LED_Task(){
	while(1){
		LED_Run(&ledLeft);
		LED_Run(&ledRight);
		vTaskDelay(pdMS_TO_TICKS(5));
	}
}

void app_main(void)
{
	gpio_config(&led_gpio_conf);
	
	LED_Init(&ledLeft, &ledUserFunctions);
	LED_Init(&ledRight, &ledUserFunctions);
	
	LED_SetIndication(&ledLeft, LED_INDICATION_SINGLE_BLINK, 5);
	LED_SetCustomIndication(&ledRight, &indicationCustom, 6);
	
	xTaskCreate(LED_Task, NULL, 1024, NULL, 0, NULL);
}
```
____