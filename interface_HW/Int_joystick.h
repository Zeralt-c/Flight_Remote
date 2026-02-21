#ifndef __INT_JOYSTICK_H
#define __INT_JOYSTICK_H

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "adc.h"

typedef struct {
    uint16_t thr;// 油门
    uint16_t yaw;// 航向
    uint16_t roll;// 横滚
    uint16_t pitch;// 俯仰
} Joystick_Struct;

/**
 * @brief 摇杆初始化函数, 初始化ADC和DMA
 */
void Joystick_Init(void);
/**
 * @brief 获取摇杆数据函数
 * @param joystick 指向Joystick_Struct结构体的指针，用于存储摇杆数据
 */
void Joystick_GetData(Joystick_Struct *joystick);

#endif // __INT_JOYSTICK_H
