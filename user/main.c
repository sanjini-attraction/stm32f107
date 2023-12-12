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
#include "some_game.h"
#include "common.h"
#include "game_io.h"

int values[PLAYER_MAX] = {0, }; // 각 플레이어의 데이터 저장

int player_count = 0;   // bluetooth로 받아올 총 플레이어 수
int cur_player = 0;     // 현재 플레이어의 index
int game_state = 0;     // 1(playing), 0(ready)

char allTurnEnd = 0;    // 모든 플레이어가 턴을 종료했는지 확인

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


// turnEnd button interrupt handler
void EXTI4_IRQHandler(){ turnButton_Handler(); }

// timeGame Interrupt
void EXTI0_IRQHandler(){ timeGame_TouchHandler(); }
void TIM2_IRQHandler(){ timeGame_TimerHandler(); }

// bluetooth & PuTTY Interrupt
void USART1_IRQHandler(){ io_USART1_IRQHandler(); }
void USART2_IRQHandler(){ io_USART2_IRQHandler();}

void Init(){
	SystemInit();
	turnButton_Button_Configure();

    io_Configure();
	timeGame_Configure();
	
	// 해당하는 게임별 사용하는 GPIO, EXTI, NVIC, DMA, Timer등을 설정
	someGame_Configure();
}

int main(){
  Init();

  while(1){
    if(allTurnEnd){
      // send_values();

      allTurnEnd = 0;
      cur_player = 0;
    }
  }
}