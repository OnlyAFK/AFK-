#ifndef __SYS_H
#define __SYS_H

#include "Device/Include/stm32f10x.h"   // Device header
#include "delay.h"
#include "motor.h"
#include "encoder.h"
//#include "OLED.h"
#include "OLED_I2C.h"
#include <stdio.h>
#include "lcd.h"
#include "bluetooth.h"

/**********************************************************************************************/
/* GPIO����ר�ú궨�� */
#define SYS_GPIO_MODE_IN        0       /* ��ͨ����ģʽ */
#define SYS_GPIO_MODE_OUT       1       /* ��ͨ���ģʽ */
#define SYS_GPIO_MODE_AF        2       /* AF����ģʽ */
#define SYS_GPIO_MODE_AIN       3       /* ģ������ģʽ */

#define SYS_GPIO_SPEED_LOW      2       /* GPIO�ٶ�(����,2M) */
#define SYS_GPIO_SPEED_MID      1       /* GPIO�ٶ�(����,10M) */
#define SYS_GPIO_SPEED_HIGH     3       /* GPIO�ٶ�(����,50M) */

#define SYS_GPIO_PUPD_NONE      0       /* ���������� */
#define SYS_GPIO_PUPD_PU        1       /* ���� */
#define SYS_GPIO_PUPD_PD        2       /* ���� */

#define SYS_GPIO_OTYPE_PP       0       /* ������� */
#define SYS_GPIO_OTYPE_OD       1       /* ��©��� */

/* GPIO����λ�ú궨��  */
#define SYS_GPIO_PIN0           1<<0
#define SYS_GPIO_PIN1           1<<1
#define SYS_GPIO_PIN2           1<<2
#define SYS_GPIO_PIN3           1<<3
#define SYS_GPIO_PIN4           1<<4
#define SYS_GPIO_PIN5           1<<5
#define SYS_GPIO_PIN6           1<<6
#define SYS_GPIO_PIN7           1<<7
#define SYS_GPIO_PIN8           1<<8
#define SYS_GPIO_PIN9           1<<9
#define SYS_GPIO_PIN10          1<<10
#define SYS_GPIO_PIN11          1<<11
#define SYS_GPIO_PIN12          1<<12
#define SYS_GPIO_PIN13          1<<13
#define SYS_GPIO_PIN14          1<<14
#define SYS_GPIO_PIN15          1<<15

void sys_gpio_remap_set(uint8_t pos, uint8_t bit, uint8_t val);                 /* GPIO REMAP ���� */
void sys_gpio_set(GPIO_TypeDef *p_gpiox, uint16_t pinx, uint32_t mode, 
                  uint32_t otype, uint32_t ospeed, uint32_t pupd);              /*  GPIOͨ������ */
void sys_gpio_pin_set(GPIO_TypeDef *p_gpiox, uint16_t pinx, uint8_t status);    /* ����GPIOĳ�����ŵ����״̬ */
uint8_t sys_gpio_pin_get(GPIO_TypeDef *p_gpiox, uint16_t pinx);                 /* ��ȡGPIOĳ�����ŵ�״̬ */

#endif
