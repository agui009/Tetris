// block.cpp

#include "block.h"
#define UNIT_SIZE 10 // �����ֵ����Ϊ�������ֵ��

extern int START_X; // ����������ʼ������
extern int START_Y; // ����������ʼ������
extern int SCREEN_WIDTH; // ����������
extern int SCREEN_HEIGHT; // ��������߶�
extern int visit[22][15]; // �����������
extern int markColor[22][15]; // ������ɫ����
extern int get_next_num(const bool new_seed = false, const unsigned int seed = 0); // ����α��������ɺ���

int colors[10] = { GREEN, CYAN, MAGENTA, BROWN, YELLOW, RED, BLUE, WHITE, LIGHTGRAY, LIGHTGREEN };

int blocks[10][BLOCK_SIZE][BLOCK_SIZE] = { // ��������
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

// ���캯��
Block::Block() {
    x = START_X + SCREEN_WIDTH / 2 * UNIT_SIZE - BLOCK_SIZE / 2 * UNIT_SIZE; // ���ó�ʼ������
    y = START_Y; // ���ó�ʼ������
    index = get_next_num(); // ���������������
    color = colors[index]; // ����������ɫ
    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            block[i][j] = blocks[index][i][j]; // ������������
        }
    }
}

// ��ת����
void Block::rotate(int temp[BLOCK_SIZE][BLOCK_SIZE]) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            temp[j][BLOCK_SIZE - 1 - i] = block[i][j]; // ��ʱ����ת90��
        }
    }
}

// �ƶ�����
void Block::move(int x, int y, move_dir_t dir) {
    if (dir == MOVE_DOWN) {
        this->y += UNIT_SIZE; // ����������
    }
    else if (dir == MOVE_LEFT) {
        this->x -= UNIT_SIZE; // ���������
    }
    else if (dir == MOVE_RIGHT) {
        this->x += UNIT_SIZE; // ����������
    }
}

// ��������
void Block::draw(int x, int y) {
    setcolor(color);
    setfont(UNIT_SIZE, 0, "����");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            if (block[i][j] == 1) {
                outtextxy(x + UNIT_SIZE * j, y + UNIT_SIZE * i, "��");
            }
        }
    }
}

// �������
void Block::clear(int x, int y) {
    setcolor(BLACK);
    setfont(UNIT_SIZE, 0, "����");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            if (block[i][j] == 1) {
                outtextxy(x + UNIT_SIZE * j, y + UNIT_SIZE * i, "��");
            }
        }
    }
}

// �������
void Block::mark() {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            if (block[i][j] == 1) {
                visit[(y - START_Y) / UNIT_SIZE + i][(x - START_X) / UNIT_SIZE + j] = 1; // ��Ƿ�������
                markColor[(y - START_Y) / UNIT_SIZE + i][(x - START_X) / UNIT_SIZE + j] = color; // �����ɫ����
            }
        }
    }
}
