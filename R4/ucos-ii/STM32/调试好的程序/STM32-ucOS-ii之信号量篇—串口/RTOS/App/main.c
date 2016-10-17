#include "includes.h"

static OS_STK   TaskStartStk[TASK_START_STK_SIZE];	// 定义起始任务堆栈大小
static OS_STK   Task1Stk[TASK1_STK_SIZE];			// 定义任务堆栈大小
static OS_STK   Task2Stk[TASK2_STK_SIZE];
static OS_STK   Task3Stk[TASK3_STK_SIZE];
static OS_STK   Task4Stk[TASK4_STK_SIZE];

static void TaskStart(void *p_arg);
static void Task1(void *p_arg);
static void Task2(void *p_arg);
static void Task3(void *p_arg);
static void Task4(void *p_arg);
void Delay(vu32 nCount); 
void Delayms (vu32 m);
OS_EVENT *mutex;
OS_EVENT *key_Sem1;
OS_EVENT *key_Sem2;
int main(void)
{
	INT8U err;
    OSInit();	 //初始化 uC/OS-II 
	mutex=OSMutexCreate(3,&err);	// 建立互斥信号量
	key_Sem1 = OSSemCreate(0);  	    // 建立信号量  起始值为0 
	key_Sem2 = OSSemCreate(0);  	    // 建立信号量  起始值为0 
    OSTaskCreate(TaskStart,(void*)0,&TaskStartStk[TASK_START_STK_SIZE-1],1); // 创建起始任务，赋予最高优先级1 
    OSStart();					   //开始多任务
	
}

/*
********************************************************************************
**起始任务，优先级1
********************************************************************************
*/
static void TaskStart(void *p_arg)
{
    (void)p_arg;
    BSP_Init();
   
#if (OS_TASK_STAT_EN > 0)
     OSStatInit();
#endif
    OSTaskCreate(Task1,(void*)0,&Task1Stk[TASK1_STK_SIZE-1],2);  // 在起始任务中创建Task1，优先级2 
    OSTaskCreate(Task2,(void*)0,&Task2Stk[TASK2_STK_SIZE-1],4);  // 在起始任务中创建Task2，优先级4
	OSTaskCreate(Task3,(void*)0,&Task3Stk[TASK3_STK_SIZE-1],5);  // 在起始任务中创建Task1，优先级5 
    OSTaskCreate(Task4,(void*)0,&Task4Stk[TASK4_STK_SIZE-1],6);  // 在起始任务中创建Task2，优先级6
	 
	D1_H;D2_H;D3_H;D4_H;D5_H;D6_H;D7_H;D1_H;D8_H;D9_H;D10_H;D11_H;D12_H;D13_H;D14_H;D15_H;D16_H;D18_H;

    while(1)
    {
	   OSTimeDly(10);
       OSTaskSuspend(OS_PRIO_SELF);	  // 挂起自身
    }    
}
/*
********************************************************************************
**任务1，优先级2
********************************************************************************
*/
static void Task1(void *p_arg)
{     
   while(1)
    {
		INT8U err;
 		OSTimeDlyHMSM(0,0,1,0);
		OSSemPend(key_Sem1,0,&err);  // 等待信号量被释放
		 OSSemPend(key_Sem2,0,&err);  // 等待信号量被释放
		USART_SendData(USART1, 0x01);
		USART_SendData(USART1, 0x01);        
			    
     }
}
/*
********************************************************************************
**任务2，优先级3
********************************************************************************
*/
static void Task2(void *p_arg)
{	
	INT8U err;
    while(1)
    {

		 OSTimeDlyHMSM(0,0,1,0);		 
         USART_SendData(USART1, 0x02);
		 USART_SendData(USART1, 0x02);
		 OSSemPost(key_Sem1);		 // 释放信号量
     }
}

/*
********************************************************************************
**任务3，优先级4
********************************************************************************
*/
static void Task3(void *p_arg)
{
     INT8U err;
	while(1)
    {	
		 //OSSemPend(key_Sem2,0,&err);  // 等待信号量被释放
		 OSTimeDlyHMSM(0,0,1,0);		 
         USART_SendData(USART1, 0x03);
	     USART_SendData(USART1, 0x03);
		 //OSSemPost(key_Sem);		 // 释放信号量

     }
}

/*
********************************************************************************
**任务4，优先级5
********************************************************************************
*/
static void Task4(void *p_arg)
{
	 INT8U err;
     while(1)
     {
		OSTimeDlyHMSM(0,0,1,0);
	  	//OSSemPend(key_Sem,0,&err);  // 等待信号量被释放	
        USART_SendData(USART1, 0x04);
		USART_SendData(USART1, 0x04);
		OSSemPost(key_Sem2);		 // 释放信号量
		                              
       		
      }
}


/*
********************************************************************************
** 函数名称 ： Delay(vu32 nCount)
** 函数功能 ： 短延时函数	  i=1;40us延时
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
/* 
 void Delay(vu32 t)
{
  u8 j,k;
   for(k=0;k<t;k++)
      for(j=0;j<255;j++);
}
*/
 void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}
/*
********************************************************************************
** 函数名称 ： void Delayms(vu32 m)
** 函数功能 ： 长延时函数	 m=1,延时1ms
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
 void Delayms(vu32 m)
{
  u32 i;
  
  for(; m != 0; m--)	
       for (i=0; i<50000; i++);
}

