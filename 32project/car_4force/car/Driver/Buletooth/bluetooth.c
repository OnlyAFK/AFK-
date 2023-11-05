#include "bluetooth.h"

    //配置蓝牙要用的串口
void Bluetooth_USART_Init(void)
{
    //开启RCC
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    
    //配置GPIO
    GPIO_InitTypeDef Bluetooth_GPIO_InitStructure;
    Bluetooth_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    Bluetooth_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//单片机的PA9是TX脚，配为复用推挽输出
    Bluetooth_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&Bluetooth_GPIO_InitStructure);
    Bluetooth_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    Bluetooth_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//单片机的PA10是RX脚，配为浮空输入
    Bluetooth_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&Bluetooth_GPIO_InitStructure);
    
    //串口初始化
    USART_InitTypeDef Bluetooth_USART_InitStructure;
    Bluetooth_USART_InitStructure.USART_BaudRate = 9600;//波特率
    Bluetooth_USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//关闭硬件流控制
    Bluetooth_USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//开启串口输出和接收模式
    Bluetooth_USART_InitStructure.USART_Parity = USART_Parity_No;//无校验位
    Bluetooth_USART_InitStructure.USART_StopBits = USART_StopBits_1;//1位停止位
    Bluetooth_USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位有效数据
    USART_Init(USART1,&Bluetooth_USART_InitStructure);
    
    //配置接收中断
    
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//开启串口接收中断
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//配置NVIC通道
    NVIC_InitTypeDef Bluetooth_NVIC_InitStructure;
    Bluetooth_NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    Bluetooth_NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    Bluetooth_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    Bluetooth_NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&Bluetooth_NVIC_InitStructure);
    
    //使能串口
    USART_Cmd(USART1,ENABLE);
}

void Bluetooth_Init(void)
{
    Bluetooth_USART_Init();
}

/******************发送相关******************/

//发送字节
void Bluetooth_USART_SendByte(uint8_t Data)
{
    USART_SendData(USART1,Data);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);//等待发送完成
}

//发送数组
void Bluetooth_USART_SendArray(uint16_t *Array,uint16_t Length)
{
    uint16_t i;
    for(i = 0;i < Length;i++)
    {
        Bluetooth_USART_SendByte(Array[i]);
    }
}

//发送字符串
void Bluetooth_USART_SendString(char *String)
{
    uint16_t i;
    for(i = 0;String[i] != '\0';i++)
    {
        Bluetooth_USART_SendByte(String[i]);
    }
}

//发送数字
uint32_t Bluetooth_Pow(uint32_t X,uint32_t Y)//辅助发送数字
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

////printf重定向
//int fputc(int ch,FILE *f)
//{
//    Bluetooth_USART_SendByte(ch);
//    return ch;
//}



/********************************************/

/******************接收相关******************/
uint8_t Bluetooth_RxData;
uint8_t Bluetooth_RxFlag;

//获取接收标志位
uint8_t Bluetooth_GetRxFlag(void)
{
    if(Bluetooth_RxFlag == 1)
    {
        Bluetooth_RxFlag = 0;
        return 1;
    }
    return 0;
}

//获取数据
uint8_t Bluetooth_GetRxData(void)
{
    return Bluetooth_RxData;
}

//串口中断回调函数
void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
    {
        Bluetooth_RxData = USART_ReceiveData(USART1);
        Bluetooth_RxFlag = 1;
        USART_ClearITPendingBit(USART1,USART_FLAG_RXNE);//手动清除中断标志位
        
    }
}

/********************************************/

uint16_t Value;
uint16_t Speed = 30;
//遥控函数
void Bluooth_Detecting(void)
{
    if(Bluetooth_GetRxFlag() == 1)//接收到数据
    {
        Value = Bluetooth_GetRxData();
    }
    
    if(Speed > 100)
    {
        Speed = 100;
    }
    
    switch(Value)
    {
        case '0'://停止
            Motor_Run(0);
        break;
        
        case '1'://前进
            Motor_Run(Speed);
        break;
        
        case '2'://后退
            Motor_Back(Speed);
        break;
        
        case '3'://左平移
            Motor_LeftMove(Speed);
        break;
        
        case '4'://右平移
            Motor_RightMove(Speed);
        break;
        
        case '5'://左上移
            Motor_LeftUp(Speed);
        break;
        
        case '6'://左下移
            Motor_LeftDown(Speed);
        break;
        
        case '7'://右上移
            Motor_RightUp(Speed);
        break;
        
        case '8'://右下移
            Motor_RightDown(Speed);
        break;
        
        case '9'://左旋
            Motor_LetfTurn(Speed);
        break;
        
        case 'A'://右旋
            Motor_RightTurn(Speed);
        break;
        
        case 'B'://调挡  +10
            Speed += 10;
        break;
        
        case 'C'://调挡  -10
            Speed -= 10;
        break;
    }
}

