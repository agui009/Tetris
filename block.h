// block.h


#ifndef BLOCK_H
#define BLOCK_H

#include <graphics.h>

#define BLOCK_SIZE 5 // 数字区域大小
#define UNIT_SIZE 3 // 单元大小

typedef enum { // 移动方向
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT
} move_dir_t;

class Block {
public:
    int x; // 数字的横坐标
    int y; // 数字的纵坐标
    int index; // 数字的种类
    static const int START_X = 10; // 调整为适当的值
    static const int START_Y = 5;  // 调整为适当的值
    static const int SCREEN_WIDTH = 15; // 调整为适当的值
    int block[BLOCK_SIZE][BLOCK_SIZE]; // 数字的数组
    int color; // 数字的颜色，加上这一行
    Block(); // 构造函数
    void rotate(int temp[BLOCK_SIZE][BLOCK_SIZE]); // 旋转数字
    void move(int x, int y, move_dir_t dir); // 移动数字
    void draw(int x, int y); // 绘制数字
    void clear(int x, int y); // 清除数字
    void mark(); // 标记数字
};

#endif
