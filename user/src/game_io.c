#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

#include "game_io.h"

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
    // todo: 일단 되는지 확인하고 io_arr -> values, 5 -> player_count로 변경
    // 게임 종료 후 결과를 핸드폰으로 전송함
    uint16_t io_arr[100] = {19, 30, 90, 10, 12};
    for(int i=0; i<5; i++){
        USART_SendData(USART2, io_arr[i]);
        for(int i=0; i<2000000; i++); // delay
    }

    USART_SendData(USART2, 1000);
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}

uint16_t io_char[100]; // 받아온 값을 저장
int io_index = 0;

void io_USART2_IRQHandler() {
    // 블루투스로부터 데이터 받고 컴퓨터한테 보냄
    uint16_t word;
    if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET){
        word = USART_ReceiveData(USART2);

        io_char[io_index] = word;
        io_index++;

        if (word == '\n') {
            is_data_received = 1;
        }

        USART_SendData(USART1, word);
        USART_ClearITPendingBit(USART2,USART_IT_RXNE);
    }
    // io_char은 현재 {'0', ' ', '1', '2' } 이렇게 저장되어 있기 때문에
    // 이를 int형으로 parsing하는 작업이 필요
    if (is_data_received == 1) {
        io_receivedDataParsing();
    }
}

void io_receivedDataParsing(){
    // numbers = {index(게임 인덱스 - 0~2), people_count(인원 수), goal(목표 점수)}
    int numbers[100];
    int io_count = 0;
    int currentNumber = 0;

    for (int i = 0; i < 100; i++) {
        if (io_char[i] >= '0' && io_char[i] <= '9') {
            currentNumber = currentNumber * 10 + (io_char[i] - '0');
        }
        else if (io_char[i] == ' ' && io_char >= 0) {
            numbers[io_count] = currentNumber;
            io_count++;
            currentNumber = 0;

            if (io_count == 3) break;
        }
    }
    // todo: 결과값 확인하고 주석 지우기
    printf("received data: ")
    for (int i = 0; i < io_count; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n")

    cur_game = numbers[0];
    player_count = numbers[1];
    is_data_received = 2;

    printf("cur_game: %d, player_count = %d\n", cur_game, player_count);
}

void io_Configure() {
    io_rcc_Configure();
    io_gpio_Configure();
    io_USART1_Init();
    io_USART2_Init();
    io_NVIC_Configure();
}
