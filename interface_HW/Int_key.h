#ifndef __INT_KEY_H__
#define __INT_KEY_H__
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

typedef enum{
    KEY_NONE = 0,
    KEY_UP = 1 << 0,
    KEY_DOWN = 1 << 1,
    KEY_LEFT = 1 << 2,
    KEY_RIGHT = 1 << 3,
    KEY_LEFT_X = 1 << 4,
    KEY_RIGHT_X_SHORT = 1 << 5,
    KEY_RIGHT_X_LONG = 1 << 6
}Key_type;
/**
 * @brief 按键扫描函数
 * 
 * @return Key_type 按键类型
 */
Key_type Key_Get(void);
#endif // __INT_KEY_H__
