#ifndef __APP_PROCESS_DATA_H__
#define __APP_PROCESS_DATA_H__

#include "Int_Key.h"
#include "Int_joystick.h"
#include "Com_debug.h"
#include "Com_tool.h"

/**
 * @brief 遥控器数据结构体
 *  int16_t thr;    // 油门
    int16_t yaw;    // 航向
    int16_t pitch;  // 俯仰
    int16_t roll;   // 横滚
    uint8_t shutdown; // 关机按键
    uint8_t fixed_height; // 定高按键
 */
typedef struct {
    int16_t thr;    // 油门
    int16_t yaw;    // 航向
    int16_t pitch;  // 俯仰
    int16_t roll;   // 横滚
    uint8_t shutdown; // 关机按键，1表示关机
    uint8_t fixed_height; // 定高按键，1表示切换
} Remote_Data;




/**
 * @brief 处理按键数据
 * 
 * 
 */
void App_process_key_data(void);

/**
 * @brief 处理摇杆数据，修正摇杆数据，进行死区处理等
 * 
 * 
 */
void App_process_joystick_data(void);

#endif // __APP_PROCESS_DATA_H__
