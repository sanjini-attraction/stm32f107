#include "stm32f10x.h"

#include "common.h"
#include "shake_game.h"

#define MEM_OFFSET(x, y) (volatile unsigned int *) (x + y);

/* -----------     Configure     ----------- */
unsigned int BASE_RCC = 0x40021000;
unsigned int BASE_PORT_B = 0x40010C00;
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
  volatile unsigned int *REG_PORT_B_LOW = MEM_OFFSET(BASE_PORT_B, OFFSET_PORT_CONF_LOW);   
  *REG_PORT_B_LOW = 0x44844444;  
}


/* -----------    Game Control   ----------- */

// sensor input : PB10
int is_shaked(int btn) {  
  volatile unsigned int *PORT_B_INPUT = MEM_OFFSET(BASE_PORT_B, OFFSET_INPUT_DATA);    
  if (btn == 2) {
    volatile unsigned int input = *PORT_B_INPUT;
    return (input & 0x400) != 0x400;
  }  
  return 0;
}

void shakeGame_Configure(){
    enable_ports();
    set_input_output();
}


int count;


void shakeGame_turnHandler(){
    if(game_state == 0){  // 이제 턴 시작 (ready)
		count = 0;
    }
    else{  // 해당 플레이어의 턴 종료
        // 해당 플레이어의 값을 저장
        values[cur_player] = count;
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



