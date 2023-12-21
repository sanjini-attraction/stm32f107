/*
    Shake Game

    정해진 흔드는 세기를 최대한 가깝게 맞추는 사람이 이기는 게임
    - 게임 진행 순서
        1. 터치 센서를 눌러 턴을 시작
        2. 플레이어가 진동 센서를 흔듦
        3. 터치 센서를 눌러 턴을 종료 (얼마나 흔들렸는지 기록)
        4. 모든 플레이어가 턴을 종료할 때 까지 1~3 반복
        5. 가장 오차가 작은 플레이어가 승리

    - 내부 구현
        - 터치 센서 인터럽트를 이용하여, state 변수를 변경
            - state 변수에 따라 턴 시작, 종료를 구분
            - 턴 시작 시 counter 변수를 0으로 초기화
            - 턴 종료 시 counter 값을 저장
            - GPIO_ReadInputDataBit를 이용하여 흔들렸는지 확인
            - 흔들림이 감지된 경우 count 1씩 증가
*/

/* -----------     Configure     ----------- */
void shakeGame_Configure();

/* -----------    Game Control   ----------- */
void shakeGame();
void shakeGame_turnHandler();