// main.cpp


#include <iostream>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include "block.h" // 引入数字类的头文件

#define START_X 130 // 区域起始横坐标
#define START_Y 30 // 区域起始纵坐标
const int SCREEN_WIDTH = 15; // 区域宽度，加上const修饰符
const int SCREEN_HEIGHT = 22; // 区域高度，加上const修饰符
#define KEY_UP 72 // 上箭头键
#define KEY_RIGHT 77 // 右箭头键
#define KEY_LEFT 75 // 左箭头键
#define KEY_SPACE 32 // 空格键
#define KEY_DOWN 80 // 下箭头键

int speed = 1000; // 下落速度
int score = 0; // 分数
int rank = 0; // 等级
int visit[SCREEN_HEIGHT][SCREEN_WIDTH]; // 访问数组
int markColor[SCREEN_HEIGHT][SCREEN_WIDTH]; // 颜色数组
int minX = SCREEN_WIDTH; // 最小横坐标
int minY = SCREEN_HEIGHT; // 最小纵坐标
Block* nextBlock = NULL; // 下一个数字
Block* curBlock = NULL; // 当前数字

// 消除光标闪动
void cursor(bool a) {
    CONSOLE_CURSOR_INFO cursor_info = { 1, a };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

// 把光标定位到指定坐标
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

// 生成下一个数字
void generateNextBlock() {
    if (nextBlock == NULL) {
        nextBlock = new Block();
    }
    if (curBlock == NULL) {
        curBlock = nextBlock;
        nextBlock = new Block();
    }
}

// 显示下一个数字的提示
void showNextBlock() {
    setcolor(WHITE);
    setfont(UNIT_SIZE, 0, "楷体");
    outtextxy(START_X + SCREEN_WIDTH * UNIT_SIZE + 10, START_Y + 10, "下一个数字：");
    nextBlock->draw(START_X + SCREEN_WIDTH * UNIT_SIZE + 10, START_Y + 40);
}

// 显示分数和等级
void showScoreAndRank() {
    setcolor(WHITE);
    setfont(UNIT_SIZE, 0, "楷体");
    outtextxy(START_X + SCREEN_WIDTH * UNIT_SIZE + 10, START_Y + 100, "分数：");
    outtextxy(START_X + SCREEN_WIDTH * UNIT_SIZE + 10, START_Y + 130, "等级：");
    char scoreStr[10];
    char rankStr[10];
    //sprintf(scoreStr, "%d", score);
    //sprintf(rankStr, "%d", rank);
    sprintf_s(scoreStr, sizeof(scoreStr), "%d", score);
    sprintf_s(rankStr, sizeof(rankStr), "%d", rank);

    outtextxy(START_X + SCREEN_WIDTH * UNIT_SIZE + 60, START_Y + 100, scoreStr);
    outtextxy(START_X + SCREEN_WIDTH * UNIT_SIZE + 60, START_Y + 130, rankStr);
}

// 更新分数和等级
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

// 检查是否可以移动
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

// 检查是否可以旋转
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

// 检查是否可以继续游戏
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


// 检查是否可以消除行
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
                    setfont(UNIT_SIZE, 0, "楷体");
                    outtextxy(START_X + UNIT_SIZE * j, START_Y + UNIT_SIZE * i, "■");
                }
                else {
                    setcolor(BLACK);
                    setfont(UNIT_SIZE, 0, "楷体");
                    outtextxy(START_X + UNIT_SIZE * j, START_Y + UNIT_SIZE * i, "■");
                }
            }
        }
    }
}

// 画边框
void drawBorder() {
    setcolor(WHITE);
    setfont(UNIT_SIZE, 0, "楷体");
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        outtextxy(START_X - UNIT_SIZE, START_Y + UNIT_SIZE * i, "■");
        outtextxy(START_X + SCREEN_WIDTH * UNIT_SIZE, START_Y + UNIT_SIZE * i, "■");
    }
    for (int i = 0; i < SCREEN_WIDTH + 2; i++) {
        outtextxy(START_X - UNIT_SIZE + UNIT_SIZE * i, START_Y - UNIT_SIZE, "■");
        outtextxy(START_X - UNIT_SIZE + UNIT_SIZE * i, START_Y + SCREEN_HEIGHT * UNIT_SIZE, "■");
    }
}

// 游戏结束
void gameOver() {
    setcolor(RED);
    setfont(UNIT_SIZE * 2, 0, "楷体");
    outtextxy(START_X + SCREEN_WIDTH * UNIT_SIZE / 2 - 60, START_Y + SCREEN_HEIGHT * UNIT_SIZE / 2 - 20, "游戏结束");
}

// 游戏主循环
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

// 游戏初始化
void gameInit() {
    initgraph(SCREEN_WIDTH * UNIT_SIZE + 400, SCREEN_HEIGHT * UNIT_SIZE + 300); // 初始化图形界面
    cursor(false); // 隐藏光标
    drawBorder(); // 画边框
    showScoreAndRank(); // 显示分数和等级
}

// 游戏开始
void gameStart() {
    gameInit(); // 游戏初始化
    gameLoop(); // 游戏主循环
    //getch(); // 按任意键退出
    //_getch(); // 按任意键退出
    closegraph(); // 关闭图形界面
}



// 主函数
int main() {
  
    srand(time(NULL)); // 设置随机数种子
    gameStart(); // 游戏开始
    return 0;
}
