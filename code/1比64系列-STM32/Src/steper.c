
#include "steper.h"
#include "stm32f1xx_hal.h"



void steper_one_step(uint8_t direction, uint8_t speed){//72度

	if(direction==0){
		A_a_1();A_b_0();B_a_0();B_b_0();
		HAL_Delay(speed);
		A_a_0();A_b_0();B_a_1();B_b_0();
		HAL_Delay(speed);
		A_a_0();A_b_1();B_a_0();B_b_0();
		HAL_Delay(speed);
		A_a_0();A_b_0();B_a_0();B_b_1();
		HAL_Delay(speed);
	}else{
		A_a_0();A_b_0();B_a_0();B_b_1();
		HAL_Delay(speed);
		A_a_0();A_b_1();B_a_0();B_b_0();
		HAL_Delay(speed);
		A_a_0();A_b_0();B_a_1();B_b_0();
		HAL_Delay(speed);
		A_a_1();A_b_0();B_a_0();B_b_0();
		HAL_Delay(speed);
	}
}


void steper_stop(void){
		A_a_0();A_b_0();B_a_0();B_b_0();
}


void steper_one_cycle(uint8_t direction, uint8_t speed){//360度
	for(int i=0;i<5;i++){
		steper_one_step(direction,speed);
	}
}
