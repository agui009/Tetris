// block.cpp

#include "block.h"
#define UNIT_SIZE 10 // 将这个值调整为更大的数值，

extern int START_X; // 引入区域起始横坐标
extern int START_Y; // 引入区域起始纵坐标
extern int SCREEN_WIDTH; // 引入区域宽度
extern int SCREEN_HEIGHT; // 引入区域高度
extern int visit[22][15]; // 引入访问数组
extern int markColor[22][15]; // 引入颜色数组
extern int get_next_num(const bool new_seed = false, const unsigned int seed = 0); // 引入伪随机数生成函数

int colors[10] = { GREEN, CYAN, MAGENTA, BROWN, YELLOW, RED, BLUE, WHITE, LIGHTGRAY, LIGHTGREEN };

int blocks[10][BLOCK_SIZE][BLOCK_SIZE] = { // 数字数组
    // 0
    {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 1, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0}
    },
    // 1
    {
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}
    },
    // 2
    {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0}
    },
    // 3
    {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0}
    },
    // 4
    {
        {0, 0, 0, 0, 0},
        {0, 1, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0}
    },
    // 5
    {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0}
    },
    // 6
    {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 1, 0, 0}
    },
    // 7
    {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}
    },
    // 8
    {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 1, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0}
    },
    // 9
    {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 1, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0}
    }
};

// 构造函数
Block::Block() {
    x = START_X + SCREEN_WIDTH / 2 * UNIT_SIZE - BLOCK_SIZE / 2 * UNIT_SIZE; // 设置初始横坐标
    y = START_Y; // 设置初始纵坐标
    index = get_next_num(); // 随机生成数字种类
    color = colors[index]; // 设置数字颜色
    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            block[i][j] = blocks[index][i][j]; // 设置数字数组
        }
    }
}

// 旋转数字
void Block::rotate(int temp[BLOCK_SIZE][BLOCK_SIZE]) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            temp[j][BLOCK_SIZE - 1 - i] = block[i][j]; // 逆时针旋转90度
        }
    }
}

// 移动数字
void Block::move(int x, int y, move_dir_t dir) {
    if (dir == MOVE_DOWN) {
        this->y += UNIT_SIZE; // 纵坐标增加
    }
    else if (dir == MOVE_LEFT) {
        this->x -= UNIT_SIZE; // 横坐标减少
    }
    else if (dir == MOVE_RIGHT) {
        this->x += UNIT_SIZE; // 横坐标增加
    }
}

// 绘制数字
void Block::draw(int x, int y) {
    setcolor(color);
    setfont(UNIT_SIZE, 0, "楷体");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            if (block[i][j] == 1) {
                outtextxy(x + UNIT_SIZE * j, y + UNIT_SIZE * i, "■");
            }
        }
    }
}

// 清除数字
void Block::clear(int x, int y) {
    setcolor(BLACK);
    setfont(UNIT_SIZE, 0, "楷体");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            if (block[i][j] == 1) {
                outtextxy(x + UNIT_SIZE * j, y + UNIT_SIZE * i, "■");
            }
        }
    }
}

// 标记数字
void Block::mark() {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            if (block[i][j] == 1) {
                visit[(y - START_Y) / UNIT_SIZE + i][(x - START_X) / UNIT_SIZE + j] = 1; // 标记访问数组
                markColor[(y - START_Y) / UNIT_SIZE + i][(x - START_X) / UNIT_SIZE + j] = color; // 标记颜色数组
            }
        }
    }
}
