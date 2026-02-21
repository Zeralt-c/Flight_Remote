#include "App_process_data.h"

Joystick_Struct joystick_data = {0}; // 定义一个结构体变量用于存储摇杆数据
Remote_Data remote_data = {0}; // 定义一个结构体变量用于存储遥控器数据
//按键偏移
int16_t key_pitch_offset = 0; 
int16_t key_roll_offset = 0; 

//校准偏移
int16_t calibrate_thr_offset = 0; 
int16_t calibrate_pitch_offset = 0; 
int16_t calibrate_roll_offset = 0; 
int16_t calibrate_yaw_offset = 0; 

//校准摇杆，摇杆校准thr值为0，其他值为500
void App_calibrate_joystick(void){

    //首先清空按键偏移
    key_pitch_offset = 0;
    key_roll_offset = 0;

    int16_t thr_sum = 0;
    int16_t yaw_sum = 0;
    int16_t pitch_sum = 0;
    int16_t roll_sum = 0;
    for(int i = 0; i < 10; i++){
        App_process_joystick_data();
        thr_sum += joystick_data.thr - 0;
        yaw_sum += joystick_data.yaw - 500;
        pitch_sum += joystick_data.pitch - 500;
        roll_sum += joystick_data.roll - 500;
        vTaskDelay(10);
    }
    calibrate_thr_offset += thr_sum / 10;
    calibrate_yaw_offset += yaw_sum / 10;
    calibrate_pitch_offset += pitch_sum / 10;
    calibrate_roll_offset += roll_sum / 10;
}



void App_process_key_data(void){
    Key_type key_status = Key_Get(); //调用按键扫描函数，获取按键状态
    //根据按键调整偏航和俯仰的偏移量
    if(key_status & KEY_UP){
        key_pitch_offset += 10;
    }
    if(key_status & KEY_DOWN){
        key_pitch_offset -= 10;
    }
    if(key_status & KEY_LEFT){
        key_roll_offset -= 10;
    }
    if(key_status & KEY_RIGHT){
        key_roll_offset += 10;
    }
    if(key_status & KEY_LEFT_X){
        remote_data.shutdown = 1; // 设置关机标志
    }
    if (key_status & KEY_RIGHT_X_SHORT) {
        remote_data.fixed_height = 1; // 设置定高标志
    } 
    if (key_status & KEY_RIGHT_X_LONG) {
        App_calibrate_joystick(); // 长按右键X进行校准
    }    
}


void App_process_joystick_data(void){
    Joystick_GetData(&joystick_data); // 调用获取摇杆数据函数，将数据存储到结构体中
    
    taskENTER_CRITICAL();

    //处理范围和极性，ADC值的范围转换到0-1000
    joystick_data.thr = 1000 - (joystick_data.thr * 1000 / 4095); // 油门
    joystick_data.yaw = 1000 - (joystick_data.yaw * 1000 / 4095); // 方向
    joystick_data.pitch = 1000 - (joystick_data.pitch * 1000 / 4095); // 俯仰
    joystick_data.roll = 1000- (joystick_data.roll * 1000 / 4095); // 滚转

    // 校准偏移
    joystick_data.thr -= calibrate_thr_offset;
    joystick_data.yaw -= calibrate_yaw_offset;
    joystick_data.pitch -= calibrate_pitch_offset;
    joystick_data.roll -= calibrate_roll_offset;

    //按键微调
    joystick_data.pitch += key_pitch_offset;
    joystick_data.roll += key_roll_offset;

    //限制范围在0-1000
    joystick_data.thr = Com_limit(joystick_data.thr, 0, 1000);
    joystick_data.yaw = Com_limit(joystick_data.yaw, 0, 1000);
    joystick_data.pitch = Com_limit(joystick_data.pitch, 0, 1000);
    joystick_data.roll = Com_limit(joystick_data.roll, 0, 1000);

    remote_data.pitch = joystick_data.pitch;
    remote_data.roll = joystick_data.roll;
    remote_data.yaw = joystick_data.yaw;
    remote_data.thr = joystick_data.thr;

    taskEXIT_CRITICAL();

    //LOG_DEBUG(":%d,%d,%d,%d\n",joystick_data.thr, joystick_data.yaw, joystick_data.pitch, joystick_data.roll);
}
