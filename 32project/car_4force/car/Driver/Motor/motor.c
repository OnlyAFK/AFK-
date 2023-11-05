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
#define PWMB GPIO_Pin_3  //��Ϊ��ͻ����TIM2��CH1��CH2��ӳ�䣬CH1�ӵ�PA15  CH2�ӵ�PB3
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

//��ʼ�������ص����ţ�������TIMΪPWM���   ��ΪPWM���ŵĳ�ʼ���͵���������ŵĳ�ʼ��
void Motor_Init(void)
{
    
    RCC_APB1PeriphClockCmd(PWMA_Timer,ENABLE);//����PWMA���
    RCC_APB1PeriphClockCmd(PWMB_Timer,ENABLE);//����PWMB���
    RCC_APB1PeriphClockCmd(PWMC_Timer,ENABLE);//����PWMC���
    RCC_APB1PeriphClockCmd(PWMD_Timer,ENABLE);//����PWMD���
    RCC_APB2PeriphClockCmd(IN_Timer,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//��ӳ����뿪��AFIOʱ��
    GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);//��TIM2��CH1��CH2��ӳ��
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    
    GPIO_InitTypeDef Motor_GPIO_InitStructure;
    Motor_GPIO_InitStructure.GPIO_Pin = AIN1 | AIN2 | BIN1 | BIN2 | CIN1 | CIN2 | DIN1 |DIN2;
    Motor_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    Motor_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(IN_Port,&Motor_GPIO_InitStructure);//��ʼ�����������˿�
    
    Motor_GPIO_InitStructure.GPIO_Pin =PWMA;
    Motor_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    Motor_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PWMA_Port,&Motor_GPIO_InitStructure);//��ʼ��PWMA����˿�
    Motor_GPIO_InitStructure.GPIO_Pin =PWMB;
    Motor_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(PWMB_Port,&Motor_GPIO_InitStructure);//��ʼ��PWMB����˿�
    Motor_GPIO_InitStructure.GPIO_Pin =PWMC;
    Motor_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(PWMC_Port,&Motor_GPIO_InitStructure);//��ʼ��PWMC����˿�
    Motor_GPIO_InitStructure.GPIO_Pin =PWMD;
    Motor_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(PWMD_Port,&Motor_GPIO_InitStructure);//��ʼ��PWMD����˿�
                               
    TIM_InternalClockConfig(PWMA_TIMx);//ѡ��PWMA�ڲ�ʱ��
    TIM_InternalClockConfig(PWMB_TIMx);//ѡ��PWMB�ڲ�ʱ��
    TIM_InternalClockConfig(PWMC_TIMx);//ѡ��PWMC�ڲ�ʱ��
    TIM_InternalClockConfig(PWMD_TIMx);//ѡ��PWMD�ڲ�ʱ��
    
    TIM_TimeBaseInitTypeDef Motor_TimeBase_InitStructure;
    Motor_TimeBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    Motor_TimeBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    Motor_TimeBase_InitStructure.TIM_Period = 100 - 1;//ARR  ����
    Motor_TimeBase_InitStructure.TIM_Prescaler = 720 - 1;//PSC  Ԥ��Ƶ��
    Motor_TimeBase_InitStructure.TIM_RepetitionCounter = 0;//��װֵ
    TIM_TimeBaseInit(PWMA_TIMx,&Motor_TimeBase_InitStructure);//��ʼ��ʱ����Ԫ
    TIM_TimeBaseInit(PWMB_TIMx,&Motor_TimeBase_InitStructure);
    TIM_TimeBaseInit(PWMC_TIMx,&Motor_TimeBase_InitStructure);
    TIM_TimeBaseInit(PWMD_TIMx,&Motor_TimeBase_InitStructure);
    
    TIM_OCInitTypeDef Motor_TIM_OCIniterStructure;
    TIM_OCStructInit(&Motor_TIM_OCIniterStructure);
    Motor_TIM_OCIniterStructure.TIM_OCMode = TIM_OCMode_PWM1;//���ģʽ
    Motor_TIM_OCIniterStructure.TIM_OCPolarity = TIM_OCPolarity_High;//�߼���
    Motor_TIM_OCIniterStructure.TIM_OutputState = TIM_OutputState_Enable;//ʹ������Ƚ�
    Motor_TIM_OCIniterStructure.TIM_Pulse = 0;//CCR    Ĭ��0ռ�ձ�
    TIM_OC1Init(PWMA_TIMx,&Motor_TIM_OCIniterStructure);//��ʼ��4��ͨ��������Ƚ�
    TIM_OC2Init(PWMB_TIMx,&Motor_TIM_OCIniterStructure);
    TIM_OC3Init(PWMC_TIMx,&Motor_TIM_OCIniterStructure);
    TIM_OC4Init(PWMD_TIMx,&Motor_TIM_OCIniterStructure);
    
    TIM_Cmd(PWMA_TIMx,ENABLE);//������ʱ��
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
            if(Value > 0)//��ת
            {
                GPIO_SetBits(AIN2_Port,AIN2);
                GPIO_ResetBits(AIN1_Port,AIN1);
                TIM_SetCompare1(PWMA_TIMx,Value);
            }
            else if(Value < 0 )//��ת
            {
                GPIO_SetBits(AIN1_Port,AIN1);
                GPIO_ResetBits(AIN2_Port,AIN2);
                Value = -Value;
                TIM_SetCompare1(PWMA_TIMx,Value);
            }
            else//0��ֹͣ
            {
                GPIO_ResetBits(AIN1_Port,AIN1);
                GPIO_ResetBits(AIN2_Port,AIN2);
            }
            
        break;
        
        case 2:
            if(Value > 0)//��ת
            {
                GPIO_SetBits(BIN1_Port,BIN1);
                GPIO_ResetBits(BIN2_Port,BIN2);
                TIM_SetCompare2(PWMB_TIMx,Value);
            }
            else if(Value < 0 )//��ת
            {
                GPIO_SetBits(BIN2_Port,BIN2);
                GPIO_ResetBits(BIN1_Port,BIN1);
                Value = -Value;
                TIM_SetCompare2(PWMB_TIMx,Value);
            }
            else//0��ֹͣ
            {
                GPIO_ResetBits(BIN1_Port,BIN1);
                GPIO_ResetBits(BIN2_Port,BIN2);
            }
            
        break;
        
        case 3:
            if(Value > 0)//��ת
            {
                GPIO_SetBits(CIN2_Port,CIN2);
                GPIO_ResetBits(CIN1_Port,CIN1);
                TIM_SetCompare3(PWMC_TIMx,Value);
            }
            else if(Value < 0 )//��ת
            {
                GPIO_SetBits(CIN1_Port,CIN1);
                GPIO_ResetBits(CIN2_Port,CIN2);
                Value = -Value;
                TIM_SetCompare3(PWMC_TIMx,Value);
            }
            else//0��ֹͣ
            {
                GPIO_ResetBits(CIN1_Port,CIN1);
                GPIO_ResetBits(CIN2_Port,CIN2);
            }
            
        break;
        
        case 4:
            if(Value > 0)//��ת
            {
                GPIO_SetBits(DIN1_Port,DIN1);
                GPIO_ResetBits(DIN2_Port,DIN2);
                TIM_SetCompare4(PWMD_TIMx,Value);
            }
            else if(Value < 0 )//��ת
            {
                GPIO_SetBits(DIN2_Port,DIN2);
                GPIO_ResetBits(DIN1_Port,DIN1);
                Value = -Value;
                TIM_SetCompare4(PWMD_TIMx,Value);
            }
            else//0��ֹͣ
            {
                GPIO_ResetBits(DIN1_Port,DIN1);
                GPIO_ResetBits(DIN2_Port,DIN2);
            }
            
        break;
    }
    
}

//ǰ��
void Motor_Run(uint16_t Speed)
{
    Motor_SetSpeed(1,Speed);
    Motor_SetSpeed(2,Speed);
    Motor_SetSpeed(3,Speed);
    Motor_SetSpeed(4,Speed);
}

//����
void Motor_Back(uint16_t Speed)
{
    Motor_SetSpeed(1,-Speed);
    Motor_SetSpeed(2,-Speed);
    Motor_SetSpeed(3,-Speed);
    Motor_SetSpeed(4,-Speed);    
}

//��ƽ��
void Motor_LeftMove(uint16_t Speed)
{
    Motor_SetSpeed(1,-Speed);
    Motor_SetSpeed(2,Speed);
    Motor_SetSpeed(3,Speed);
    Motor_SetSpeed(4,-Speed);    
}

//��ƽ��
void Motor_RightMove(uint16_t Speed)
{
    Motor_SetSpeed(1,Speed);
    Motor_SetSpeed(2,-Speed);
    Motor_SetSpeed(3,-Speed);
    Motor_SetSpeed(4,Speed);    
}

//������   2��3��ǰ�������಻��
void Motor_LeftUp(uint16_t Speed)
{
    Motor_SetSpeed(1,0);
    Motor_SetSpeed(2,Speed);
    Motor_SetSpeed(3,Speed);
    Motor_SetSpeed(4,0);   
}


//������    2��3�ֺ��ˣ����಻��
void Motor_LeftDown(uint16_t Speed)
{
    Motor_SetSpeed(1,-Speed);
    Motor_SetSpeed(2,0);
    Motor_SetSpeed(3,0);
    Motor_SetSpeed(4,-Speed);     
}

//������    1��4��ǰ�������಻��
void Motor_RightUp(uint16_t Speed)
{
    Motor_SetSpeed(1,Speed);
    Motor_SetSpeed(2,0);
    Motor_SetSpeed(3,0);
    Motor_SetSpeed(4,Speed);   
}


//������       1��4�ֺ��ˣ����಻��
void Motor_RightDown(uint16_t Speed)
{
    Motor_SetSpeed(1,0);
    Motor_SetSpeed(2,-Speed);
    Motor_SetSpeed(3,-Speed);
    Motor_SetSpeed(4,0);
}

//����ת��      2��4ǰ����1��3����
void Motor_LetfTurn(uint16_t Speed)
{
    Motor_SetSpeed(1,-Speed);
    Motor_SetSpeed(2,Speed);
    Motor_SetSpeed(3,-Speed);
    Motor_SetSpeed(4,Speed);   
}


//����ת��      1��3ǰ����2��4����
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
