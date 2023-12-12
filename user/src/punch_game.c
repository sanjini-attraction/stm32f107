#include "stm32f10x_adc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_dma.h"
#include "misc.h"

#include "common.h"
#include "common.c"
#include "punch_game.h"

#define MEM_OFFSET(x, y) (volatile unsigned int *) (x + y);

// volatile unsigned 32bits
volatile uint32_t ADC_Value;
int max_mappedValue = 0;
cur_player = 0;

/* -----------     Configure     ----------- */
unsigned int BASE_RCC = 0x40021000;
unsigned int BASE_PORT_C = 0x40011000;
unsigned int OFFSET_APB2_CLOCK = 0x00000018;
unsigned int OFFSET_PORT_CONF_LOW = 0x00;
unsigned int OFFSET_PORT_CONF_HIGH = 0x04;
unsigned int OFFSET_INPUT_DATA = 0x08;
unsigned int OFFSET_BIT_SET_RESET = 0x10;

void enable_ports() {
  volatile unsigned int *REG_APB2_CLOCK = MEM_OFFSET(BASE_RCC, OFFSET_APB2_CLOCK);
  *REG_APB2_CLOCK = 0x3C;
}

void set_input_output() {
  volatile unsigned int *REG_PORT_C_LOW = MEM_OFFSET(BASE_PORT_C, OFFSET_PORT_CONF_LOW);
  volatile unsigned int *REG_PORT_C_HIGH = MEM_OFFSET(BASE_PORT_C, OFFSET_PORT_CONF_HIGH);
  
  *REG_PORT_C_LOW = 0x44448444;
  *REG_PORT_C_HIGH = 0x44844444;
}

int is_button_pushed(int btn) {
  volatile unsigned int *PORT_C_INPUT = MEM_OFFSET(BASE_PORT_C, OFFSET_INPUT_DATA);
  
  if (btn == 1) {
    volatile unsigned int input = *PORT_C_INPUT;
    return (input & 0x10) != 0x10;
  }
  return 0;
}

void RccInit(void) {
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
}

void GpioInit(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void AdcInit(void) {
  ADC_InitTypeDef ADC_InitStructure;
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_239Cycles5);
  ADC_DMACmd(ADC1, ENABLE);
  ADC_Cmd(ADC1, ENABLE);
  ADC_ResetCalibration(ADC1);

  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void DMA_Configure(void) {
  DMA_InitTypeDef DMA_Instructure;

  DMA_Instructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
  DMA_Instructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_Value; 
  DMA_Instructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_Instructure.DMA_BufferSize = 1; 
  DMA_Instructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_Instructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_Instructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; 
  DMA_Instructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_Instructure.DMA_Mode = DMA_Mode_Circular; 
  DMA_Instructure.DMA_Priority = DMA_Priority_High;
  DMA_Instructure.DMA_M2M = DMA_M2M_Disable; 

  DMA_Init(DMA1_Channel1, &DMA_Instructure); 
  DMA_Cmd(DMA1_Channel1, ENABLE);
}

void punchGame_Configure(void) {
  SystemInit();
  RccInit();
  GpioInit();
  AdcInit();
  DMA_Configure();
}

/* -----------    Game Control   ----------- */
int mapping(int punch_value, int in_min, int in_max, int out_min, int out_max){
    return (punch_value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void punchGame_turnHandler(){
    if(game_state == 0){  // 이제 턴 시작 (ready)
    }
    else{  // 해당 플레이어의 턴 종료
        // 해당 플레이어의 값을 저장
        values[cur_player] = max_mappedValue;
        // 현재 플레이어의 값을 초기화
        max_mappedValue = 0;
        // 다음 플레이어로 넘어감
        cur_player++;
        // 모든 플레이어가 턴을 종료했는지 확인
        if(cur_player == player_count)
          allTurnEnd = 1;
    }
    // 게임 상태 변경
    game_state = !game_state;
}

void punchGame(){
  // 모든 플레이어가 턴을 종료했을 때 게임 종료
    if (allTurnEnd) {
      cur_player = 0;
      return 0;
    }
    // 게임 시작
    int punch_pressure = 0;
    game_state = 0;
    while(!allTurnEnd){   // 플레이어의 턴일 때만 로직을 실행
    // 인터럽트로 punchGame_turnHandler 호출
      // if (EXTI_GetITStatus(EXTI_Line4) != RESET){ // 대기 상태 확인
        // if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) == Bit_RESET){ // 눌러진 상태 확인
          // turnButton_Handler();
            while (game_state == 1){
              punch_pressure = ADC_Value;
              punch_pressure = punch_pressure > 400 ? 400 : (punch_pressure < 100) ? 100 : punch_pressure;
              int mappedValue = mapping(punch_pressure, 100, 400, 1, 100);
              if (mappedValue > max_mappedValue) max_mappedValue = mappedValue;
            }
          }
        // EXTI_ClearITPendingBit(EXTI_Line4);
    //   }
    // }
}
