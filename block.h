// block.h


#ifndef BLOCK_H
#define BLOCK_H

#include <graphics.h>

#define BLOCK_SIZE 5 // ���������С
#define UNIT_SIZE 3 // ��Ԫ��С

typedef enum { // �ƶ�����
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT
} move_dir_t;

class Block {
public:
    int x; // ���ֵĺ�����
    int y; // ���ֵ�������
    int index; // ���ֵ�����
    static const int START_X = 10; // ����Ϊ�ʵ���ֵ
    static const int START_Y = 5;  // ����Ϊ�ʵ���ֵ
    static const int SCREEN_WIDTH = 15; // ����Ϊ�ʵ���ֵ
    int block[BLOCK_SIZE][BLOCK_SIZE]; // ���ֵ�����
    int color; // ���ֵ���ɫ��������һ��
    Block(); // ���캯��
    void rotate(int temp[BLOCK_SIZE][BLOCK_SIZE]); // ��ת����
    void move(int x, int y, move_dir_t dir); // �ƶ�����
    void draw(int x, int y); // ��������
    void clear(int x, int y); // �������
    void mark(); // �������
};

#endif
