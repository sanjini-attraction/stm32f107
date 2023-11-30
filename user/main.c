#include "stm32f10x_adc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_tim.h"
#include "misc.h"
#include "lcd.h"
#include "touch.h"

/*
  main: flaot value[PLAYER_MAX] = {};
  main: 휴대폰이랑 연결
  while{
    app: 인원선택
    app: 게임 선택
    app: 목표 선택
    app: 보드에 전송(인원, 게임)

    main: 게임 선택, 인원 설정
    game_?: 게임 플레이()
    game_?: main으로 리턴
    main: value데이터 app으로 전송
  }    
*/
/*
  1. 펀치: polling으로 측정해서 max
  2. 굴리기: 턴 종료시 거리 측정
  3. 시간 맞추기: 터치로 알아서
  4. 앱: 알아서
  5. main
*/
#define PLAYER_MAX 10
int values[PLAYER_MAX] = {};

void RCC_Configure() {
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
}
void TouchSensor_Configure(){
  GPIO_InitTypeDef touch1;
  touch1.GPIO_Pin = GPIO_Pin_4;
  touch1.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &touch1);
}

void EXTI_Configure(){
  EXTI_InitTypeDef touch1 = {
    .EXTI_Line = EXTI_Line4,
    .EXTI_Mode = EXTI_Mode_Interrupt,
    .EXTI_Trigger = EXTI_Trigger_Falling,
    .EXTI_LineCmd = ENABLE
  };
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4);
  EXTI_Init(&touch1);
}

void NVIC_Configure(){
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

  NVIC_InitTypeDef touch1 = {
    .NVIC_IRQChannel = EXTI4_IRQn,
    .NVIC_IRQChannelPreemptionPriority = 0x0,
    .NVIC_IRQChannelSubPriority = 0x0,
    .NVIC_IRQChannelCmd = ENABLE
  };
  NVIC_Init(&touch1);
}

void EXTI4_IRQHandler() {
  if (EXTI_GetITStatus(EXTI_Line4) != RESET) {
    if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) == Bit_RESET) {
      // Todo
    }
    EXTI_ClearITPendingBit(EXTI_Line4);
  }
}

void Init() {
  SystemInit();
  RCC_Configure();

  // GPIO_Configure
  TouchSensor_Configure();

  EXTI_Configure();
  NVIC_Configure();
}

int main(){
  Init();

  while(1){
    // Todo
  }
}