#include "sys.h"

/**
 * @brief       GPIOͨ������
 * @param       p_gpiox: GPIOA~GPIOG, GPIOָ��
 * @param       pinx: 0X0000~0XFFFF, ����λ��, ÿ��λ����һ��IO, ��0λ����Px0, ��1λ����Px1, ��������. ����0X0101, ����ͬʱ����Px0��Px8.
 *   @arg       SYS_GPIO_PIN0~SYS_GPIO_PIN15, 1<<0 ~ 1<<15
 *
 * @param       mode: 0~3; ģʽѡ��, ��������:
 *   @arg       SYS_GPIO_MODE_IN,  0, ����ģʽ(ϵͳ��λĬ��״̬)
 *   @arg       SYS_GPIO_MODE_OUT, 1, ���ģʽ
 *   @arg       SYS_GPIO_MODE_AF,  2, ���ù���ģʽ
 *   @arg       SYS_GPIO_MODE_AIN, 3, ģ������ģʽ
 *
 * @param       otype: 0 / 1; �������ѡ��, ��������:
 *   @arg       SYS_GPIO_OTYPE_PP, 0, �������
 *   @arg       SYS_GPIO_OTYPE_OD, 1, ��©���
 *
 * @param       ospeed: 0~2; ����ٶ�, ��������(ע��: ����Ϊ0!!):
 *   @arg       SYS_GPIO_SPEED_LOW,  2, ����
 *   @arg       SYS_GPIO_SPEED_MID,  1, ����
 *   @arg       SYS_GPIO_SPEED_HIGH, 3, ����
 *
 * @param       pupd: 0~3: ����������, ��������:
 *   @arg       SYS_GPIO_PUPD_NONE, 0, ����������
 *   @arg       SYS_GPIO_PUPD_PU,   1, ����
 *   @arg       SYS_GPIO_PUPD_PD,   2, ����
 *   @arg       SYS_GPIO_PUPD_RES,  3, ����
 *
 * @note:       ע��:
 *              1, ������ģʽ(��ͨ����/ģ������)��, otype �� ospeed ������Ч!!
 *              2, �����ģʽ��, pupd ������Ч!!(��©����޷�ʹ���ڲ���������!!)
 * @retval      ��
 */
void sys_gpio_set(GPIO_TypeDef *p_gpiox, uint16_t pinx, uint32_t mode, uint32_t otype, uint32_t ospeed, uint32_t pupd)
{
    uint32_t pinpos = 0, pos = 0, curpin = 0;
    uint32_t config = 0;        /* ���ڱ���ĳһ��IO������(CNF[1:0] + MODE[1:0]),ֻ���������4λ */

    for (pinpos = 0; pinpos < 16; pinpos++)
    {
        pos = 1 << pinpos;      /* һ����λ��� */
        curpin = pinx & pos;    /* ��������Ƿ�Ҫ���� */

        if (curpin == pos)      /* ��Ҫ���� */
        {
            config = 0;         /* bit0~3������Ϊ0, ��CNF[1:0] = 0; MODE[1:0] = 0;  Ĭ����ģ������ģʽ */

            if ((mode == 0X01) || (mode == 0X02))   /* �������ͨ���ģʽ/���ù���ģʽ */
            {
                config = ospeed & 0X03;             /* ����bit0/1 MODE[1:0] = 2/1/3 �ٶȲ��� */
                config |= (otype & 0X01) << 2;      /* ����bit2   CNF[0]    = 0/1   ����/��©��� */
                config |= (mode - 1) << 3;          /* ����bit3   CNF[1]    = 0/1   ��ͨ/������� */
            }
            else if (mode == 0)     /* �������ͨ����ģʽ */
            {
                if (pupd == 0)   /* ����������,����������ģʽ */
                {
                    config = 1 << 2;               /* ����bit2/3 CNF[1:0] = 01   ��������ģʽ */
                }
                else
                {
                    config = 1 << 3;                            /* ����bit2/3 CNF[1:0] = 10   ����������ģʽ */
                    p_gpiox->ODR &= ~(1 << pinpos);             /* ���ԭ�������� */
                    p_gpiox->ODR |= (pupd & 0X01) << pinpos;    /* ����ODR = 0/1 ����/���� */
                }
            }

            /* ����IO��λ�� ����CRL / CRH�Ĵ��� */
            if (pinpos <= 7)
            {
                p_gpiox->CRL &= ~(0X0F << (pinpos * 4));        /* ���ԭ�������� */
                p_gpiox->CRL |= config << (pinpos * 4);         /* ����CNFx[1:0] �� MODEx[1:0], x = pinpos = 0~7 */
            }
            else
            {
                p_gpiox->CRH &= ~(0X0F << ((pinpos - 8) * 4));  /* ���ԭ�������� */
                p_gpiox->CRH |= config << ((pinpos - 8) * 4);   /* ����CNFx[1:0] �� MODEx[1:0], x = pinpos = 8~15 */

            }
        }
    }
}

/**
 * @brief       ����GPIOĳ�����ŵ����״̬
 * @param       p_gpiox: GPIOA~GPIOG, GPIOָ��
 * @param       0X0000~0XFFFF, ����λ��, ÿ��λ����һ��IO, ��0λ����Px0, ��1λ����Px1, ��������. ����0X0101, ����ͬʱ����Px0��Px8.
 *   @arg       SYS_GPIO_PIN0~SYS_GPIO_PIN15, 1<<0 ~ 1<<15
 * @param       status: 0/1, ����״̬(�����λ��Ч), ��������:
 *   @arg       0, ����͵�ƽ
 *   @arg       1, ����ߵ�ƽ
 * @retval      ��
 */
void sys_gpio_pin_set(GPIO_TypeDef *p_gpiox, uint16_t pinx, uint8_t status)
{
    if (status & 0X01)
    {
        p_gpiox->BSRR |= pinx;  /* ����GPIOx��pinxΪ1 */
    }
    else
    {
        p_gpiox->BSRR |= (uint32_t)pinx << 16;  /* ����GPIOx��pinxΪ0 */
    }
}

/**
 * @brief       ��ȡGPIOĳ�����ŵ�״̬
 * @param       p_gpiox: GPIOA~GPIOG, GPIOָ��
 * @param       0X0000~0XFFFF, ����λ��, ÿ��λ����һ��IO, ��0λ����Px0, ��1λ����Px1, ��������. ����0X0101, ����ͬʱ����Px0��Px8.
 *   @arg       SYS_GPIO_PIN0~SYS_GPIO_PIN15, 1<<0 ~ 1<<15
 * @retval      ��������״̬, 0, �͵�ƽ; 1, �ߵ�ƽ
 */
uint8_t sys_gpio_pin_get(GPIO_TypeDef *p_gpiox, uint16_t pinx)
{
    if (p_gpiox->IDR & pinx)
    {
        return 1;   /* pinx��״̬Ϊ1 */
    }
    else
    {
        return 0;   /* pinx��״̬Ϊ0 */
    }
}
