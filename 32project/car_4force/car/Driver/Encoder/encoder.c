#include "encoder.h"
//从前到后，从左到右，分别命名为Encoder1~Encoder4

#define Encoder1_RCC_APBxPeriph_TIMx RCC_APB1Periph_TIM3
#define Encoder1_RCC_APBxPeriph_GPIOx RCC_APB2Periph_GPIOA
#define Encoder1_GPIOx GPIOA
#define Encoder1_TIMx TIM3
#define Encoder_1A GPIO_Pin_6//TIM3 PA
#define Encoder_1B GPIO_Pin_7

#define Encoder2_RCC_APBxPeriph_TIMx RCC_APB1Periph_TIM4
#define Encoder2_RCC_APBxPeriph_GPIOx RCC_APB2Periph_GPIOD
#define Encoder2_GPIOx GPIOD
#define Encoder2_TIMx TIM4
#define Encoder_2A GPIO_Pin_12//TIM4 PD   因为PB6和PB7被IIC占用，故用其他io口
#define Encoder_2B GPIO_Pin_13

#define Encoder3_RCC_APBxPeriph_TIMx RCC_APB1Periph_TIM5
#define Encoder3_RCC_APBxPeriph_GPIOx RCC_APB2Periph_GPIOA
#define Encoder3_GPIOx GPIOA
#define Encoder3_TIMx TIM5
#define Encoder_3A GPIO_Pin_0//TIM5 PA
#define Encoder_3B GPIO_Pin_1

#define Encoder4_RCC_APBxPeriph_TIMx RCC_APB2Periph_TIM8
#define Encoder4_RCC_APBxPeriph_GPIOx RCC_APB2Periph_GPIOC
#define Encoder4_GPIOx GPIOC
#define Encoder4_TIMx TIM8
#define Encoder_4A GPIO_Pin_6//TIM8 PC
#define Encoder_4B GPIO_Pin_7

/***********************************************************
函数名称：void Encoder_Init(void)
函数功能：初始化编码器模块（启动时钟、配置GPIO、时基单元、
                            输入捕获单元、编码器接口、启动定时器）
入口参数：无
返回参数：无
***********************************************************/
void Encoder_Init(void)
{
    //启动编码器TIM时钟
    RCC_APB1PeriphClockCmd(Encoder1_RCC_APBxPeriph_TIMx,ENABLE);
    RCC_APB1PeriphClockCmd(Encoder2_RCC_APBxPeriph_TIMx,ENABLE);
    RCC_APB1PeriphClockCmd(Encoder3_RCC_APBxPeriph_TIMx,ENABLE);
    RCC_APB2PeriphClockCmd(Encoder4_RCC_APBxPeriph_TIMx,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   //重映射必须要开AFIO时钟

    GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);//将TIM4的引脚重映射
    
    //启动GPIO时钟
    RCC_APB2PeriphClockCmd(Encoder1_RCC_APBxPeriph_GPIOx,ENABLE);
    RCC_APB2PeriphClockCmd(Encoder2_RCC_APBxPeriph_GPIOx,ENABLE);
    RCC_APB2PeriphClockCmd(Encoder3_RCC_APBxPeriph_GPIOx,ENABLE);
    RCC_APB2PeriphClockCmd(Encoder4_RCC_APBxPeriph_GPIOx,ENABLE);
    
    //配置GPIO为输入上拉
    GPIO_InitTypeDef Encoder_GPIO_InitStructure;
    Encoder_GPIO_InitStructure.GPIO_Pin = Encoder_1A | Encoder_1B;
    Encoder_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    Encoder_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(Encoder1_GPIOx,&Encoder_GPIO_InitStructure);
    
    Encoder_GPIO_InitStructure.GPIO_Pin = Encoder_2A | Encoder_2B;
    GPIO_Init(Encoder2_GPIOx,&Encoder_GPIO_InitStructure);
    
    Encoder_GPIO_InitStructure.GPIO_Pin = Encoder_3A | Encoder_3B;
    GPIO_Init(Encoder3_GPIOx,&Encoder_GPIO_InitStructure);
    
    Encoder_GPIO_InitStructure.GPIO_Pin = Encoder_4A | Encoder_4B;
    GPIO_Init(Encoder4_GPIOx,&Encoder_GPIO_InitStructure);
    
    //配置时基单元
    TIM_TimeBaseInitTypeDef Encoder_TimeBaseInitStructure;    
    Encoder_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    Encoder_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    Encoder_TimeBaseInitStructure.TIM_Period = 65535 - 1;//ARR
    Encoder_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;//PSC
    Encoder_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//重装值
    TIM_TimeBaseInit(Encoder1_TIMx,&Encoder_TimeBaseInitStructure);//配Encoder1的时基单元
    TIM_TimeBaseInit(Encoder2_TIMx,&Encoder_TimeBaseInitStructure);//配Encoder2的时基单元
    TIM_TimeBaseInit(Encoder3_TIMx,&Encoder_TimeBaseInitStructure);//配Encoder3的时基单元
    TIM_TimeBaseInit(Encoder4_TIMx,&Encoder_TimeBaseInitStructure);//配Encoder4的时基单元
    
    //配置输入捕获单元
    TIM_ICInitTypeDef Encoder_ICInitStructure;
    TIM_ICStructInit(&Encoder_ICInitStructure);
    
    Encoder_ICInitStructure.TIM_Channel = TIM_Channel_1;
    Encoder_ICInitStructure.TIM_ICFilter = 0xF;
    Encoder_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInit(Encoder1_TIMx,&Encoder_ICInitStructure);
    Encoder_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInit(Encoder1_TIMx,&Encoder_ICInitStructure);//配置Encoder1的输入捕获单元
    
    Encoder_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInit(Encoder2_TIMx,&Encoder_ICInitStructure);
    Encoder_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInit(Encoder2_TIMx,&Encoder_ICInitStructure);//配置Encoder2的输入捕获单元
    
    Encoder_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInit(Encoder3_TIMx,&Encoder_ICInitStructure);
    Encoder_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInit(Encoder3_TIMx,&Encoder_ICInitStructure);//配置Encoder3的输入捕获单元
    
    Encoder_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInit(Encoder4_TIMx,&Encoder_ICInitStructure);
    Encoder_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInit(Encoder4_TIMx,&Encoder_ICInitStructure);//配置Encoder4的输入捕获单元
    
    //配置编码器接口
    TIM_EncoderInterfaceConfig(Encoder1_TIMx,TIM_EncoderMode_TI12,
                        TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//配置Encoder1的编码器接口
    TIM_EncoderInterfaceConfig(Encoder2_TIMx,TIM_EncoderMode_TI12,
                        TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//配置Encoder2的编码器接口
    TIM_EncoderInterfaceConfig(Encoder3_TIMx,TIM_EncoderMode_TI12,
                        TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//配置Encoder3的编码器接口
    TIM_EncoderInterfaceConfig(Encoder4_TIMx,TIM_EncoderMode_TI12,
                        TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//配置Encoder4的编码器接口
    
    //启动编码器的定时器  TIM3 TIM4 TIM5 TIM8
    TIM_Cmd(Encoder1_TIMx,ENABLE);
    TIM_Cmd(Encoder2_TIMx,ENABLE);
    TIM_Cmd(Encoder3_TIMx,ENABLE);
    TIM_Cmd(Encoder4_TIMx,ENABLE);
}

/***********************************************************
函数名称：uint16_t Encoder_GetConter(uint16_t Encoderx)
函数功能：获取编码器数据
入口参数：Encoderx  取值是1~4 对应4个编码器
返回参数：编码器TIM的计数值
***********************************************************/
    int16_t Encoder_temp = 0;

uint16_t Encoder_GetConter(uint16_t Encoderx)
{

    switch(Encoderx)
    {
        case 1:
            Encoder_temp = TIM_GetCounter(Encoder1_TIMx);
            TIM_SetCounter(Encoder1_TIMx,0);
            return Encoder_temp;

        
        case 2:
            Encoder_temp = TIM_GetCounter(Encoder2_TIMx);
            TIM_SetCounter(Encoder2_TIMx,0);
            return Encoder_temp;

        
        case 3:
            Encoder_temp = TIM_GetCounter(Encoder3_TIMx);
            TIM_SetCounter(Encoder3_TIMx,0);
            return Encoder_temp;

        
        case 4:
            Encoder_temp = TIM_GetCounter(Encoder4_TIMx);
            TIM_SetCounter(Encoder4_TIMx,0);
            return Encoder_temp;
    }
    return 0;
}
