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
    // USART2_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS;
    USART2_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART2, &USART2_InitStructure);

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

void io_NVIC_Configure(void) {

    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1; // TODO
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x1; // TODO
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // USART2
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1; // TODO
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x1; // TODO
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void io_USART1_IRQHandler() {
    // 컴퓨터로부터 받고 블루투스한테 보냄
    // uint16_t io_arr[] = {1,2,30,100,0};
    // uint16_t word;

    // for(int i=0; i<5; i++){
    //     USART_SendData(USART2, io_arr[i]);
    // }
    // if(USART_GetITStatus(USART1,USART_IT_RXNE)==RESET){
    //     // the most recent received data by the USART1 peripheral
    //     word = USART_ReceiveData(USART1);

    //     USART_SendData(USART2, word);

    //     // clear 'Read data register not empty' flag
    // 	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    // }
    // USART1.TXDATAL = 'c';
}
uint16_t io_char[100];

int io_index = 0;
void sendMessage() {
    uint16_t io_arr[100] = {19, 30, 90, 10, 12};

        for(int i=0; i<5; i++){
            USART_SendData(USART2, io_arr[i]);
            // while((USART2->SR & USART_SR_TC) == 0);
            for(int i=0; i<2000000; i++);
        }
        
        USART_SendData(USART2, 1000);
        for(int i=0; i<2000000; i++);
}
void io_USART2_IRQHandler() {
    // 블루투스로부터 데이터 받고 컴퓨터한테 보냄
    //printf("in io_USART2_IRQHandler\n");
    uint16_t word;
    if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET){
        word = USART_ReceiveData(USART2);

        //printf("getMessage: %d %d\n", io_index, word);
        io_char[io_index] = word;
        io_index++;

        USART_SendData(USART1, word);
        USART_ClearITPendingBit(USART2,USART_IT_RXNE);
        // sendMessage();
    }
   // int mindex = 0;
     printf("get = %c\n", word);
    // printf("get = %d\n", word);
    // printf(word);
    // for (mindex = 0; mindex < 100; mindex++){
    //     printf("messa: %c ", io_char[io_index-1]);
    // }
    // printf("\n");
}

void io_Configure() {
    io_rcc_Configure();
    io_gpio_Configure();
    io_USART1_Init();
    io_USART2_Init();
    io_NVIC_Configure();
}
