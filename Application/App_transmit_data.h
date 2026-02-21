#ifndef __APP_TRANSMIT_DATA_H
#define __APP_TRANSMIT_DATA_H


#include "App_FreeRTOS.h"

//帧头校验宏定义
#define FRAME_HEADER_CHECK_1 'z'
#define FRAME_HEADER_CHECK_2 'z'
#define FRAME_HEADER_CHECK_3 'z'

/**
 * @brief 数据发送，将遥控器数据发送到串口
 * 
 */
void App_transmit_data(void);

#endif
