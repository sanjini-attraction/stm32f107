#include "stm32f10x.h"

#include "common.h"
#include "shake_game.h"

#define MEM_OFFSET(x, y) (volatile unsigned int *) (x + y);
int count;

/* -----------     Configure     ----------- */
void enable_ports() {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // KEY4(PB10)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// volatile unsigned int *REG_APB2_CLOCK = MEM_OFFSET(BASE_RCC, OFFSET_APB2_CLOCK);
	// *REG_APB2_CLOCK = 0x3C;
}

void set_input_output() {
    // GPIO_InitTypeDef GPIO_InitStructure;
    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    // GPIO_Init(GPIOB, &GPIO_InitStructure);
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
	// volatile unsigned int *PORT_B_INPUT = MEM_OFFSET(BASE_PORT_B, OFFSET_INPUT_DATA);    
	// if (btn == 2) {
	//   volatile unsigned int input = *PORT_B_INPUT;
	//   return (input & 0x400) != 0x400;
	// }  
	// return 0;
}

void shakeGame_turnHandler(){
    if(game_state == 0){  // 이제 턴 시작 (ready)
		count = 0;
    }
    else{  // 해당 플레이어의 턴 종료
        // 해당 플레이어의 값을 저장
        values[cur_player] = count;
        printf("values[%d] = %d\n", cur_player, count);
        // 현재 플레이어의 값을 초기화
        count = 0;
        // 다음 플레이어로 넘어감
        cur_player++;
        // 모든 플레이어가 턴을 종료했는지 확인
        if(cur_player == player_count)
          allTurnEnd = 1;
    }
}

void shakeGame(){      
    while(!allTurnEnd){   // 플레이어의 턴일 때만 로직을 실행
		if(game_state == 1){
			if(is_shaked(2)){
				count++;          
			}   
			if(count > 100){
				count = 0;
			}             
		}
	}
}



