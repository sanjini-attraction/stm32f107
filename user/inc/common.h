#define PLAYER_MAX 100

/*
    Bluethooh로 보낼 데이터

    values : 각 플레이어의 데이터 저장
*/
extern int values[PLAYER_MAX];


/* 
    Bluethooh로 받아올때 필요한 변수

    player_count : 총 플레이어 수
    cur_game : 현재 게임의 index
        0 : shake game
        1 : punch game
        2 : time game
    is_data_received : 블루투스 연결시 값을 받아왔는지에 대한 상태 관리
        0 : 아직 다 안 받아옴
        1 : 데이터 받아옴
        2 : 파싱까지 완료
*/
extern int player_count;
extern int cur_game;
extern int is_data_received;


/*
    게임 진행에 필요한 변수

    led_state : 현재 턴 LED의 상태
        0 : 꺼짐
        1 : 켜짐
    cur_player : 현재 플레이어의 index
    game_state : 게임의 상태(1: 게임 진행중 | 0: 게임 종료)
    allTurnEnd : 모든 플레이어가 턴을 종료했는지 확인
*/
//extern int led_state;
extern int cur_player;
extern int game_state;
extern int allTurnEnd;


/*
    여러 게임에서 공통적으로 사용하는 함수들
    turnEnd_Handler : 사용자가 턴 버튼을 눌렀을때 ISR에 의해 호출되어 게임의 상태를 변경
    turnEnd_Button_Configure : 턴 버튼 GPIO, EXTI, NVIC 설정
    turnLed_Configure : 턴 LED GPIO 설정
*/
void turnButton_Handler();
void turnButton_Button_Configure();
void turnLed_Configure();