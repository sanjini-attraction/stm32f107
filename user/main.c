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
#include "common.h"
#include "game_io.h"

int values[PLAYER_MAX] = {0, };
int is_data_received = 0;
int cur_game = 0;
int player_count = 3;;
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
    main: 결과 데이터(value) app으로 전송
  }    
*/

// turnEnd button interrupt handler
void EXTI4_IRQHandler(){ turnButton_Handler(); }

// timeGame Interrupt
void EXTI1_IRQHandler(){ timeGame_TouchHandler(); }
void TIM2_IRQHandler(){ timeGame_TimerHandler(); }

// bluetooth Interrupt
void USART2_IRQHandler(){ io_USART2_IRQHandler();}

void Init(){
	SystemInit();

  	// common configure(게임 플레이와 무관한 설정)
  	io_Configure();
	turnLed_Configure();
	turnButton_Button_Configure();
  
	// game configure(해당하는 게임별 사용하는 GPIO, EXTI, NVIC, DMA, Timer등을 설정)
	timeGame_Configure();
	punchGame_Configure();
	shakeGame_Configure();
}

int main(){
	Init();

	while(1){
		// bluetooth로 부터 받아올 데이터 초기화
		cur_game  = 0;
		is_data_received = 0;
		player_count = 0;

        // 게임 데이터를 모두 받아올 때까지 대기
		while(is_data_received != 2);
        printf("data received %d %d \n", player_count, cur_game);
        
		// 게임 데이터 초기화
		allTurnEnd = 0;
		cur_player = 0;
		game_state = 0;

        // 게임 진행
		switch(cur_game){
			case 0: shakeGame(); break;
			case 1: punchGame(); break;
			case 2: timeGame();	 break;
			default: 			 break;
		}

        // 게임 결과를 앱으로 전송
		sendMessage();
	}
}