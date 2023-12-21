#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "stm32f10x.h"

#include "common.h"
#include "shake_game.h"

/*
    Touch Sensor: PB10

    count: 턴 동안 센서에 의해 측정된 횟수
*/
int count;

/* -----------     Configure     ----------- */
void enable_ports() {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // KEY4(PB10)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

void set_input_output() {
    GPIO_InitTypeDef touch = {
        .GPIO_Pin = GPIO_Pin_10,
        .GPIO_Mode = GPIO_Mode_IPU
    };
    GPIO_Init(GPIOB, &touch);
}

void shakeGame_Configure(){
    enable_ports();
    set_input_output();
}

/* -----------    Game Control   ----------- */
int is_shaked(int btn) {
  	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == Bit_RESET;  // sensor input : PB10
}

void shakeGame_turnHandler(){
    if(game_state == 0){  // 턴 시작
        printf("turn begin\n");
		count = 0;
    }
    else{  // 턴 종료
        printf("turn end / %d player value = %d\n", cur_player, count);
        values[cur_player] = count;
        cur_player++;
        
        // 현재 플레이어의 값을 초기화
        count = 0;

        if(cur_player == player_count)
            allTurnEnd = 1;
    }
}

void shakeGame(){      
    printf("in shakeGame\n");
    while(!allTurnEnd){   // 플레이어의 턴일 때만 로직을 실행
		if(game_state == 1){
			if(is_shaked(2)) count++;
			if(count > 100) count = 0;
		}
	}
    printf("shakeGame end\n");
}