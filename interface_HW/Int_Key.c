#include "Int_Key.h"

//使用了vtaskDelay()函数，需要在FreeRTOS任务中使用
Key_type Key_Get(void){
    Key_type key_state = KEY_NONE;
    if(HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin) == GPIO_PIN_RESET){
        //电弧和机械抖动，需要消抖
        vTaskDelay(pdMS_TO_TICKS(10)); //延时10ms，等待抖动结束
        //再次读取按键状态，确认按键是否仍然按下
        if(HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin) == GPIO_PIN_RESET){
            //等待按键释放
            while(HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin) == GPIO_PIN_RESET){
                vTaskDelay(pdMS_TO_TICKS(1));//防堵塞
            }  
            key_state |= KEY_UP;
        }
    }
    if(HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin) == GPIO_PIN_RESET){
        vTaskDelay(pdMS_TO_TICKS(10)); //延时10ms，等待抖动结束
        if(HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin) == GPIO_PIN_RESET){
            while(HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin) == GPIO_PIN_RESET){
                vTaskDelay(pdMS_TO_TICKS(1));//防堵塞
            }   
            key_state |= KEY_DOWN;
        }
    }
    if(HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port, KEY_LEFT_Pin) == GPIO_PIN_RESET){
        vTaskDelay(pdMS_TO_TICKS(10)); //延时10ms，等待抖动结束
        if(HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port, KEY_LEFT_Pin) == GPIO_PIN_RESET){
            while(HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port, KEY_LEFT_Pin) == GPIO_PIN_RESET){
                vTaskDelay(pdMS_TO_TICKS(1));//防堵塞
            }   
            key_state |= KEY_LEFT;
        }
    }
    if(HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port, KEY_RIGHT_Pin) == GPIO_PIN_RESET){
        vTaskDelay(pdMS_TO_TICKS(10)); //延时10ms，等待抖动结束
        if(HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port, KEY_RIGHT_Pin) == GPIO_PIN_RESET){
            while(HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port, KEY_RIGHT_Pin) == GPIO_PIN_RESET){
                vTaskDelay(pdMS_TO_TICKS(1));//防堵塞
            }   
            key_state |= KEY_RIGHT;
        }
    }
    // 左X键开关机
    if(HAL_GPIO_ReadPin(KEY_LEFT_X_GPIO_Port, KEY_LEFT_X_Pin) == GPIO_PIN_RESET){
        vTaskDelay(pdMS_TO_TICKS(10)); //延时10ms，等待抖动结束
        if(HAL_GPIO_ReadPin(KEY_LEFT_X_GPIO_Port, KEY_LEFT_X_Pin) == GPIO_PIN_RESET){            
            while(HAL_GPIO_ReadPin(KEY_LEFT_X_GPIO_Port, KEY_LEFT_X_Pin) == GPIO_PIN_RESET){
                vTaskDelay(pdMS_TO_TICKS(1));//防堵塞
            }             
            key_state |= KEY_LEFT_X;
        }
    }
    // 右X键，需要区分长按和短按
    if(HAL_GPIO_ReadPin(KEY_RIGHT_X_GPIO_Port, KEY_RIGHT_X_Pin) == GPIO_PIN_RESET){
        vTaskDelay(pdMS_TO_TICKS(10)); //延时10ms，等待抖动结束
        if(HAL_GPIO_ReadPin(KEY_RIGHT_X_GPIO_Port, KEY_RIGHT_X_Pin) == GPIO_PIN_RESET){
            TickType_t press_start_tick = xTaskGetTickCount(); //记录按键按下的起始时间
            while(HAL_GPIO_ReadPin(KEY_RIGHT_X_GPIO_Port, KEY_RIGHT_X_Pin) == GPIO_PIN_RESET){
                vTaskDelay(pdMS_TO_TICKS(1));//防堵塞
            } 
            TickType_t press_duration = xTaskGetTickCount() - press_start_tick; //计算按键持续时间
            if(press_duration >= pdMS_TO_TICKS(1000)){ //长按超过1秒
                key_state |= KEY_RIGHT_X_LONG;
            } else { //短按
                key_state |= KEY_RIGHT_X_SHORT;
            }
        }
    }
    return key_state;
}
