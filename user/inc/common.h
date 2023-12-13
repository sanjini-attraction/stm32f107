#define PLAYER_MAX 100

// 각 플레이어의 데이터 저장
extern int values[PLAYER_MAX];

// 블루투스 연결시 값을 받아왔는지에 대한 상태 관리
// 2: 파싱까지 완료 | 1: 데이터 받아옴 | 0: 아직 다 안 받아옴
extern int is_data_received;

// 현재 게임의 index
extern int cur_game;

// 총 플레이어 수
extern int player_count;

// 현재 플레이어의 index
extern int cur_player;

// 1(playing), 0(ready)
extern int game_state;

// 모든 플레이어가 턴을 종료했는지 확인
extern int allTurnEnd;

/*
    여러 게임에서 공통적으로 사용하는 함수들
    turnEnd_Handler : 사용자가 턴 종료/시작 버튼을 눌렀을때 호출되어 게임의 상태를 변경하는 함수
*/
void turnButton_Handler();
void turnButton_Button_Configure();