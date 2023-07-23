
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"
#include "IR.h"
#include "steper.h"


uint32_t temp = 0;//用于接收数据
uint32_t temp2 = 0;//用于接收数据
int long_push = 0;
int tim2_start_f = 0;

int tim2_light_start_f = 0;
int tim2_light_time = 0;
int tim2_light_time_f = 70;

int tim2_light_left_start_f = 0;
int tim2_light_left_time = 0;
int tim2_light_left_time_f = 70;

int tim2_bee_start_f = 0;
int tim2_bee_time = 0;
int tim2_bee_time_f = 1;

int forward_start_f = 0;
int backward_start_f = 0;

void SystemClock_Config(void);
void stop_all(void);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//捕获回调函数
{
	if (TIM1 == htim->Instance)
	{
		IR_CaptureCallback();
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//捕获回调函数
{
	if (htim == (&htim2))
	{
		if(tim2_bee_start_f==1&&tim2_bee_time==0){
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_11);//蜂鸣器
		}
		if(tim2_light_start_f==1&&tim2_light_time==0){
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_11);//蜂鸣器
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_14);//右转向灯
		}
		if(tim2_light_left_start_f==1&&tim2_light_left_time==0){
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_11);//蜂鸣器
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_15);//左转向灯
		}
		tim2_bee_time++;
		tim2_light_time++;
		tim2_light_left_time++;
		if(tim2_bee_time==tim2_bee_time_f){tim2_bee_time=0;}
		if(tim2_light_time==tim2_light_time_f){tim2_light_time=0;}
		if(tim2_light_left_time==tim2_light_left_time_f){tim2_light_left_time=0;}
	}
}


int main(void)
{

  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  __HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
  IR_Init();

 //  MX_TIM3_Init();
//  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
//  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
//  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
//  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);

//  MX_I2C1_Init();
//  OLED_Init();
//  OLED_Clear();
//  //型号：风火轮1:64
//  OLED_ShowCHinese(0,0,16,16);
//  OLED_ShowCHinese(16,0,17,16);
//  OLED_ShowString(32,0,":",16);
//  OLED_ShowCHinese(40,0,22,16);
//  OLED_ShowCHinese(56,0,23,16);
//  OLED_ShowCHinese(72,0,24,16);
//  OLED_ShowString(88,0,"1:64",16);
//  //电量82%
//  OLED_ShowCHinese(0,2,18,16);
//  OLED_ShowCHinese(16,2,19,16);
//  OLED_ShowString(32,2,":82%",16);
// //温度31
//  OLED_ShowCHinese(0,4,7,16);
//  OLED_ShowCHinese(16,4,8,16);
//  OLED_ShowString(32,4,":31",16);
//  OLED_ShowCHinese(60,4,13,16);
//  //磁盘55%
//  OLED_ShowCHinese(0,6,11,16);
//  OLED_ShowCHinese(16,6,12,16);
//  OLED_ShowString(32,6,":55%",16);
//  //内存55%
//  OLED_ShowCHinese(66,6,20,16);
//  OLED_ShowCHinese(82,6,21,16);
//  OLED_ShowString(98,6,":9%",16);
//  OLED_ShowNum(40,2,temp,8,16);

  while (1)
  {
	  if (IR_Scanf(&temp))
	  	{
//区分单击和双击
		  HAL_Delay(200);
		  if (IR_Scanf(&temp2) && temp==temp2){
			  long_push=1;
		  }
		  else{
			  long_push=0;
		  }

		  	//左转向灯操作
		  	if (temp==num_7){
		  		if(long_push==1){
		  			if(tim2_start_f==0){HAL_TIM_Base_Start_IT(&htim2);
		  			tim2_start_f=1;
		  			}
		  			tim2_light_left_start_f=1;
		  		}else{
		  			if(tim2_start_f==0){
		  			HAL_TIM_Base_Start_IT(&htim2);
		  			}
		  			if(tim2_light_left_start_f==1){
		  				tim2_light_left_start_f=0;
		  				if(tim2_bee_start_f==0&&tim2_light_start_f==0&&tim2_light_left_start_f==0){
		  						  			HAL_TIM_Base_Stop_IT(&htim2);
		  						  			tim2_start_f=0;}
		  			}else{
		  			tim2_light_left_start_f=1;
		  			}
		  			HAL_Delay(2000);
		  			tim2_light_left_start_f=0;
		  			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 1);
		  			if(tim2_bee_start_f==0&&tim2_light_start_f==0&&tim2_light_left_start_f==0){
		  			HAL_TIM_Base_Stop_IT(&htim2);
		  			tim2_start_f=0;}

		  		}

			}

		  	//右转向灯操作
		  	if (temp==num_9){
		  		if(long_push==1){
		  			if(tim2_start_f==0){HAL_TIM_Base_Start_IT(&htim2);
		  			tim2_start_f=1;
		  			}
		  			tim2_light_start_f=1;
		  		}else{
		  			if(tim2_start_f==0){
		  			HAL_TIM_Base_Start_IT(&htim2);
		  			}
		  			if(tim2_light_start_f==1){
		  				tim2_light_start_f=0;
		  				if(tim2_bee_start_f==0&&tim2_light_start_f==0){
		  						  			HAL_TIM_Base_Stop_IT(&htim2);
		  						  			tim2_start_f=0;}
		  			}else{
		  			tim2_light_start_f=1;
		  			}
		  			HAL_Delay(2000);
		  			tim2_light_start_f=0;
		  			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);
		  			if(tim2_bee_start_f==0&&tim2_light_start_f==0){
		  			HAL_TIM_Base_Stop_IT(&htim2);
		  			tim2_start_f=0;}

		  		}
		  	}

		  	//喇叭操作
		  	if (temp==num_8){
		  		if(long_push==1){
		  			if(tim2_start_f==0){
		  			  HAL_TIM_Base_Start_IT(&htim2);
		  			tim2_start_f=1;
		  			}
		  			tim2_bee_start_f=1;
		  		}else{
		  			if(tim2_start_f==0){
		  			HAL_TIM_Base_Start_IT(&htim2);
		  			}
		  			if(tim2_bee_start_f==1){
		  				tim2_bee_start_f=0;
		  				if(tim2_bee_start_f==0&&tim2_light_start_f==0){
		  						  			HAL_TIM_Base_Stop_IT(&htim2);
		  						  			tim2_start_f=0;}
		  			}else{
		  			tim2_bee_start_f=1;
		  			}
		  			HAL_Delay(800);
		  			tim2_bee_start_f=0;
		  			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, 0);
		  			if(tim2_bee_start_f==0&&tim2_light_start_f==0){
		  			HAL_TIM_Base_Stop_IT(&htim2);
		  			tim2_start_f=0;}
		  		}

			}

		  	if (temp==num_5){
		  		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_13);//前灯
			}
		  	if (temp==num_0){
		  		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_12);//尾灯
		  	}

		  	if (temp==right){

		  		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 0);//左右电机
		  		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 1);
		  		HAL_Delay(100);
		  		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 0);//左右电机
		  		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 0);
			}
		  	if (temp==left){

		  		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 1);//左右电机
		  		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 0);
		  		HAL_Delay(100);
		  		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 0);//左右电机
		  		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 0);

			}
		  	if (temp==num_2){

				for( int a = 0; a < 10; a = a + 1 ){
					steper_one_step(1,30);
					steper_one_step(1,30);
					steper_stop();
					HAL_Delay(400);
					steper_one_step(0,30);
					steper_one_step(0,30);
					steper_stop();
					HAL_Delay(400);
					if (IR_Scanf(&temp)){steper_stop();break;}
				}
				steper_stop();
			}

		  	//前后电机操作
		  	if (temp==up){
		  		if(long_push==1){
				  	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 1);
				  	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);
				  	  forward_start_f=1;
		  		}else{
		  			if(forward_start_f==1){
		  		  	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
					  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);
					  forward_start_f=0;
					  continue;
		  			}else{
				  	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 1);
				  	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);
				  	  HAL_Delay(800);
				  	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
				  	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);
		  			}
		  		}

			}
		  	if (temp==down){
		  		if(long_push==1){
				  	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
				  	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 1);
				  	  backward_start_f=1;
		  		}else{
		  			if(backward_start_f==1){
		  		  	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
					  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);
					  backward_start_f=0;
					  continue;
		  			}else{
				  	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
				  	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 1);
				  	  HAL_Delay(800);
				  	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
				  	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);
		  			}
		  		}


			}
		  	if (temp==ok){stop_all();}


		  	//灵活位置
		  	//M1
			if (temp==num_1){
				if(long_push==1){
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 1);
				}else{
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 1);
				  	  HAL_Delay(300);
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0);
				}
			}
			if (temp==num_3){
				if(long_push==1){
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0);
				}else{
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0);
				  	  HAL_Delay(300);
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0);
				}
			}
			//M2
		  	if (temp==num_4){
				if(long_push==1){
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 0);
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
				}else{
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 0);
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
				  	  HAL_Delay(800);
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 0);
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
				}
			}
		  	if (temp==num_6){
				if(long_push==1){
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 1);
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
				}else{
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 1);
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
				  	  HAL_Delay(800);
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 0);
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
				}

			}
		  	//M3
		  	if (temp==star){
				if(long_push==1){
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 0);
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 1);
				}else{
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 0);
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 1);
				  	  HAL_Delay(800);
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 0);
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 0);
				}

			}
			if (temp==call){
				if(long_push==1){
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 1);
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 0);
				}else{
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 1);
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 0);
				  	  HAL_Delay(800);
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 0);
				  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 0);
				}

			}
			//D1
		  	if (temp==new1){
		  		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_2);
			}
		  	//D2
		  	if (temp==new3){
		  		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_3);
			}
		  	//D3
		  	if (temp==new7){
		  		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_4);
			}
		  	//D4
		  	if (temp==new9){
		  		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
			}
	  		}
    /* USER CODE BEGIN 3 */
  }

  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void stop_all(void){
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0);//步进电机 B+
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, 0);//步进电机 A+
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, 0);//步进电机 A-


	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 0);//步进电机 B-
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);//右转向灯
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 1);//左转向灯
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 1);//前灯
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 1);//尾灯
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, 0);//蜂鸣器
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);//前后电机
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);//前后电机
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 0);//左右电机
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 0);//左右电机

	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);//灵活端口
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);//灵活端口
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);//灵活端口
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 0);//灵活端口
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 0);//灵活端口
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, 0);//灵活端口


	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 0);//灵活端口
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);//灵活端口
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 0);//灵活端口
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 0);//灵活端口
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);//灵活端口
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);//灵活端口
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);//灵活端口
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0);//灵活端口
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 0);//灵活端口
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);//灵活端口
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 0);//灵活端口
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 0);//灵活端口
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 0);//灵活端口

		HAL_TIM_Base_Stop_IT(&htim2);
		tim2_start_f=0;

		tim2_light_start_f = 0;
		tim2_light_time = 0;

		tim2_light_left_start_f = 0;
		tim2_light_left_time = 0;

		tim2_bee_start_f = 0;
		tim2_bee_time = 0;

		forward_start_f = 0;
		backward_start_f = 0;

}
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
