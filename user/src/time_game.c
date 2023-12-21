#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

#include "time_game.h"
#include "common.h"

/*
    Touch Sensor: PA1
    Timer2: 10Hz
*/
int time_count, timer_running;

/* -----------     Configure     ----------- */
void touch_exti_Configure(){
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
    EXTI_InitTypeDef touch = {
        .EXTI_Line = EXTI_Line1,
        .EXTI_Mode = EXTI_Mode_Interrupt,
        .EXTI_Trigger = EXTI_Trigger_Falling,
        .EXTI_LineCmd = ENABLE
    };
    
    EXTI_Init(&touch);
}
void touch_gpio_Configure(){
    GPIO_InitTypeDef touch = {
        .GPIO_Pin = GPIO_Pin_1,
        .GPIO_Mode = GPIO_Mode_IPU
    };
    GPIO_Init(GPIOA, &touch);
}
void touch_nvic_Configure(){
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitTypeDef touch = {
        .NVIC_IRQChannel = EXTI1_IRQn,
        .NVIC_IRQChannelPreemptionPriority = 0x0,
        .NVIC_IRQChannelSubPriority = 0x0,
        .NVIC_IRQChannelCmd = ENABLE
    };
    NVIC_Init(&touch);
}
void timer_Configure(){
    TIM_TimeBaseInitTypeDef timer2 = {
        // 1Hz = 72MHz / TIM_Prescaler / TIM_Period = 72MHz / 10k / 7200
        .TIM_Prescaler = (uint16_t) (SystemCoreClock / 100000)-1,
        .TIM_CounterMode = TIM_CounterMode_Up,
        .TIM_Period = 10000-1,
        .TIM_ClockDivision = 0,
    };
    TIM_TimeBaseInit(TIM2, &timer2);
    TIM_ARRPreloadConfig(TIM2, ENABLE);
}
void timer_interrupt_Configure(){
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, DISABLE);
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
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
        time_count++;

    printf("timer = %d\n", time_count);
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 
}

void timeGame_TouchHandler(){
    if (EXTI_GetITStatus(EXTI_Line1) != RESET) {
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == Bit_RESET) {
            if(cur_game == 2){
                // 계속 timer interrupt가 발생할 필요 X
                // Touch Sensor가 눌렸을 때, timer 상태에 따라 timer를 시작하거나 멈춤
                if(timer_running == 0)
                    TIM_Cmd(TIM2, ENABLE);
                else TIM_Cmd(TIM2, DISABLE);

                timer_running = !timer_running;
            }
        }
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
} 

/* -----------    Game Control   ----------- */
void timeGame_turnHandler(){
    if(game_state == 0){
        // 턴 시작
        printf("turn begin\n");
        time_count = 0;
        timer_running = 0;
        TIM_Cmd(TIM2, DISABLE);
    }
    else{
        // 턴 종료
        printf("turn end / %d player time = %d\n", cur_player, time_count);

        // 결과값 저장
        values[cur_player] = time_count;

        // 다음 player로 넘어감
        cur_player++;

        // 모든 player가 게임을 끝낸 경우 state(allTurnEnd)를 바꿔줌
        if(cur_player == player_count)
            allTurnEnd = 1;        
    }
}
void timeGame(){
    // 모든 턴이 끝날때까지 반복
    printf("in timeGame\n");
    while(!allTurnEnd);
    printf("timeGame ended\n");
}