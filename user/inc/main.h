#define PLAYER_MAX 10

extern int values[PLAYER_MAX]; // 각 플레이어의 데이터 저장

extern int player_count;   // bluetooth로 받아올 총 플레이어 수
extern int cur_player;     // 현재 플레이어의 index
extern int game_state;     // 1(playing), 0(ready)

extern char allTrunEnd;    // 모든 플레이어가 턴을 종료했는지 확인