#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

#include "punch_game.h"
#include "time_game.h"
#include "shake_game.h"
#include "common.h"

/*
    C4 button(key1)을 turnButton으로 사용
    D2 led를 turnLed로 사용
*/
int led_state;


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
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    turnButton_gpio_Configure();
    turnButton_exti_Configure();
    turnButton_nvic_Configure();
}
void turnLed_Configure(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitTypeDef led1 = {
        .GPIO_Pin = GPIO_Pin_2,
        .GPIO_Mode = GPIO_Mode_Out_PP
    };
    GPIO_Init(GPIOD, &led1);

    led_state = 0;
}

void turnButton_Handler(){
    if (EXTI_GetITStatus(EXTI_Line4) != RESET) {
        if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) == Bit_RESET) {
            // 현재 게임의 index에 따라, 해당 게임의 turnHandler를 호출
            switch(cur_game){
                case 0: shakeGame_turnHandler(); break;
                case 1: punchGame_turnHandler(); break;
                case 2: timeGame_turnHandler();  break;
                default: break;
            }
            
            // 게임 상태 변경
            game_state = !game_state;

            if(led_state) GPIO_ResetBits(GPIOD, GPIO_Pin_2);
            else GPIO_SetBits(GPIOD, GPIO_Pin_2);
            led_state = !led_state;
        }
        EXTI_ClearITPendingBit(EXTI_Line4);
    }
}