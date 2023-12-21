#include "stm32f10x_adc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_dma.h"
#include "misc.h"

#include "common.h"
#include "punch_game.h"


/*
	ADC1: PB0
	DMA1: ADC1 -> ADC_Value

	max_mappedValue: 현재 진행중인 플레이어의 최대값
*/
volatile uint32_t ADC_Value;
int max_mappedValue = 0;

/* -----------     Configure     ----------- */
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
    if(game_state == 0){
        // 턴 시작
		printf("turn begin\n");
    }
    else{
        // 턴 종료
        printf("turn end / player %d = %d\n", cur_player, max_mappedValue);

        // 결과값 저장
        values[cur_player] = max_mappedValue;

        // 다음 player로 넘어감
        cur_player++;

        max_mappedValue = 0;

        // 모든 player가 게임을 끝낸 경우 state(allTurnEnd)를 바꿔줌
        if(cur_player == player_count)
			allTurnEnd = 1;
    }
}

void punchGame(){
    int mappedValue, punch_pressure = 0;
    printf("in punchGame\n");
    
	while(!allTurnEnd){
        // 플레이어의 턴일 때만 로직을 실행
		while(game_state == 1){

            punch_pressure = ADC_Value;
            printf("%d\n",ADC_Value);

            // 압력 센서가 많이 민감하기 때문에, 100~600 사이의 값만 활용
            punch_pressure = punch_pressure > 600 ? 600 : (punch_pressure < 100) ? 100 : punch_pressure;

            // 압력 센서의 값을 1~100 사이의 값으로 변환
            mappedValue = mapping(punch_pressure, 100, 600, 1, 100);

            if(mappedValue > max_mappedValue)
                max_mappedValue = mappedValue;
		}
	}
	printf("punchGame end\n");
}
