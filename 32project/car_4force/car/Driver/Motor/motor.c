#include "motor.h"

#define PWMA_TIMx TIM2
#define PWMB_TIMx TIM2
#define PWMC_TIMx TIM2
#define PWMD_TIMx TIM2
#define PWMA_Timer RCC_APB1Periph_TIM2
#define PWMB_Timer RCC_APB1Periph_TIM2
#define PWMC_Timer RCC_APB1Periph_TIM2
#define PWMD_Timer RCC_APB1Periph_TIM2
#define PWMA GPIO_Pin_15
#define PWMB GPIO_Pin_3  //因为冲突，将TIM2的CH1和CH2重映射，CH1接到PA15  CH2接到PB3
#define PWMC GPIO_Pin_2
#define PWMD GPIO_Pin_3
#define PWMA_Port GPIOA
#define PWMB_Port GPIOB
#define PWMC_Port GPIOA
#define PWMD_Port GPIOA
#define IN_Timer RCC_APB2Periph_GPIOE
#define AIN1 GPIO_Pin_7
#define AIN2 GPIO_Pin_8
#define BIN1 GPIO_Pin_9
#define BIN2 GPIO_Pin_10
#define CIN1 GPIO_Pin_11
#define CIN2 GPIO_Pin_12
#define DIN1 GPIO_Pin_13
#define DIN2 GPIO_Pin_14
#define IN_Port GPIOE
#define AIN1_Port GPIOE
#define AIN2_Port GPIOE
#define BIN1_Port GPIOE
#define BIN2_Port GPIOE
#define CIN1_Port GPIOE
#define CIN2_Port GPIOE
#define DIN1_Port GPIOE
#define DIN2_Port GPIOE

//初始化电机相关的引脚，且设置TIM为PWM输出   分为PWM引脚的初始化和电机驱动引脚的初始化
void Motor_Init(void)
{
    
    RCC_APB1PeriphClockCmd(PWMA_Timer,ENABLE);//启动PWMA输出
    RCC_APB1PeriphClockCmd(PWMB_Timer,ENABLE);//启动PWMB输出
    RCC_APB1PeriphClockCmd(PWMC_Timer,ENABLE);//启动PWMC输出
    RCC_APB1PeriphClockCmd(PWMD_Timer,ENABLE);//启动PWMD输出
    RCC_APB2PeriphClockCmd(IN_Timer,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//重映射必须开启AFIO时钟
    GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);//将TIM2的CH1和CH2重映射
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    
    GPIO_InitTypeDef Motor_GPIO_InitStructure;
    Motor_GPIO_InitStructure.GPIO_Pin = AIN1 | AIN2 | BIN1 | BIN2 | CIN1 | CIN2 | DIN1 |DIN2;
    Motor_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    Motor_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(IN_Port,&Motor_GPIO_InitStructure);//初始化方向驱动端口
    
    Motor_GPIO_InitStructure.GPIO_Pin =PWMA;
    Motor_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    Motor_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PWMA_Port,&Motor_GPIO_InitStructure);//初始化PWMA输出端口
    Motor_GPIO_InitStructure.GPIO_Pin =PWMB;
    Motor_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(PWMB_Port,&Motor_GPIO_InitStructure);//初始化PWMB输出端口
    Motor_GPIO_InitStructure.GPIO_Pin =PWMC;
    Motor_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(PWMC_Port,&Motor_GPIO_InitStructure);//初始化PWMC输出端口
    Motor_GPIO_InitStructure.GPIO_Pin =PWMD;
    Motor_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(PWMD_Port,&Motor_GPIO_InitStructure);//初始化PWMD输出端口
                               
    TIM_InternalClockConfig(PWMA_TIMx);//选择PWMA内部时钟
    TIM_InternalClockConfig(PWMB_TIMx);//选择PWMB内部时钟
    TIM_InternalClockConfig(PWMC_TIMx);//选择PWMC内部时钟
    TIM_InternalClockConfig(PWMD_TIMx);//选择PWMD内部时钟
    
    TIM_TimeBaseInitTypeDef Motor_TimeBase_InitStructure;
    Motor_TimeBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    Motor_TimeBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    Motor_TimeBase_InitStructure.TIM_Period = 100 - 1;//ARR  周期
    Motor_TimeBase_InitStructure.TIM_Prescaler = 720 - 1;//PSC  预分频数
    Motor_TimeBase_InitStructure.TIM_RepetitionCounter = 0;//重装值
    TIM_TimeBaseInit(PWMA_TIMx,&Motor_TimeBase_InitStructure);//初始化时基单元
    TIM_TimeBaseInit(PWMB_TIMx,&Motor_TimeBase_InitStructure);
    TIM_TimeBaseInit(PWMC_TIMx,&Motor_TimeBase_InitStructure);
    TIM_TimeBaseInit(PWMD_TIMx,&Motor_TimeBase_InitStructure);
    
    TIM_OCInitTypeDef Motor_TIM_OCIniterStructure;
    TIM_OCStructInit(&Motor_TIM_OCIniterStructure);
    Motor_TIM_OCIniterStructure.TIM_OCMode = TIM_OCMode_PWM1;//输出模式
    Motor_TIM_OCIniterStructure.TIM_OCPolarity = TIM_OCPolarity_High;//高极性
    Motor_TIM_OCIniterStructure.TIM_OutputState = TIM_OutputState_Enable;//使能输出比较
    Motor_TIM_OCIniterStructure.TIM_Pulse = 0;//CCR    默认0占空比
    TIM_OC1Init(PWMA_TIMx,&Motor_TIM_OCIniterStructure);//初始化4个通道的输出比较
    TIM_OC2Init(PWMB_TIMx,&Motor_TIM_OCIniterStructure);
    TIM_OC3Init(PWMC_TIMx,&Motor_TIM_OCIniterStructure);
    TIM_OC4Init(PWMD_TIMx,&Motor_TIM_OCIniterStructure);
    
    TIM_Cmd(PWMA_TIMx,ENABLE);//启动定时器
    TIM_Cmd(PWMB_TIMx,ENABLE);
    TIM_Cmd(PWMC_TIMx,ENABLE);
    TIM_Cmd(PWMD_TIMx,ENABLE);
}
   
void Motor_SetSpeed(uint16_t Celect_Motor,int16_t Speed)
{
    int16_t Value = Speed;
    
    switch(Celect_Motor)
    {
        case 1:
            if(Value > 0)//正转
            {
                GPIO_SetBits(AIN2_Port,AIN2);
                GPIO_ResetBits(AIN1_Port,AIN1);
                TIM_SetCompare1(PWMA_TIMx,Value);
            }
            else if(Value < 0 )//反转
            {
                GPIO_SetBits(AIN1_Port,AIN1);
                GPIO_ResetBits(AIN2_Port,AIN2);
                Value = -Value;
                TIM_SetCompare1(PWMA_TIMx,Value);
            }
            else//0就停止
            {
                GPIO_ResetBits(AIN1_Port,AIN1);
                GPIO_ResetBits(AIN2_Port,AIN2);
            }
            
        break;
        
        case 2:
            if(Value > 0)//正转
            {
                GPIO_SetBits(BIN1_Port,BIN1);
                GPIO_ResetBits(BIN2_Port,BIN2);
                TIM_SetCompare2(PWMB_TIMx,Value);
            }
            else if(Value < 0 )//反转
            {
                GPIO_SetBits(BIN2_Port,BIN2);
                GPIO_ResetBits(BIN1_Port,BIN1);
                Value = -Value;
                TIM_SetCompare2(PWMB_TIMx,Value);
            }
            else//0就停止
            {
                GPIO_ResetBits(BIN1_Port,BIN1);
                GPIO_ResetBits(BIN2_Port,BIN2);
            }
            
        break;
        
        case 3:
            if(Value > 0)//正转
            {
                GPIO_SetBits(CIN2_Port,CIN2);
                GPIO_ResetBits(CIN1_Port,CIN1);
                TIM_SetCompare3(PWMC_TIMx,Value);
            }
            else if(Value < 0 )//反转
            {
                GPIO_SetBits(CIN1_Port,CIN1);
                GPIO_ResetBits(CIN2_Port,CIN2);
                Value = -Value;
                TIM_SetCompare3(PWMC_TIMx,Value);
            }
            else//0就停止
            {
                GPIO_ResetBits(CIN1_Port,CIN1);
                GPIO_ResetBits(CIN2_Port,CIN2);
            }
            
        break;
        
        case 4:
            if(Value > 0)//正转
            {
                GPIO_SetBits(DIN1_Port,DIN1);
                GPIO_ResetBits(DIN2_Port,DIN2);
                TIM_SetCompare4(PWMD_TIMx,Value);
            }
            else if(Value < 0 )//反转
            {
                GPIO_SetBits(DIN2_Port,DIN2);
                GPIO_ResetBits(DIN1_Port,DIN1);
                Value = -Value;
                TIM_SetCompare4(PWMD_TIMx,Value);
            }
            else//0就停止
            {
                GPIO_ResetBits(DIN1_Port,DIN1);
                GPIO_ResetBits(DIN2_Port,DIN2);
            }
            
        break;
    }
    
}

//前进
void Motor_Run(uint16_t Speed)
{
    Motor_SetSpeed(1,Speed);
    Motor_SetSpeed(2,Speed);
    Motor_SetSpeed(3,Speed);
    Motor_SetSpeed(4,Speed);
}

//后退
void Motor_Back(uint16_t Speed)
{
    Motor_SetSpeed(1,-Speed);
    Motor_SetSpeed(2,-Speed);
    Motor_SetSpeed(3,-Speed);
    Motor_SetSpeed(4,-Speed);    
}

//左平移
void Motor_LeftMove(uint16_t Speed)
{
    Motor_SetSpeed(1,-Speed);
    Motor_SetSpeed(2,Speed);
    Motor_SetSpeed(3,Speed);
    Motor_SetSpeed(4,-Speed);    
}

//右平移
void Motor_RightMove(uint16_t Speed)
{
    Motor_SetSpeed(1,Speed);
    Motor_SetSpeed(2,-Speed);
    Motor_SetSpeed(3,-Speed);
    Motor_SetSpeed(4,Speed);    
}

//左上移   2，3轮前进，其余不动
void Motor_LeftUp(uint16_t Speed)
{
    Motor_SetSpeed(1,0);
    Motor_SetSpeed(2,Speed);
    Motor_SetSpeed(3,Speed);
    Motor_SetSpeed(4,0);   
}


//左下移    2，3轮后退，其余不动
void Motor_LeftDown(uint16_t Speed)
{
    Motor_SetSpeed(1,-Speed);
    Motor_SetSpeed(2,0);
    Motor_SetSpeed(3,0);
    Motor_SetSpeed(4,-Speed);     
}

//右上移    1，4轮前进，其余不动
void Motor_RightUp(uint16_t Speed)
{
    Motor_SetSpeed(1,Speed);
    Motor_SetSpeed(2,0);
    Motor_SetSpeed(3,0);
    Motor_SetSpeed(4,Speed);   
}


//右下移       1，4轮后退，其余不动
void Motor_RightDown(uint16_t Speed)
{
    Motor_SetSpeed(1,0);
    Motor_SetSpeed(2,-Speed);
    Motor_SetSpeed(3,-Speed);
    Motor_SetSpeed(4,0);
}

//向左转动      2，4前进，1，3后退
void Motor_LetfTurn(uint16_t Speed)
{
    Motor_SetSpeed(1,-Speed);
    Motor_SetSpeed(2,Speed);
    Motor_SetSpeed(3,-Speed);
    Motor_SetSpeed(4,Speed);   
}


//向右转动      1，3前进，2，4后退
void Motor_RightTurn(uint16_t Speed)
{
    Motor_SetSpeed(1,Speed);
    Motor_SetSpeed(2,-Speed);
    Motor_SetSpeed(3,Speed);
    Motor_SetSpeed(4,-Speed);   
}

void test(void)
{
    for(uint16_t i = 10;i<100;i+=10)
    {
        Motor_Run(i);
        DelayS(2);
        Motor_Back(i);
        DelayS(2);
        Motor_LeftMove(i);
        DelayS(2);
        Motor_RightMove(i);
        DelayS(2);
        Motor_LeftUp(i);
        DelayS(2);
        Motor_LeftDown(i);
        DelayS(2);
        Motor_RightUp(i);
        DelayS(2);
        Motor_RightDown(i);
        DelayS(2);
        Motor_LetfTurn(i);
        DelayS(2);
        Motor_RightTurn(i);
        DelayS(2);  
    }
    
}
