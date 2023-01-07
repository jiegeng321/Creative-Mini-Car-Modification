#ifndef __steper_H
#define __steper_H
#include "stm32f1xx_hal.h"
#define A_a_1() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, 1)//A-
#define A_a_0() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, 0)//A-

#define A_b_1() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, 1)//A+
#define A_b_0() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, 0)//A+

#define B_a_1() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 1)//B-
#define B_a_0() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 0)//B-

#define B_b_1() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1)//B+
#define B_b_0() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0)//B+

//#define A_a_1() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 1)//A-
//#define A_a_0() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0)//A-
//
//#define A_b_1() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 1)//A+
//#define A_b_0() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0)//A+
//
//#define B_a_1() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 1)//B-
//#define B_a_0() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 0)//B-
//
//#define B_b_1() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 1)//B+
//#define B_b_0() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 0)//B+

void steper_one_step(uint8_t direction, uint8_t speed);
void steper_stop(void);
void steper_one_cycle(uint8_t direction, uint8_t speed);





#endif
