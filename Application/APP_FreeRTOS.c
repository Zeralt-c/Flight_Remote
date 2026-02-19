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

//

//
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
    /*
    xTaskCreate(
        task2,       // 任务函数
        "Task2", // 任务名称
        TASK2_STACK_SIZE,         // 堆栈大小
        NULL,        // 任务参数
        TASK2_PRIORITY,           // 任务优先级
        &task2_handle         // 任务句柄
    );
    */
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
        vTaskDelayUntil(&current_tick, pdMS_TO_TICKS(10000));

        //启动电源
        HAL_GPIO_WritePin(POWER_KEY_GPIO_Port, POWER_KEY_Pin, GPIO_PIN_RESET);
        //根据IP5305T的规格书，保持POWER_KEY引脚低电平至少100ms可以启动电源
        vTaskDelay(pdMS_TO_TICKS(100)); // 延时100ms，确保满足启动条件
        //之后可以根据需要继续保持低电平，或者设置为高电平
        HAL_GPIO_WritePin(POWER_KEY_GPIO_Port, POWER_KEY_Pin, GPIO_PIN_SET);
        
    }
}

void task2( void *args ){
    while(1){
        LOG_DEBUG("Task2 is running");
        vTaskDelay(pdMS_TO_TICKS(1500)); // 延时1.5秒
    }
}

