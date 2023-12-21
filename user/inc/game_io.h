/*
    Bluetooth IO

    게임을 진행하는데 필요한 데이터를 블루투스를 통해 전달
    - player_count, cur_game

    게임이 완료되고, 각 플레이어의 데이터를 블루투스로 전송
    - values
*/

/* -----------     Configure     ----------- */
// rccConfigure, gpioConfigure, nvicConfigure, USART1,2 Init
void io_Configure();

/* ----------- Interrupt Handler ----------- */
// 게임 관련 정보 수신 (핸드폰 -> 컴퓨터)
void io_USART2_IRQHandler();

/* -----------     Functions     ----------- */
// 게임 결과 전송 (컴퓨터 -> 핸드폰)
void sendMessage();