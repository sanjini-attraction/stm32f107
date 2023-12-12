#define PLAYER_MAX 100

extern int values[PLAYER_MAX]; // 각 플레이어의 데이터 저장

extern int player_count;   // bluetooth로 받아올 총 플레이어 수
extern int cur_player;     // 현재 플레이어의 index
extern int game_state;     // 1(playing), 0(ready)

extern char allTurnEnd;    // 모든 플레이어가 턴을 종료했는지 확인

/*
    여러 게임에서 공통적으로 사용하는 함수들
    turnEnd_Handler : 사용자가 턴 종료/시작 버튼을 눌렀을때 호출되어 게임의 상태를 변경하는 함수
*/
void turnEnd_Handler();
void turnEnd_Button_Configure();