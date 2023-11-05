#ifndef __MOTOR_H
#define __MOTOR_H

    #include "sys.h"
    void Motor_Init(void);
    void Motor_SetSpeed(uint16_t Celect_Motor,int16_t Temp);    
    void Motor_Run(uint16_t Speed);
    void Motor_Back(uint16_t Speed);
    void Motor_LeftMove(uint16_t Speed);
    void Motor_RightMove(uint16_t Speed);
    void Motor_LeftUp(uint16_t Speed);
    void Motor_LeftDown(uint16_t Speed);
    void Motor_RightUp(uint16_t Speed);
    void Motor_RightDown(uint16_t Speed);
    void Motor_LetfTurn(uint16_t Speed);
    void Motor_RightTurn(uint16_t Speed);
    void test(void);

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
    
#endif
