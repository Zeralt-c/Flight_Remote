#include "Int_joystick.h"


uint16_t joystick_data_buffer[4] = {0}; // 用于存储ADC DMA传输的数据
void Joystick_Init(void){
    // ADC和DMA的初始化代码
    // 这里假设已经在CubeMX中配置好了ADC和DMA，并生成了相应的初始化代码
    // 你需要根据实际情况修改以下代码
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)joystick_data_buffer, 4); // 启动ADC DMA传输，假设有4个通道
}

void Joystick_GetData(Joystick_Struct *joystick){
    // 从DMA缓冲区获取数据并存储到Joystick_Struct结构体中
    joystick->thr = joystick_data_buffer[0]; // 油门
    joystick->yaw = joystick_data_buffer[1]; // 航向
    joystick->pitch = joystick_data_buffer[2]; // 俯仰
    joystick->roll = joystick_data_buffer[3]; // 横滚    
}
