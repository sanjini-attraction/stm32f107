/*
    Bluetooth IO

    게임을 진행하는데 필요한 데이터를 블루투스를 통해 전달
    - player_count, cur_game

    게임이 완료되고, 각 플레이어의 데이터를 블루투스로 전송
    - values
*/

/* -----------     Configure     ----------- */
void io_Configure();


/* ----------- Interrupt Handler ----------- */
void io_USART2_IRQHandler();


/* -----------     Functions     ----------- */
void sendMessage();