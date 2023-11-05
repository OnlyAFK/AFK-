#include "bluetooth.h"

    //��������Ҫ�õĴ���
void Bluetooth_USART_Init(void)
{
    //����RCC
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    
    //����GPIO
    GPIO_InitTypeDef Bluetooth_GPIO_InitStructure;
    Bluetooth_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    Bluetooth_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//��Ƭ����PA9��TX�ţ���Ϊ�����������
    Bluetooth_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&Bluetooth_GPIO_InitStructure);
    Bluetooth_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    Bluetooth_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��Ƭ����PA10��RX�ţ���Ϊ��������
    Bluetooth_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&Bluetooth_GPIO_InitStructure);
    
    //���ڳ�ʼ��
    USART_InitTypeDef Bluetooth_USART_InitStructure;
    Bluetooth_USART_InitStructure.USART_BaudRate = 9600;//������
    Bluetooth_USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//�ر�Ӳ��������
    Bluetooth_USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//������������ͽ���ģʽ
    Bluetooth_USART_InitStructure.USART_Parity = USART_Parity_No;//��У��λ
    Bluetooth_USART_InitStructure.USART_StopBits = USART_StopBits_1;//1λֹͣλ
    Bluetooth_USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ��Ч����
    USART_Init(USART1,&Bluetooth_USART_InitStructure);
    
    //���ý����ж�
    
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//�������ڽ����ж�
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����NVICͨ��
    NVIC_InitTypeDef Bluetooth_NVIC_InitStructure;
    Bluetooth_NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    Bluetooth_NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    Bluetooth_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    Bluetooth_NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&Bluetooth_NVIC_InitStructure);
    
    //ʹ�ܴ���
    USART_Cmd(USART1,ENABLE);
}

void Bluetooth_Init(void)
{
    Bluetooth_USART_Init();
}

/******************�������******************/

//�����ֽ�
void Bluetooth_USART_SendByte(uint8_t Data)
{
    USART_SendData(USART1,Data);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);//�ȴ��������
}

//��������
void Bluetooth_USART_SendArray(uint16_t *Array,uint16_t Length)
{
    uint16_t i;
    for(i = 0;i < Length;i++)
    {
        Bluetooth_USART_SendByte(Array[i]);
    }
}

//�����ַ���
void Bluetooth_USART_SendString(char *String)
{
    uint16_t i;
    for(i = 0;String[i] != '\0';i++)
    {
        Bluetooth_USART_SendByte(String[i]);
    }
}

//��������
uint32_t Bluetooth_Pow(uint32_t X,uint32_t Y)//������������
{
    uint32_t Resuit = 1;
    while(Y--)
    {
        Resuit *= X;
    }
    return Resuit;
}

void Bluetooth_SendNumber(int32_t Number,uint8_t Length)
{
    uint16_t i;
    for(i = 0;i < Length;i++)
    {
        Bluetooth_USART_SendByte(Number / Bluetooth_Pow(10,Length - i - 1) % 10 + '0');
    }
}

////printf�ض���
//int fputc(int ch,FILE *f)
//{
//    Bluetooth_USART_SendByte(ch);
//    return ch;
//}



/********************************************/

/******************�������******************/
uint8_t Bluetooth_RxData;
uint8_t Bluetooth_RxFlag;

//��ȡ���ձ�־λ
uint8_t Bluetooth_GetRxFlag(void)
{
    if(Bluetooth_RxFlag == 1)
    {
        Bluetooth_RxFlag = 0;
        return 1;
    }
    return 0;
}

//��ȡ����
uint8_t Bluetooth_GetRxData(void)
{
    return Bluetooth_RxData;
}

//�����жϻص�����
void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
    {
        Bluetooth_RxData = USART_ReceiveData(USART1);
        Bluetooth_RxFlag = 1;
        USART_ClearITPendingBit(USART1,USART_FLAG_RXNE);//�ֶ�����жϱ�־λ
        
    }
}

/********************************************/

uint16_t Value;
uint16_t Speed = 30;
//ң�غ���
void Bluooth_Detecting(void)
{
    if(Bluetooth_GetRxFlag() == 1)//���յ�����
    {
        Value = Bluetooth_GetRxData();
    }
    
    if(Speed > 100)
    {
        Speed = 100;
    }
    
    switch(Value)
    {
        case '0'://ֹͣ
            Motor_Run(0);
        break;
        
        case '1'://ǰ��
            Motor_Run(Speed);
        break;
        
        case '2'://����
            Motor_Back(Speed);
        break;
        
        case '3'://��ƽ��
            Motor_LeftMove(Speed);
        break;
        
        case '4'://��ƽ��
            Motor_RightMove(Speed);
        break;
        
        case '5'://������
            Motor_LeftUp(Speed);
        break;
        
        case '6'://������
            Motor_LeftDown(Speed);
        break;
        
        case '7'://������
            Motor_RightUp(Speed);
        break;
        
        case '8'://������
            Motor_RightDown(Speed);
        break;
        
        case '9'://����
            Motor_LetfTurn(Speed);
        break;
        
        case 'A'://����
            Motor_RightTurn(Speed);
        break;
        
        case 'B'://����  +10
            Speed += 10;
        break;
        
        case 'C'://����  -10
            Speed -= 10;
        break;
    }
}

