// #include "stm3210x_???.h"
#include <stdint.h>

#include "some_game.h"
#include "common.h"

volatile uint32_t some_ADCValue;
int curUserValue = 0;

/* -----------     Configure     ----------- */
void someGame_Configure(){
    // 게임에서 사용하는 모든 GPIO, EXTI, NVIC, DMA, Timer등을 설정

    // sensor RCC Configure
    // sensor GPIO Configure

    // sensor DMA Configure
    // or
    // sensor EXTI Configure
    // sensor NVIC Configure
}

/* ----------- Interrupt Handler ----------- */
// 게임이 인터럽트 방식으로 동작할 때, 인터럽트 핸들러를 정의

/* -----------    Game Control   ----------- */
void someGame_turnHandler(){
    if(game_state == 0){  // 이제 턴 시작
    }
    else{  // 해당 플레이어의 턴 종료
        // 해당 플레이어의 값을 저장
        values[cur_player] = curUserValue;
        // 현재 플레이어의 값을 초기화
        curUserValue = 0;
        // 다음 플레이어로 넘어감
        cur_player++;

        // 모든 플레이어가 턴을 종료했는지 확인
        if(cur_player == player_count)
            allTurnEnd = 1;
    }

    // 게임 상태 변경
    game_state = !game_state;
}

void someGame(){
    // 게임 시작
    // 플레이어 수 초기화
    cur_player = 0;

    // 무한 루프를 돌며 polling방식으로 값을 확인
    while(1){
        // 플레이어의 턴일 때만 로직을 실행
        if(game_state == 1){
            // ADC 값 볌위 조절

            // 게임에 따라 ADC값의 Max나 Min값을 저장
            if(some_ADCValue > curUserValue)
                curUserValue = some_ADCValue;
        }

        // 모든 플레이어가 턴을 종료했을 때 게임 종료
        if(allTurnEnd) return;
    }
}