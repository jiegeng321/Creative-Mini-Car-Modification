/*
 * IR.h
 *
 *  Created on: Dec 9, 2020
 *      Author: Administrator
 */

#ifndef INC_IR_H_
#define INC_IR_H_
//csdn_可谓不低
#ifndef __IR_H
#define __IR_H

#include "main.h"

#define num_0 16750695
#define num_1 16753245
#define num_2 16736925
#define num_3 16769565
#define num_4 16720605
#define num_5 16712445
#define num_6 16761405
#define num_7 16769055
#define num_8 16754775
#define num_9 16748655

#define star 16738455
#define call 16756815

#define up 16718055
#define left 16716015
#define right 16734885
#define down 16730805
#define ok 16726215

#define new1 16724175
#define new3 16743045
#define new7 16728765
#define new9 16732845


#define IR_TIM htim1
#define IR_TIM_CHANNE TIM_CHANNEL_1

#define IR_us_LOGIC_0 560
#define IR_us_LOGIC_1 1680
#define IR_us_START 4480
#define IR_us_REPEAT_START 39350
#define IR_us_REPEAT_END 2220
#define IR_DEVIATION 100	//误差范围


#define IR_CHECK_ADDRESS 1  //1：检查地址位 0：取消
#define IR_CHECK_COMMAND 1  //1：检查数据位 0：取消
#define IR_CHECK_REPEAT 1   //1：检查重复   0：取消

void IR_Init(void);
void IR_CaptureCallback(void);
uint8_t IR_Scanf(uint32_t* data);

#endif
/***************************************
 * 1        00FFA25D
 * 2        00FF629D
 * 3        00FFE21D
 * 4        00FF22DD
 * 5        00FF02FD
 * 6        00FFC23D
 * 7        00FFE01F
 * 8        00FFA857
 * 9        00FF906F
 * *        00FF6897
 * 0        00FF9867
 * #        00FFB04F
 * up       00FF18E7
 * left     00FF10EF
 * OK       00FF38C7
 * right    00FF5AA5
 * down     00FF4AB5
*****************************************/
/****************************************
 * 放入捕获中断回调函数中使用，按需修改TIM1
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (TIM1 == htim->Instance)
	{
		IR_CaptureCallback();
	}
}
******************************************/




#endif /* INC_IR_H_ */
