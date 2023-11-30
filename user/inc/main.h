#define PLAYER_MAX 10


int values[PLAYER_MAX] = {0, }; // 각 플레이어의 데이터 저장


int player_count = 0;   // bluetooth로 받아올 총 플레이어 수
int cur_player = 0;     // 현재 플레이어의 index
int game_state = 0;     // 1(playing), 0(ready)

char allTrunEnd = 0;    // 모든 플레이어가 턴을 종료했는지 확인