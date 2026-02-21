#include "APP_FreeRTOS.h"

/**
 * STM32F103C8T6 => SRAM 20k, 
 * 操作系统使用configTOTAL_HEAP_SIZE = 17k，
 * 实际使用中根据任务需求调整
 * TASK_STACK_SIZE定义任务堆栈大小，最小推荐128，字节数128*4，实际使用中根据任务需求调整
 */
//内存管理 => C中结构体通常保存在堆栈中，不会自动回收，可以循环使用同个结构体

/**
 * TASK_PRIORITY 定义各个任务优先级，数值越大优先级越高，实际使用中根据任务需求调整
 *@note 0是最低优先级，往往被用作空闲任务，1是正常优先级，2以上是高优先级
 */

//电源管理任务，负责监控电源状态，控制电源开关等
void power_task( void *args );
#define POWER_TASK_STACK_SIZE 128
#define POWER_TASK_PRIORITY 4   //维持电源启动状态，10s执行一次，优先级较高
TaskHandle_t power_task_handle;
#define POWER_TASK_PERIOD_MS 10000  //定义任务周期，单位为ms

//通信任务，负责处理通信相关的操作，如数据发送、接收等
void comm_task( void *args );
#define COMM_TASK_STACK_SIZE 128
#define COMM_TASK_PRIORITY 3   //通信任务优先级，处理通信数据，优先级较高但低于电源管理任务
TaskHandle_t comm_task_handle;
#define COMM_TASK_PERIOD_MS 6  //定义任务周期，单位为ms

//按键任务，负责处理按键相关的操作，如按键检测、按键处理等
void key_task( void *args );
#define KEY_TASK_STACK_SIZE 128
#define KEY_TASK_PRIORITY 2   //按键任务优先级，处理按键事件
TaskHandle_t key_task_handle;
#define KEY_TASK_PERIOD_MS 20  //定义任务周期，单位为ms

//摇杆任务
Joystick_Struct joystick_data = {0}; // 定义一个结构体变量用于存储摇杆数据
void joystick_task( void *args );
#define JOYSTICK_TASK_STACK_SIZE 128
#define JOYSTICK_TASK_PRIORITY 2   //摇杆任务优先级，处理摇杆输入
TaskHandle_t joystick_task_handle;
#define JOYSTICK_TASK_PERIOD_MS 20  //定义任务周期，单位为ms

void APP_FreeRTOS_Start(void){
    //在这里创建任务、队列、信号量等FreeRTOS对象
    //电源管理任务，优先级较高，确保及时响应电源事件
    xTaskCreate(
        power_task,       // 任务函数
        "PowerTask", // 任务名称
        POWER_TASK_STACK_SIZE,         // 堆栈大小
        NULL,        // 任务参数
        POWER_TASK_PRIORITY,           // 任务优先级
        &power_task_handle         // 任务句柄
    );

    //创建其他任务、队列、信号量等对象
    //通信任务，优先级较高但低于电源管理任务，确保及时处理通信数据
    xTaskCreate(
        comm_task,       // 任务函数
        "CommTask", // 任务名称
        COMM_TASK_STACK_SIZE,         // 堆栈大小
        NULL,        // 任务参数
        COMM_TASK_PRIORITY,           // 任务优先级
        &comm_task_handle         // 任务句柄
    );
    
    //按键任务，优先级较低，处理按键事件
    xTaskCreate(
        key_task,       // 任务函数
        "KeyTask", // 任务名称
        KEY_TASK_STACK_SIZE,         // 堆栈大小
        NULL,        // 任务参数
        KEY_TASK_PRIORITY,           // 任务优先级
        &key_task_handle         // 任务句柄
    );

    //摇杆任务，优先级较低，处理摇杆输入
    xTaskCreate(    
        joystick_task,       // 任务函数
        "JoystickTask", // 任务名称
        JOYSTICK_TASK_STACK_SIZE,         // 堆栈大小
        NULL,        // 任务参数
        JOYSTICK_TASK_PRIORITY,           // 任务优先级
        &joystick_task_handle         // 任务句柄
    );

    //启动调度器，开始执行任务
    vTaskStartScheduler();
}



//各任务内容
//电源管理任务，负责监控电源状态，控制电源开关等
void power_task( void *args ){
    //获取当前基准时间
    TickType_t current_tick = xTaskGetTickCount();
    while(1){
        /**每10s执行一次维持电源启动任务，
         * 为了防止机械启动干扰任务，先延时10s，确保机械启动完成后再执行电源维持操作
         */
        vTaskDelayUntil(&current_tick, pdMS_TO_TICKS(POWER_TASK_PERIOD_MS));

        //启动电源
        HAL_GPIO_WritePin(POWER_KEY_GPIO_Port, POWER_KEY_Pin, GPIO_PIN_RESET);
        //根据IP5305T的规格书，保持POWER_KEY引脚低电平至少100ms可以启动电源
        vTaskDelay(pdMS_TO_TICKS(100)); // 延时100ms，确保满足启动条件
        //之后可以根据需要继续保持低电平，或者设置为高电平
        HAL_GPIO_WritePin(POWER_KEY_GPIO_Port, POWER_KEY_Pin, GPIO_PIN_SET);
        
    }
}

//通信任务，负责处理通信相关的操作，如数据发送、接收等
//测试数据缓冲区，用于存储通信数据，测试发送hello world
uint8_t test_data_buffer[TX_PLOAD_WIDTH] = "hello world";

void comm_task( void *args ){
    //获取当前基准时间
    TickType_t current_tick = xTaskGetTickCount();
    while(1){
        //发送数据，先进入TX模式，发送完成后进入RX模式
        SI24R1_TX_Mode();
        //发送数据包
        SI24R1_TxPacket(test_data_buffer);
        SI24R1_RX_Mode();
        /**每6ms执行一次通信任务，处理通信数据，
         * 根据实际通信需求调整任务周期，确保及时处理通信数据
         */
        vTaskDelayUntil(&current_tick, pdMS_TO_TICKS(COMM_TASK_PERIOD_MS));

        //处理通信数据的代码，例如发送或接收数据
        //这里可以添加具体的通信处理逻辑，例如读取传感器数据并发送，或者接收命令并执行等
    }
}

//按键任务，负责处理按键相关的操作，如按键检测、按键处理等

void key_task( void *args ){

    //获取当前基准时间

    TickType_t current_tick = xTaskGetTickCount();

    while(1){
        /**每20ms执行一次按键任务，处理按键事件，
         * 根据实际按键需求调整任务周期，确保及时处理按键事件
         */
        Key_type key_status = Key_Get(); //调用按键扫描函数，获取按键状态
        //打印按键状态
        if(key_status != KEY_NONE){
            LOG_DEBUG("Key Status: %02X\r\n", key_status);
        }
        vTaskDelayUntil(&current_tick, pdMS_TO_TICKS(KEY_TASK_PERIOD_MS));
        //处理按键事件的代码，例如检测按键状态并执行相应操作
        //这里可以添加具体的按键处理逻辑，例如按键按下时发送数据，或者按键长按时执行特定操作等
    }
}

//摇杆任务，负责处理摇杆相关的操作，如摇杆数据读取、处理等
void joystick_task( void *args ){
    //获取当前基准时间
    TickType_t current_tick = xTaskGetTickCount();
    //初始化摇杆数据
    Joystick_Init();    
    while(1){
        /**每20ms执行一次摇杆任务，处理摇杆输入，
         * 根据实际摇杆需求调整任务周期，确保及时处理摇杆输入
         */
        Joystick_GetData(&joystick_data); // 调用获取摇杆数据函数，将数据存储到结构体中
        
        LOG_DEBUG(":%d,%d,%d,%d\n", 
                  joystick_data.thr, joystick_data.yaw, joystick_data.pitch, joystick_data.roll);
        vTaskDelayUntil(&current_tick, pdMS_TO_TICKS(JOYSTICK_TASK_PERIOD_MS));
        
        //处理摇杆事件的代码，例如读取摇杆数据并执行相应操作，VOFA数据观测格式
        //这里可以添加具体的摇杆处理逻辑，例如根据摇杆数据控制飞行器姿态等
    }
}
