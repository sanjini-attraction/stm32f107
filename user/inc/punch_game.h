/*
    Punch Game

    설정해둔 값과 가장 비슷하게 압력을 가하는 사람이 이기는 게임
    - 게임 진행 순서
        1. 1번 버튼을 누르면 턴을 시작한다.
        2. 압력 센서에 압력을 가한다.
        3. 2번 버튼을 누르면 턴을 종료한다.
        4. 모든 플레이어가 턴을 종료할 때 까지 1 ~ 4단계를 반복한다.
        5. 가장 오차가 적게 나는 사람이 승리한다.

    - 내부 구현
        - 압력 센서의 값을 ADC와 DMA를 이용하여 변수에 저장
        - mapping 함수를 이용하여 압력값을 0 ~ 100 사이의 값으로 변환
        - Polling 방식으로 최대값을 계속 업데이트
            - 목표 점수가 바뀔 수 있기 때문에 threshold를 이용하여 interrup를 발생시키는 것이 불가능
*/

/* -----------     Configure     ----------- */
void punchGame_Configure();

/* -----------    Game Control   ----------- */
void punchGame();
void punchGame_turnHandler();