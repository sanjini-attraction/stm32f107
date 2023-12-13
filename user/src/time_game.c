#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

#include "time_game.h"
#include "common.h"

int time_count = 0;

/* -----------     Configure     ----------- */
void touch_exti_Configure(){
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
    EXTI_InitTypeDef touch = {
        .EXTI_Line = EXTI_Line0,
        .EXTI_Mode = EXTI_Mode_Interrupt,
        .EXTI_Trigger = EXTI_Trigger_Falling,
        .EXTI_LineCmd = ENABLE
    };
    
    EXTI_Init(&touch);
}
void touch_gpio_Configure(){
    GPIO_InitTypeDef touch = {
        .GPIO_Pin = GPIO_Pin_0,
        .GPIO_Mode = GPIO_Mode_IPU
    };
    GPIO_Init(GPIOA, &touch);
}
void touch_nvic_Configure(){
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitTypeDef touch = {
        .NVIC_IRQChannel = EXTI0_IRQn,
        .NVIC_IRQChannelPreemptionPriority = 0x0,
        .NVIC_IRQChannelSubPriority = 0x0,
        .NVIC_IRQChannelCmd = ENABLE
    };
    NVIC_Init(&touch);
}
void timer_Configure(){
    TIM_TimeBaseInitTypeDef timer2 = {
        // 1Hz = 72MHz / TIM_Prescaler / TIM_Period = 72MHz / 10k / 7200
        .TIM_Prescaler = (uint16_t) (SystemCoreClock / 10000)-1,
        .TIM_CounterMode = TIM_CounterMode_Up,
        .TIM_Period = 10000-1,
        .TIM_ClockDivision = 0,
    };
    TIM_TimeBaseInit(TIM2, &timer2);
    TIM_ARRPreloadConfig(TIM2, ENABLE);
}
void timer_interrupt_Configure(){
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}
void timer_nvic_Configure(){
    NVIC_InitTypeDef timer2 = {
        .NVIC_IRQChannel = TIM2_IRQn,
        .NVIC_IRQChannelPreemptionPriority = 0x0,
        .NVIC_IRQChannelSubPriority = 0x0,
        .NVIC_IRQChannelCmd = ENABLE
    };
    NVIC_Init(&timer2);
}

void timeGame_Configure(){
    // RCC Configure
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // KEY4(PA0)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // Touch Sensor Configure
    touch_gpio_Configure();
    touch_exti_Configure();
    touch_nvic_Configure();

    // Timer2 Configure
    timer_Configure();
    timer_interrupt_Configure();
    timer_nvic_Configure();
}


/* ----------- Interrupt Handler ----------- */
void timeGame_TimerHandler(){
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        if(game_state == 1) time_count++;
        // if (GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_2))
        //   GPIO_ResetBits(GPIOD, GPIO_Pin_2);
        // else GPIO_SetBits(GPIOD, GPIO_Pin_2);         
    }
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 
}

void timeGame_TouchHandler(){
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET) {
            if(game_state == 0){  // 턴 시작
                time_count = 0;
                TIM_Cmd(TIM2, ENABLE);
            }
            else{ // 턴 종료
                TIM_Cmd(TIM2, DISABLE);
                timeGame_turnEnd();
            }
            game_state = !game_state;
        }
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
} 

/* -----------    Game Control   ----------- */
void timeGame_turnEnd(){
    // 모든 플레이어의 턴 종료(게임 종료)
    if(cur_player+1 == player_count){
        allTurnEnd = 1;
    }
    else{
        values[cur_player] = time_count; // 기록 저장
        cur_player++;
    }
}