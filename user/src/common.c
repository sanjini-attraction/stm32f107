#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

#include "common.h"

int cur_game;       // 현재 게임의 index


void turnButton_gpio_Configure(){
     GPIO_InitTypeDef button = {
        .GPIO_Pin = GPIO_Pin_4,
        .GPIO_Mode = GPIO_Mode_IPU
    };
    GPIO_Init(GPIOC, &button);
}
void turnButton_exti_Configure(){
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4);
    EXTI_InitTypeDef button = {
        .EXTI_Line = EXTI_Line4,
        .EXTI_Mode = EXTI_Mode_Interrupt,
        .EXTI_Trigger = EXTI_Trigger_Falling,
        .EXTI_LineCmd = ENABLE
    };
    
    EXTI_Init(&button);
}
void turnButton_nvic_Configure(){
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitTypeDef button = {
        .NVIC_IRQChannel = EXTI4_IRQn,
        .NVIC_IRQChannelPreemptionPriority = 0x0,
        .NVIC_IRQChannelSubPriority = 0x0,
        .NVIC_IRQChannelCmd = ENABLE
    };
    NVIC_Init(&button);
}

void turnButton_Button_Configure(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // KEY1(PC4)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    turnButton_gpio_Configure();
    turnButton_exti_Configure();
    turnButton_nvic_Configure();
}

void turnButton_Handler(){
    // 현제 게임의 index에 따라, 해당 게임의 turnHandler를 호출
    switch(cur_game){
        case 0: someGame_turnHandler(); break;  // 예시
        // case 1: ????_turnHandler(); break;
        default: break;
    }
}

void turnEnd_Button_Configure(){}