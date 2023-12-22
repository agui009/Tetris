// main.cpp


#include <iostream>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include "block.h" // �����������ͷ�ļ�

#define START_X 130 // ������ʼ������
#define START_Y 30 // ������ʼ������
const int SCREEN_WIDTH = 15; // �����ȣ�����const���η�
const int SCREEN_HEIGHT = 22; // ����߶ȣ�����const���η�
#define KEY_UP 72 // �ϼ�ͷ��
#define KEY_RIGHT 77 // �Ҽ�ͷ��
#define KEY_LEFT 75 // ���ͷ��
#define KEY_SPACE 32 // �ո��
#define KEY_DOWN 80 // �¼�ͷ��

int speed = 1000; // �����ٶ�
int score = 0; // ����
int rank = 0; // �ȼ�
int visit[SCREEN_HEIGHT][SCREEN_WIDTH]; // ��������
int markColor[SCREEN_HEIGHT][SCREEN_WIDTH]; // ��ɫ����
int minX = SCREEN_WIDTH; // ��С������
int minY = SCREEN_HEIGHT; // ��С������
Block* nextBlock = NULL; // ��һ������
Block* curBlock = NULL; // ��ǰ����

// �����������
void cursor(bool a) {
    CONSOLE_CURSOR_INFO cursor_info = { 1, a };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

// �ѹ�궨λ��ָ������
void gotoxy(int x, int y) {
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
int get_next_num(const bool new_seed = false, const unsigned int seed = 0) {
    if (new_seed) {
        srand(seed);
    }
    return rand() % 10;
}

// ������һ������
void generateNextBlock() {
    if (nextBlock == NULL) {
        nextBlock = new Block();
    }
    if (curBlock == NULL) {
        curBlock = nextBlock;
        nextBlock = new Block();
    }
}

// ��ʾ��һ�����ֵ���ʾ
void showNextBlock() {
    setcolor(WHITE);
    setfont(UNIT_SIZE, 0, "����");
    outtextxy(START_X + SCREEN_WIDTH * UNIT_SIZE + 10, START_Y + 10, "��һ�����֣�");
    nextBlock->draw(START_X + SCREEN_WIDTH * UNIT_SIZE + 10, START_Y + 40);
}

// ��ʾ�����͵ȼ�
void showScoreAndRank() {
    setcolor(WHITE);
    setfont(UNIT_SIZE, 0, "����");
    outtextxy(START_X + SCREEN_WIDTH * UNIT_SIZE + 10, START_Y + 100, "������");
    outtextxy(START_X + SCREEN_WIDTH * UNIT_SIZE + 10, START_Y + 130, "�ȼ���");
    char scoreStr[10];
    char rankStr[10];
    //sprintf(scoreStr, "%d", score);
    //sprintf(rankStr, "%d", rank);
    sprintf_s(scoreStr, sizeof(scoreStr), "%d", score);
    sprintf_s(rankStr, sizeof(rankStr), "%d", rank);

    outtextxy(START_X + SCREEN_WIDTH * UNIT_SIZE + 60, START_Y + 100, scoreStr);
    outtextxy(START_X + SCREEN_WIDTH * UNIT_SIZE + 60, START_Y + 130, rankStr);
}

// ���·����͵ȼ�
void updateScoreAndRank(int line) {
    if (line == 1) {
        score += 1;
    }
    else if (line == 2) {
        score += 3;
    }
    else if (line == 3) {
        score += 6;
    }
    else if (line == 4) {
        score += 10;
    }
    else if (line == 5) {
        score += 15;
    }
    rank = score / 15;
    if (rank > 7) {
        rank = 7;
    }
    speed = 1000 - rank * 100;
    showScoreAndRank();
}

// ����Ƿ�����ƶ�
bool checkMove(int x, int y, move_dir_t dir) {
    bool flag = true;
    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            if (curBlock->block[i][j] == 1) {
                if (dir == MOVE_DOWN) {
                    if (y + UNIT_SIZE * (i + 1) >= START_Y + SCREEN_HEIGHT * UNIT_SIZE || visit[(y - START_Y) / UNIT_SIZE + i + 1][(x - START_X) / UNIT_SIZE + j] == 1) {
                        flag = false;
                        break;
                    }
                }
                else if (dir == MOVE_LEFT) {
                    if (x + UNIT_SIZE * (j - 1) < START_X || visit[(y - START_Y) / UNIT_SIZE + i][(x - START_X) / UNIT_SIZE + j - 1] == 1) {
                        flag = false;
                        break;
                    }
                }
                else if (dir == MOVE_RIGHT) {
                    if (x + UNIT_SIZE * (j + 1) >= START_X + SCREEN_WIDTH * UNIT_SIZE || visit[(y - START_Y) / UNIT_SIZE + i][(x - START_X) / UNIT_SIZE + j + 1] == 1) {
                        flag = false;
                        break;
                    }
                }
            }
        }
        if (!flag) {
            break;
        }
    }
    return flag;
}

// ����Ƿ������ת
bool checkRotate() {
    bool flag = true;
    int temp[BLOCK_SIZE][BLOCK_SIZE];
    curBlock->rotate(temp);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            if (temp[i][j] == 1) {
                if (curBlock->x + UNIT_SIZE * j < START_X || curBlock->x + UNIT_SIZE * j >= START_X + SCREEN_WIDTH * UNIT_SIZE || curBlock->y + UNIT_SIZE * i >= START_Y + SCREEN_HEIGHT * UNIT_SIZE || visit[(curBlock->y - START_Y) / UNIT_SIZE + i][(curBlock->x - START_X) / UNIT_SIZE + j] == 1) {
                    flag = false;
                    break;
                }
            }
        }
        if (!flag) {
            break;
        }
    }
    return flag;
}

// ����Ƿ���Լ�����Ϸ
bool checkContinue() {
    bool flag = true;
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        if (visit[0][i] == 1) {
            flag = false;
            break;
        }
    }
    return flag;
}


// ����Ƿ����������
void checkLine() {
    int line = 0;
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        bool flag = true;
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            if (visit[i][j] == 0) {
                flag = false;
                break;
            }
        }
        if (flag) {
            line++;
            for (int k = i; k > 0; k--) {
                for (int l = 0; l < SCREEN_WIDTH; l++) {
                    visit[k][l] = visit[k - 1][l];
                    markColor[k][l] = markColor[k - 1][l];
                }
            }
            for (int l = 0; l < SCREEN_WIDTH; l++) {
                visit[0][l] = 0;
                markColor[0][l] = 0;
            }
        }
    }
    if (line > 0) {
        updateScoreAndRank(line);
        for (int i = 0; i < SCREEN_HEIGHT; i++) {
            for (int j = 0; j < SCREEN_WIDTH; j++) {
                if (visit[i][j] == 1) {
                    setcolor(markColor[i][j]);
                    setfont(UNIT_SIZE, 0, "����");
                    outtextxy(START_X + UNIT_SIZE * j, START_Y + UNIT_SIZE * i, "��");
                }
                else {
                    setcolor(BLACK);
                    setfont(UNIT_SIZE, 0, "����");
                    outtextxy(START_X + UNIT_SIZE * j, START_Y + UNIT_SIZE * i, "��");
                }
            }
        }
    }
}

// ���߿�
void drawBorder() {
    setcolor(WHITE);
    setfont(UNIT_SIZE, 0, "����");
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        outtextxy(START_X - UNIT_SIZE, START_Y + UNIT_SIZE * i, "��");
        outtextxy(START_X + SCREEN_WIDTH * UNIT_SIZE, START_Y + UNIT_SIZE * i, "��");
    }
    for (int i = 0; i < SCREEN_WIDTH + 2; i++) {
        outtextxy(START_X - UNIT_SIZE + UNIT_SIZE * i, START_Y - UNIT_SIZE, "��");
        outtextxy(START_X - UNIT_SIZE + UNIT_SIZE * i, START_Y + SCREEN_HEIGHT * UNIT_SIZE, "��");
    }
}

// ��Ϸ����
void gameOver() {
    setcolor(RED);
    setfont(UNIT_SIZE * 2, 0, "����");
    outtextxy(START_X + SCREEN_WIDTH * UNIT_SIZE / 2 - 60, START_Y + SCREEN_HEIGHT * UNIT_SIZE / 2 - 20, "��Ϸ����");
}

// ��Ϸ��ѭ��
void gameLoop() {
    generateNextBlock();
    showNextBlock();
    while (true) {
        if (checkContinue()) {
            generateNextBlock();
            showNextBlock();
            curBlock->draw(curBlock->x, curBlock->y);
            while (true) {
                if (_kbhit()) {
                    int key = _getch();
                    if (key == KEY_UP) {
                        if (checkRotate()) {
                            curBlock->clear(curBlock->x, curBlock->y);
                            curBlock->rotate(curBlock->block);
                            curBlock->draw(curBlock->x, curBlock->y);
                        }
                    }
                    else if (key == KEY_LEFT) {
                        if (checkMove(curBlock->x, curBlock->y, MOVE_LEFT)) {
                            curBlock->clear(curBlock->x, curBlock->y);
                            curBlock->move(curBlock->x - UNIT_SIZE, curBlock->y, MOVE_LEFT);
                            curBlock->draw(curBlock->x, curBlock->y);
                        }
                    }
                    else if (key == KEY_RIGHT) {
                        if (checkMove(curBlock->x, curBlock->y, MOVE_RIGHT)) {
                            curBlock->clear(curBlock->x, curBlock->y);
                            curBlock->move(curBlock->x + UNIT_SIZE, curBlock->y, MOVE_RIGHT);
                            curBlock->draw(curBlock->x, curBlock->y);
                        }
                    }
                    else if (key == KEY_DOWN) {
                        if (checkMove(curBlock->x, curBlock->y, MOVE_DOWN)) {
                            curBlock->clear(curBlock->x, curBlock->y);
                            curBlock->move(curBlock->x, curBlock->y + UNIT_SIZE, MOVE_DOWN);
                            curBlock->draw(curBlock->x, curBlock->y);
                        }
                    }
                    else if (key == KEY_SPACE) {
                        while (checkMove(curBlock->x, curBlock->y, MOVE_DOWN)) {
                            curBlock->clear(curBlock->x, curBlock->y);
                            curBlock->move(curBlock->x, curBlock->y + UNIT_SIZE, MOVE_DOWN);
                            curBlock->draw(curBlock->x, curBlock->y);
                        }
                    }
                }
                Sleep(speed);
                if (checkMove(curBlock->x, curBlock->y, MOVE_DOWN)) {
                    curBlock->clear(curBlock->x, curBlock->y);
                    curBlock->move(curBlock->x, curBlock->y + UNIT_SIZE, MOVE_DOWN);
                    curBlock->draw(curBlock->x, curBlock->y);
                }
                else {
                    curBlock->mark();
                    checkLine();
                    delete curBlock;
                    curBlock = NULL;
                    break;
                }
            }
        }
        else {
            gameOver();
            break;
        }
    }
}

// ��Ϸ��ʼ��
void gameInit() {
    initgraph(SCREEN_WIDTH * UNIT_SIZE + 400, SCREEN_HEIGHT * UNIT_SIZE + 300); // ��ʼ��ͼ�ν���
    cursor(false); // ���ع��
    drawBorder(); // ���߿�
    showScoreAndRank(); // ��ʾ�����͵ȼ�
}

// ��Ϸ��ʼ
void gameStart() {
    gameInit(); // ��Ϸ��ʼ��
    gameLoop(); // ��Ϸ��ѭ��
    //getch(); // ��������˳�
    //_getch(); // ��������˳�
    closegraph(); // �ر�ͼ�ν���
}



// ������
int main() {
  
    srand(time(NULL)); // �������������
    gameStart(); // ��Ϸ��ʼ
    return 0;
}
