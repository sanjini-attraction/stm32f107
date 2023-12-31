/*
    Time Game

    정해진 시간을 최대한 가깝게 맞추는 사람이 이기는 게임
    - 게임 진행 순서
        1. 터치 센서를 눌러 턴을 시작
        2. 플레이어가 자신이 원하는 만큼 대기
        3. 터치 센서를 눌러 턴을 종료(시간 기록)
        4. 모든 플레이어가 턴을 종료할 때 까지 1~3 반복
        5. 가장 오차가 작은 플레이어가 승리

    - 내부 구현
        - 타이머 인터럽트를 이용하여 counter 변수 1씩 증가
        - 터치 센서 인터럽트를 이용하여, state 변수를 변경
            - state 변수에 따라 턴 시작, 종료를 구분 
            - 턴 시작 시 counter 변수를 0으로 초기화
            - 턴 종료 시 counter 값을 저장
*/

/* -----------     Configure     ----------- */
void timeGame_Configure();


/* ----------- Interrupt Handler ----------- */
void timeGame_TimerHandler();
void timeGame_TouchHandler();


/* -----------    Game Control   ----------- */
void timeGame_turnHandler();
void timeGame();