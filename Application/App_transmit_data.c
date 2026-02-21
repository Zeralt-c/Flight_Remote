#include "App_transmit_data.h"

//摇杆数据
extern Remote_Data remote_data;//10个字节
//数据缓冲区，用于存储通信数据
uint8_t data_buffer[TX_PLOAD_WIDTH] = {0};

void App_transmit_data(void){
    //发送模式
    SI24R1_TX_Mode();

    /** 发送数据包，唯一性和可靠性
     * 唯一性：帧头校验
     * 可靠性：帧尾检验和
     * 3字节帧头，10字节数据，4字节校验和
     */
    uint32_t checksum = 0;
    
    // 帧头
    data_buffer[0] = FRAME_HEADER_CHECK_1; 
    data_buffer[1] = FRAME_HEADER_CHECK_2;
    data_buffer[2] = FRAME_HEADER_CHECK_3;
    //数据remote_data，高8位在前
    data_buffer[3] = remote_data.thr >> 8;
    data_buffer[4] = remote_data.thr & 0xFF;

    data_buffer[5] = remote_data.yaw >> 8;
    data_buffer[6] = remote_data.yaw & 0xFF;

    data_buffer[7] = remote_data.pitch >> 8;
    data_buffer[8] = remote_data.pitch & 0xFF;

    data_buffer[9] = remote_data.roll >> 8;
    data_buffer[10] = remote_data.roll & 0xFF;

    //发完清除
    taskENTER_CRITICAL();
    data_buffer[11] = remote_data.shutdown;
    remote_data.shutdown = 0;
    data_buffer[12] = remote_data.fixed_height;
    remote_data.fixed_height = 0;
    taskEXIT_CRITICAL();

    for(int i = 0; i < 13; i++)
    {
        checksum += data_buffer[i];
    }
    //13字节数据，4字节校验和
    data_buffer[13] = checksum >> 24;
    data_buffer[14] = checksum >> 16;
    data_buffer[15] = checksum >> 8;
    data_buffer[16] = checksum & 0xFF;

    

    SI24R1_TxPacket(data_buffer);
    SI24R1_RX_Mode();

}
