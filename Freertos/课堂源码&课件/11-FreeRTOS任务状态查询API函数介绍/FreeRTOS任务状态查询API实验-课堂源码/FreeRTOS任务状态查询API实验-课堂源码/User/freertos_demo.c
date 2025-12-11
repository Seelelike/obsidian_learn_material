/**
 ****************************************************************************************************
 * @file        freertos.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.4
 * @date        2022-01-04
 * @brief       FreeRTOS 移植实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 F407电机开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "freertos_demo.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"
#include "./MALLOC/malloc.h"
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"

/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO         1
#define START_TASK_STACK_SIZE   128
TaskHandle_t    start_task_handler;
void start_task( void * pvParameters );

/* TASK1 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK1_PRIO         2
#define TASK1_STACK_SIZE   128
TaskHandle_t    task1_handler;
void task1( void * pvParameters );


/* TASK2 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK2_PRIO         3
#define TASK2_STACK_SIZE   128
TaskHandle_t    task2_handler;
void task2( void * pvParameters );
/******************************************************************************************************/

/**
 * @brief       FreeRTOS例程入口函数
 * @param       无
 * @retval      无
 */
void freertos_demo(void)
{    
    xTaskCreate((TaskFunction_t         )   start_task,
                (char *                 )   "start_task",
                (configSTACK_DEPTH_TYPE )   START_TASK_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   START_TASK_PRIO,
                (TaskHandle_t *         )   &start_task_handler );
    vTaskStartScheduler();
}


void start_task( void * pvParameters )
{
    taskENTER_CRITICAL();               /* 进入临界区 */
    xTaskCreate((TaskFunction_t         )   task1,
                (char *                 )   "task1",
                (configSTACK_DEPTH_TYPE )   TASK1_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   TASK1_PRIO,
                (TaskHandle_t *         )   &task1_handler );
                
    xTaskCreate((TaskFunction_t         )   task2,
                (char *                 )   "task2",
                (configSTACK_DEPTH_TYPE )   TASK2_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   TASK2_PRIO,
                (TaskHandle_t *         )   &task2_handler );
    vTaskDelete(NULL);
    taskEXIT_CRITICAL();                /* 退出临界区 */
}

/* 任务一，实现LED0每500ms翻转一次 */
void task1( void * pvParameters )
{
    while(1)
    {
        LED0_TOGGLE();
        vTaskDelay(500);
    }
}

char task_buff[500];
/* 任务二，实现任务状态查询API函数使用 */
void task2( void * pvParameters )
{
    UBaseType_t priority_num = 0;
    UBaseType_t task_num = 0;
    UBaseType_t task_num2 = 0;
    TaskStatus_t * status_array = 0;
    TaskStatus_t * status_array2 = 0;
    TaskHandle_t task_handle = 0;
    UBaseType_t task_stack_min = 0;
    eTaskState state = 0;
    
    uint8_t i = 0;
    
    vTaskPrioritySet( task2_handler,4 );
    priority_num = uxTaskPriorityGet( NULL );
    printf("task2任务优先级为%ld\r\n",priority_num);
    
    task_num = uxTaskGetNumberOfTasks();
    printf("任务数量：%ld\r\n",task_num);
    
    status_array = mymalloc(SRAMIN,(sizeof(TaskStatus_t) * task_num));
    task_num2 = uxTaskGetSystemState( status_array,task_num,NULL);
    printf("任务名\t\t任务优先级\t任务编号\r\n");
    for(i = 0; i < task_num2; i++)
    {
        printf("%s\t\t%ld\t%ld\r\n",
                status_array[i].pcTaskName,
                status_array[i].uxCurrentPriority,
                status_array[i].xTaskNumber);
    }
    
    status_array2 = mymalloc(SRAMIN,sizeof(TaskStatus_t));
    vTaskGetInfo( task2_handler,status_array2,pdTRUE,eInvalid);
    printf("任务名：%s\r\n",status_array2->pcTaskName);
    printf("任务优先级：%ld\r\n",status_array2->uxCurrentPriority);
    printf("任务编号：%ld\r\n",status_array2->xTaskNumber);
    printf("任务状态：%d\r\n",status_array2->eCurrentState);
    
    task_handle = xTaskGetHandle( "task1" );
    printf("任务句柄：%#x\r\n",(int)task_handle);
    printf("task1的任务句柄：%#x\r\n",(int)task1_handler);
    
    state = eTaskGetState( task2_handler );
    printf("当前task2的任务状态为：%d\r\n",state);
    
    vTaskList( task_buff );
    printf("%s\r\n",task_buff);
    while(1)
    {
//        task_stack_min = uxTaskGetStackHighWaterMark( task2_handler );
//        printf("task2历史剩余最小堆栈为%ld\r\n",task_stack_min);
        vTaskDelay(1000);
    }
}

