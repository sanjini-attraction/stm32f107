#include "stm32f10x_adc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "lcd.h"
#include "touch.h"

#include "time_game.h"
#include "main.h"

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

void TIM2_IRQHandler(){
	timeGame_TimerHandler();
}
void EXTI4_IRQHandler(){
	timeGame_TouchHandler();
}

void Init(){
  	SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	timeGame_Configure();
}

int main(){
  Init();

  while(1){
    if(allTurnEnd){
      send_values();

      allTurnEnd = 0;
      cur_player = 0;
    }
  }
}