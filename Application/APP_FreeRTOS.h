#ifndef _APP_FREERTOS_H_
#define _APP_FREERTOS_H_

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "Com_debug.h"
#include "Int_IP5305T.h"

/**
 *@brief  FreeRTOS启动函数
 */
void APP_FreeRTOS_Start(void);

/**
 * @brief Task1声明
 */
void task1( void *args );


#endif
