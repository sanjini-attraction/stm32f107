#include "stm32f10x_adc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"
#include "misc.h"

#include "shake_game.h"
#include "punch_game.h"
#include "time_game.h"
#include "some_game.h"
#include "common.h"
#include "game_io.h"

int values[PLAYER_MAX] = {0, }; // 각 플레이어의 데이터 저장

int cur_game = 0;
int player_count = 3
int cur_player = 0;
int game_state = 0;
int allTurnEnd = 0;

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

  // configure
  io_Configure();
	turnButton_Button_Configure();
  
  	// game configure(해당하는 게임별 사용하는 GPIO, EXTI, NVIC, DMA, Timer등을 설정)
	// someGame_Configure();  // 예시
	timeGame_Configure();
  punchGame_Configure();
  shakeGame_Configure();
}

int main(){
  Init();

	while(1){
		// 게임 데이터를 받아올 때까지 대기
		// while(getGameData);

		// 게임 데이터 초기화
		allTurnEnd = 0;
		cur_player = 0;
		game_state = 0;

		switch(cur_game){
			// case 0: someGame();	 break;  // 예시
			case 1: punchGame(); break;
			case 2: timeGame();	 break;
			// case 3: 
			default: 			 break;
		}
		
		// send_values();
	}
}