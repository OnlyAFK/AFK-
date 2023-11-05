#include "stm32f10x.h"                  // Device header
#include "sys.h"


int main(void)
{

//    uint8_t x = 0;
//    uint8_t lcd_id[12];
    
    DelayInit();
    Bluetooth_Init();
//    Encoder_Init();
//    I2C_Configuration();
//    OLED_Init();
//    OLED_CLS();
    Motor_Init();//放在最后初始化，防止JTAG失能失效
    
    lcd_init();                 /* 初始化LCD */
//    g_point_color = RED;
//    sprintf((char *)lcd_id, "LCD ID:%04X", lcddev.id);  /* 将LCD ID打印到lcd_id数组 */

//    uint16_t encoder_temp;
//    char temp[20];

//test();
//    uint8_t Data;
	while (1)
	{
//        Bluooth_Detecting();
        
        
        
//        Bluetooth_USART_SendByte(0x41);
//        if(Bluetooth_GetRxFlag() == 1)
//        {
//           Data = Bluetooth_GetRxData();
//           Bluetooth_USART_SendByte(Data);
//        }
//        
//        if(Data == '1')
//        {
//            Motor_Run(100);
//        }
//        else
//        {
//            Motor_Run(0);
//        }
        
        
//        switch (x)
//        {
//            case 0:
//                lcd_clear(WHITE);
//                break;

//            case 1:
//                lcd_clear(BLACK);
//                break;

//            case 2:
//                lcd_clear(BLUE);
//                break;

//            case 3:
//                lcd_clear(RED);
//                break;

//            case 4:
//                lcd_clear(MAGENTA);
//                break;

//            case 5:
//                lcd_clear(GREEN);
//                break;

//            case 6:
//                lcd_clear(CYAN);
//                break;

//            case 7:
//                lcd_clear(YELLOW);
//                break;

//            case 8:
//                lcd_clear(BRRED);
//                break;

//            case 9:
//                lcd_clear(GRAY);
//                break;

//            case 10:
//                lcd_clear(LGRAY);
//                break;

//            case 11:
//                lcd_clear(BROWN);
//                break;
//        }

//        lcd_show_string(10, 40, 240, 32, 32, "STM32", RED);
//        lcd_show_string(10, 80, 240, 24, 24, "TFTLCD TEST", RED);
//        lcd_show_string(10, 110, 240, 16, 16, "ATOM@ALIENTEK", RED);
//        lcd_show_string(10, 130, 240, 16, 16, (char*)lcd_id, RED);  /* 显示LCD ID */
//        x++;

//        if (x == 12)x = 0;

//        DelayMs(1000);


//        encoder_temp = Encoder_GetConter(1);
//        snprintf(temp, sizeof(temp), "Encoder1: %d", encoder_temp);
//        OLED_ShowStr(1,1,(unsigned char*)temp,1);
//        
//         encoder_temp = Encoder_GetConter(2);
//        snprintf(temp, sizeof(temp), "Encoder2: %d", encoder_temp);
//        OLED_ShowStr(1,2,(unsigned char*)temp,1);
//        
//         encoder_temp = Encoder_GetConter(3);
//        snprintf(temp, sizeof(temp), "Encoder3: %d", encoder_temp);
//        OLED_ShowStr(1,3,(unsigned char*)temp,1);
//        
//         encoder_temp = Encoder_GetConter(4);
//        snprintf(temp, sizeof(temp), "Encoder4: %d", encoder_temp);
//        OLED_ShowStr(1,4,(unsigned char*)temp,1);
//        
//        DelayMs(100);
        
//        Motor_Run(100);
//            GPIO_ResetBits(GPIOA,GPIO_Pin_13);
	}
}
