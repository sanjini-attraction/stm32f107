#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

#include "common.h"
#include "common.h"
#include "game_io.h"

// 핸드폰으로부터 받아온 값 1차 저장 (char 저장)
uint16_t io_char[100] = {0, };

// 이때까지 받아온 값이 몇 개인지
int io_index = 0;

/* -----------  Configure ---------- */
void io_rcc_Configure(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}
void io_gpio_Configure(){
    GPIO_InitTypeDef GPIO_InitStructure;

    //TX
    GPIO_InitTypeDef USART1_TX;
    USART1_TX.GPIO_Pin = GPIO_Pin_9;
    USART1_TX.GPIO_Speed = GPIO_Speed_50MHz;
    USART1_TX.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &USART1_TX);

    //RX
    GPIO_InitTypeDef USART1_RX;
    USART1_RX.GPIO_Pin = GPIO_Pin_10;
    USART1_RX.GPIO_Speed = GPIO_Speed_50MHz;
    USART1_RX.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &USART1_RX);

    /* USART2 pin setting */
    //TX
    GPIO_InitTypeDef USART2_TX;
    USART2_TX.GPIO_Pin = GPIO_Pin_2;
    USART2_TX.GPIO_Speed = GPIO_Speed_50MHz;
    USART2_TX.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &USART2_TX);

    //RX
    GPIO_InitTypeDef USART2_RX;
    USART2_RX.GPIO_Pin = GPIO_Pin_3;
    USART2_RX.GPIO_Speed = GPIO_Speed_50MHz;
    USART2_RX.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &USART2_RX);
}
void io_USART1_Init(void)
{
    USART_InitTypeDef USART1_InitStructure;

    USART_Cmd(USART1, ENABLE);

    USART1_InitStructure.USART_BaudRate = 9600;
    USART1_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART1_InitStructure.USART_StopBits = USART_StopBits_1;
    USART1_InitStructure.USART_Parity = USART_Parity_No;
    USART1_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART1_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}
void io_USART2_Init(void)
{
    USART_InitTypeDef USART2_InitStructure;

    USART_Cmd(USART2, ENABLE);

    USART2_InitStructure.USART_BaudRate = 9600;
    USART2_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART2_InitStructure.USART_StopBits = USART_StopBits_1;
    USART2_InitStructure.USART_Parity = USART_Parity_No;
    USART2_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART2_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART2, &USART2_InitStructure);

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

void io_NVIC_Configure(void) {

    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    // USART1
    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // USART2
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void sendMessage() {
    // 게임 종료 후 결과를 핸드폰으로 전송함
    // values: 게임 결과 값
    for(int i=0; i<player_count; i++){
        printf("value: %d", values[i]);
        USART_SendData(USART2, values[i]);

        // delay
        for(int i=0; i<2000000; i++);
    }

    // finish 메시지 (끝내는 flag값)
    USART_SendData(USART2, 1000);

    // delay
    for(int i=0; i<2000000; i++);
}

// char type인 array를 int type으로 변경
// 데이터 나누는 기준은 공백
// ex) {'1', ' ', '2', '3'} -> {1, 23}
void io_receivedDataParsing(){
    // numbers = {index(게임 인덱스 - 0~2), people_count(인원 수)}
    int numbers[100] = {0, };

    // 데이터 파싱을 위한 임시 변수
    // 현재 몇 개를 파싱했는지
    printf("io_received_parsing");
    int io_count = 0;

    // 현재 저장해야 할 값
    int currentNumber = 0;

    // 데이터 파싱
    for (int i = 0; i < 100; i++) {
        if (io_char[i] >= '0' && io_char[i] <= '9') {
            currentNumber = currentNumber * 10 + (io_char[i] - '0');
        }
        else if (io_char[i] == ' ' && io_char >= 0) {
            numbers[io_count] = currentNumber;
            io_count++;
            currentNumber = 0;

            // 파싱 완료
            if (io_count == 3) break;
        }
    }

    // 현재 게임의 index와 플레이어 인원수 저장
    cur_game = numbers[0];
    player_count = numbers[1];

    // 데이터 수신 state 변경
    is_data_received = 2;

    // 받아온 값을 저장했던 io_char 초기화
    for(int i=0; i<100; i++) io_char[i] = 0;
}

// 블루투스로부터 데이터 받고 컴퓨터한테 보냄
void io_USART2_IRQHandler() {
    uint16_t word;
    if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET){
        word = USART_ReceiveData(USART2);

        // 받아온 데이터 저장
        io_char[io_index] = word;
        io_index++;

        // FINISH_MESSAGE가 'A'이면 데이터 수신 상태 변경
        if (word == 'A') {
            is_data_received = 1;
        }

        USART_SendData(USART1, word);
        USART_ClearITPendingBit(USART2,USART_IT_RXNE);
    }

    // io_char은 현재 {'0', ' ', '1', '2' } 이렇게 저장되어 있기 때문에
    // 이를 int형으로 parsing하는 작업이 필요
    printf("mymessage: %c \n", word);

    if (is_data_received == 1) {
        io_receivedDataParsing();
    }
}

// 블루투스 연결 관련 configuration 코드
void io_Configure() {
    io_rcc_Configure();
    io_gpio_Configure();
    io_USART1_Init();
    io_USART2_Init();
    io_NVIC_Configure();
}