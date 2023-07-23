#include "stm8s.h"
#include "remote.h"

#define LED_GPIO_PORT  (GPIOD)
#define LED_GPIO_PIN  (GPIO_PIN_2)//指示灯/室内灯

#define BUZZER_GPIO_PORT  (GPIOD)
#define BUZZER_GPIO_PIN  (GPIO_PIN_4) //蜂鸣器

#define MOTOR_FRONT_GPIO_PORT  (GPIOC)
#define MOTOR_FRONT_GPIO_PIN  (GPIO_PIN_6)//前后直流电机
#define MOTOR_BACK_GPIO_PORT  (GPIOC)
#define MOTOR_BACK_GPIO_PIN  (GPIO_PIN_7) 

#define MOTOR_A1_GPIO_PORT  (GPIOC)
#define MOTOR_A1_GPIO_PIN  (GPIO_PIN_4)//左右步进电机 A-
#define MOTOR_A2_GPIO_PORT  (GPIOC)
#define MOTOR_A2_GPIO_PIN  (GPIO_PIN_5)//左右步进电机 A+
#define MOTOR_B1_GPIO_PORT  (GPIOC)
#define MOTOR_B1_GPIO_PIN  (GPIO_PIN_3)//左右步进电机 B-
#define MOTOR_B2_GPIO_PORT  (GPIOA)
#define MOTOR_B2_GPIO_PIN  (GPIO_PIN_3) //左右步进电机 B+

#define A_a_1() GPIO_WriteHigh(MOTOR_A1_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_A1_GPIO_PIN);//A-
#define A_a_0() GPIO_WriteLow(MOTOR_A1_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_A1_GPIO_PIN);//A-
#define A_b_1() GPIO_WriteHigh(MOTOR_A2_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_A2_GPIO_PIN);//A+
#define A_b_0() GPIO_WriteLow(MOTOR_A2_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_A2_GPIO_PIN);//A+
#define B_a_1() GPIO_WriteHigh(MOTOR_B1_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_B1_GPIO_PIN);//B-
#define B_a_0() GPIO_WriteLow(MOTOR_B1_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_B1_GPIO_PIN);//B-
#define B_b_1() GPIO_WriteHigh(MOTOR_B2_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_B2_GPIO_PIN);//B+
#define B_b_0() GPIO_WriteLow(MOTOR_B2_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_B2_GPIO_PIN);//B+

#define LIGHT_FRONT_GPIO_PORT  (GPIOD)
#define LIGHT_FRONT_GPIO_PIN  (GPIO_PIN_5)//前灯
#define LIGHT_BACK_GPIO_PORT  (GPIOD)
#define LIGHT_BACK_GPIO_PIN  (GPIO_PIN_6)//尾灯
#define LIGHT_LEFT_GPIO_PORT  (GPIOA)
#define LIGHT_LEFT_GPIO_PIN  (GPIO_PIN_1)//左转灯
#define LIGHT_RIGHT_GPIO_PORT  (GPIOA)
#define LIGHT_RIGHT_GPIO_PIN  (GPIO_PIN_2)//右转灯


void delay_init(u8 clk);
void delay_us(u16 nus);
void delay_ms(u32 nms);
void TIM1_Init(void);
void stop_all(void);

void steper_one_step(uint8_t direction, uint8_t speed);
void steper_stop(void);
void steper_one_cycle(uint8_t direction, uint8_t speed);

int tim1_light_right_start_f = 0;
int tim1_light_left_start_f = 0;
int tim1_emergency_start_f = 0;

int steper_cycles_per_press = 2;

int motor_front_back_time_ms = 1000;

int motor_front_back_speed = 2;

int motor_left_right_speed = 2;

int main( void )
{
  

    Clk_Init();//主时钟初始化
    Remote_Init();//IR解码涉及相关的初始化
    TIM1_Init();
    delay_init(16);
        
    GPIO_Init(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(BUZZER_GPIO_PORT, (GPIO_Pin_TypeDef)BUZZER_GPIO_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);

    GPIO_Init(LIGHT_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_FRONT_GPIO_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(LIGHT_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_BACK_GPIO_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(LIGHT_LEFT_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_LEFT_GPIO_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(LIGHT_RIGHT_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_RIGHT_GPIO_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    
    GPIO_Init(MOTOR_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_FRONT_GPIO_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(MOTOR_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_BACK_GPIO_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    
    GPIO_Init(MOTOR_A1_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_A1_GPIO_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(MOTOR_A2_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_A2_GPIO_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(MOTOR_B1_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_B1_GPIO_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(MOTOR_B2_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_B2_GPIO_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
   
    for(int i=0;i<4;i++){
      delay_ms(300);
      GPIO_WriteReverse(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PIN);
    }
    
    u8 key;//红外接收键值
    while(1){

            key=Remote_Scan();
             if(key==up&&motor_front_back_speed==2){//进退电机 前进 全速
                GPIO_WriteLow(MOTOR_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_FRONT_GPIO_PIN);
                for(int i=0;i<RmtCnt;i++){

                  GPIO_WriteHigh(MOTOR_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_BACK_GPIO_PIN);
                  delay_ms(115);
                }
                GPIO_WriteLow(MOTOR_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_FRONT_GPIO_PIN);
                GPIO_WriteLow(MOTOR_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_BACK_GPIO_PIN);
              }
             if(key==up&&motor_front_back_speed==1){//进退电机 前进 半速
                GPIO_WriteLow(MOTOR_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_FRONT_GPIO_PIN);
                for(int i=0;i<RmtCnt;i++){
                  for(int j=0;j<10;j++){
                  GPIO_WriteHigh(MOTOR_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_BACK_GPIO_PIN);
                  delay_ms(6);
                  GPIO_WriteLow(MOTOR_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_BACK_GPIO_PIN);
                  delay_ms(6);
                  }
                }
                GPIO_WriteLow(MOTOR_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_FRONT_GPIO_PIN);
                GPIO_WriteLow(MOTOR_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_BACK_GPIO_PIN);
              }
             if(key==down&&motor_front_back_speed==2){//进退电机 后退 全速
               GPIO_WriteLow(MOTOR_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_BACK_GPIO_PIN);
               for(int i=0;i<RmtCnt;i++){
                GPIO_WriteHigh(MOTOR_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_FRONT_GPIO_PIN);
                delay_ms(115);
               }
               GPIO_WriteLow(MOTOR_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_FRONT_GPIO_PIN);
               GPIO_WriteLow(MOTOR_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_BACK_GPIO_PIN);
              }
              if(key==down&&motor_front_back_speed==1){//进退电机 后退 半速
                GPIO_WriteLow(MOTOR_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_BACK_GPIO_PIN);
                for(int i=0;i<RmtCnt;i++){
                  for(int j=0;j<10;j++){
                  GPIO_WriteHigh(MOTOR_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_FRONT_GPIO_PIN);
                  delay_ms(6);
                  GPIO_WriteLow(MOTOR_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_FRONT_GPIO_PIN);
                  delay_ms(6);
                  }
                }
                GPIO_WriteLow(MOTOR_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_FRONT_GPIO_PIN);
                GPIO_WriteLow(MOTOR_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_BACK_GPIO_PIN);
              }
             
              if(key==left&&motor_left_right_speed==2){//步进电机 左转
                for(int i=0;i<RmtCnt;i++){
                  steper_one_step(1,5);
                }
                steper_stop();
              }
             
              if(key==left&&motor_left_right_speed==1){//步进电机 左转
                for(int i=0;i<RmtCnt;i++){
                  steper_one_step(1,10);
                }
                steper_stop();
              }
              if(key==right&&motor_left_right_speed==2){//步进电机 左转
                for(int i=0;i<RmtCnt;i++){
                  steper_one_step(0,5);
                }
                steper_stop();
              }
             
              if(key==right&&motor_left_right_speed==1){//步进电机 左转
                for(int i=0;i<RmtCnt;i++){
                  steper_one_step(0,10);
                }
                steper_stop();
              }
             
            if(key&&RmtCnt==1){

              if(key==num_8){//蜂鸣器
                for(int i=0;i<150;i+=1){
		  GPIO_WriteReverse(BUZZER_GPIO_PORT, (GPIO_Pin_TypeDef)BUZZER_GPIO_PIN);
		  delay_ms(1);
		  }
                GPIO_WriteLow(BUZZER_GPIO_PORT, (GPIO_Pin_TypeDef)BUZZER_GPIO_PIN);
              }
              
              //if(key==up){//进退电机 前进
              //  for(int i=0;i<RmtCnt;i++){
              //    GPIO_WriteLow(MOTOR_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_FRONT_GPIO_PIN);
              //    GPIO_WriteHigh(MOTOR_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_BACK_GPIO_PIN);
              //    delay_ms(100);
              //  }
              //  GPIO_WriteLow(MOTOR_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_FRONT_GPIO_PIN);
              //  GPIO_WriteLow(MOTOR_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_BACK_GPIO_PIN);
                //GPIO_WriteLow(MOTOR_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_FRONT_GPIO_PIN);
                //GPIO_WriteHigh(MOTOR_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_BACK_GPIO_PIN);
                //delay_ms(motor_front_back_time_ms);
                //GPIO_WriteLow(MOTOR_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_FRONT_GPIO_PIN);
                //GPIO_WriteLow(MOTOR_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_BACK_GPIO_PIN);
              //}
              //if(key==new1){//进退电机 前进
                //GPIO_WriteLow(MOTOR_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_FRONT_GPIO_PIN);
                
                //GPIO_WriteHigh(MOTOR_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_BACK_GPIO_PIN);
                
                //delay_ms(1000);
                //GPIO_WriteLow(MOTOR_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_FRONT_GPIO_PIN);
               // GPIO_WriteLow(MOTOR_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_BACK_GPIO_PIN);
             // }
              
              //if(key==down){//进退电机 后退
              //  GPIO_WriteHigh(MOTOR_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_FRONT_GPIO_PIN);
              //  GPIO_WriteLow(MOTOR_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_BACK_GPIO_PIN);
              //  delay_ms(motor_front_back_time_ms);
              //  GPIO_WriteLow(MOTOR_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_FRONT_GPIO_PIN);
              //  GPIO_WriteLow(MOTOR_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_BACK_GPIO_PIN);
              //}
              
              //if(key==left){//步进电机 左转
              //  for(int i=0;i<steper_cycles_per_press;i++){
              //    steper_one_cycle(1,25);
              //  }
              //  steper_stop();
              //}
              
              //if(key==right){//步进电机 右转
              //  for(int i=0;i<steper_cycles_per_press;i++){
              //    steper_one_cycle(0,25);
              //  }
              //  steper_stop();
              //}
              
              if(key==num_5){//前灯
               GPIO_WriteReverse(LIGHT_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_FRONT_GPIO_PIN);
              }
              if(key==num_0){//尾灯
               GPIO_WriteReverse(LIGHT_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_BACK_GPIO_PIN);
              }

            
              if(key==num_7){//右转向灯
                if(tim1_emergency_start_f==1){
                  tim1_emergency_start_f=0;
                  tim1_light_left_start_f=0;
                  tim1_light_right_start_f=0;
                }
                if(tim1_light_right_start_f==0){

                  tim1_light_left_start_f=0;
                  GPIO_WriteHigh(LIGHT_LEFT_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_LEFT_GPIO_PIN);
                  tim1_light_right_start_f=1;
                  if(tim1_light_left_start_f==0){
                    TIM1_Cmd(ENABLE);//开启定时器
                  }
                  GPIO_WriteHigh(LIGHT_RIGHT_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_RIGHT_GPIO_PIN);
                }else{
                  tim1_light_right_start_f=0;
                  if(tim1_light_right_start_f==0 && tim1_light_left_start_f==0){
                    TIM1_Cmd(DISABLE);//关闭定时器
                  }
                  GPIO_WriteHigh(LIGHT_RIGHT_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_RIGHT_GPIO_PIN);
                }
              }

              if(key==num_9){//左转向灯
                if(tim1_emergency_start_f==1){
                  tim1_emergency_start_f=0;
                  tim1_light_left_start_f=0;
                  tim1_light_right_start_f=0;
                }
                if(tim1_light_left_start_f==0){
                  tim1_light_right_start_f=0;
                  GPIO_WriteHigh(LIGHT_RIGHT_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_RIGHT_GPIO_PIN);
                  tim1_light_left_start_f=1;
                  if(tim1_light_right_start_f==0){
                    TIM1_Cmd(ENABLE);//开启定时器
                  }
                  GPIO_WriteHigh(LIGHT_LEFT_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_LEFT_GPIO_PIN);
                }else{
                  tim1_light_left_start_f=0;
                  if(tim1_light_left_start_f==0 && tim1_light_right_start_f==0){
                    TIM1_Cmd(DISABLE);//关闭定时器
                  }
                  GPIO_WriteHigh(LIGHT_LEFT_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_LEFT_GPIO_PIN);
                }
              }
              if (key==ok){//停止所有设备
                stop_all();
              }
              if(key==star){//指示灯/室内灯
               GPIO_WriteReverse(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PIN);
              }
              if (key==call){//紧急停车
                if(tim1_emergency_start_f==0){
                  tim1_emergency_start_f=1;
                  tim1_light_left_start_f=1;
                  tim1_light_right_start_f=1;
                  TIM1_Cmd(DISABLE);//关闭定时器
                  GPIO_WriteHigh(LIGHT_LEFT_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_LEFT_GPIO_PIN);
                  GPIO_WriteHigh(LIGHT_RIGHT_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_RIGHT_GPIO_PIN);
                  TIM1_Cmd(ENABLE);//开启定时器
                }else{
                  tim1_emergency_start_f=0;
                  tim1_light_left_start_f=0;
                  tim1_light_right_start_f=0;
                  TIM1_Cmd(DISABLE);//关闭定时器
                  GPIO_WriteHigh(LIGHT_LEFT_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_LEFT_GPIO_PIN);
                  GPIO_WriteHigh(LIGHT_RIGHT_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_RIGHT_GPIO_PIN);
                }
              }
              if (key==new1){
                motor_front_back_speed = 1;
              }
              if (key==new3){
                motor_front_back_speed = 2;
              } 
              if (key==new7){
                motor_left_right_speed = 1;
              }
              if (key==new9){
                motor_left_right_speed = 2;
              } 

            delay_ms(200);//防抖动
            }

        
    }
  
 
}

volatile u8 fac_us=0;
void delay_init(u8 clk)
{
  if(clk>16)fac_us=(16-4)/4;
  else if(clk>4)fac_us=(clk-4)/4;
  else fac_us=1;
}

void delay_us(u16 nus)
{
  __asm(
"PUSH A          \n"    //1T,压栈
"DELAY_XUS:      \n"
"LD A,fac_us     \n"    //1T,fac_us加载到累加器A
"DELAY_US_1:     \n"
"NOP             \n"    //1T,nop延时
"DEC A           \n"    //1T,A--
"JRNE DELAY_US_1 \n"    //不等于0,则跳转(2T)到DELAY_US_1继续执行,若等于0,则不跳转(1T).
"NOP             \n"    //1T,nop延时
"DECW X          \n"    //1T,x--
"JRNE DELAY_XUS  \n"    //不等于0,则跳转(2T)到DELAY_XUS继续执行,若等于0,则不跳转(1T).
"POP A           \n"    //1T,出栈
);
}

void delay_ms(u32 nms)
{
  u8 t;
  if(nms>65)
  {
    t=nms/65;
    while(t--)delay_us(65000);
    nms=nms%65;
  }
  delay_us(nms*1000);
}


INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11){
  
  if(tim1_light_right_start_f==1){
    GPIO_WriteReverse(LIGHT_RIGHT_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_RIGHT_GPIO_PIN);
  }
  if(tim1_light_left_start_f==1){
    GPIO_WriteReverse(LIGHT_LEFT_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_LEFT_GPIO_PIN);
  }
  if((tim1_light_right_start_f==1 || tim1_light_left_start_f==1)&&tim1_emergency_start_f==0){
    GPIO_WriteReverse(BUZZER_GPIO_PORT, (GPIO_Pin_TypeDef)BUZZER_GPIO_PIN);
  }
  TIM1_ClearFlag(TIM1_FLAG_UPDATE);//清除更新标志
  TIM1_ClearITPendingBit(TIM1_IT_UPDATE);
   
}

void TIM1_Init(void){
    
    TIM1_DeInit();
    TIM1_TimeBaseInit(15,TIM1_COUNTERMODE_UP, 10000,50); //最大10ms溢出，1us加1，只有向上计数
    TIM1_ARRPreloadConfig(ENABLE);
    TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);//允许更新/溢出中断

}

void steper_one_step(uint8_t direction, uint8_t speed){//72度

	if(direction==0){
		A_a_1();A_b_0();B_a_0();B_b_0();
		delay_ms(speed);
		A_a_0();A_b_0();B_a_1();B_b_0();
		delay_ms(speed);
		A_a_0();A_b_1();B_a_0();B_b_0();
		delay_ms(speed);
		A_a_0();A_b_0();B_a_0();B_b_1();
		delay_ms(speed);
	}else{
		A_a_0();A_b_0();B_a_0();B_b_1();
		delay_ms(speed);
		A_a_0();A_b_1();B_a_0();B_b_0();
		delay_ms(speed);
		A_a_0();A_b_0();B_a_1();B_b_0();
		delay_ms(speed);
		A_a_1();A_b_0();B_a_0();B_b_0();
		delay_ms(speed);
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

void stop_all(void){
  steper_stop();//关闭步进电机
  
  TIM1_Cmd(DISABLE);//关闭定时器
  GPIO_WriteHigh(LIGHT_LEFT_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_LEFT_GPIO_PIN);//关闭左转向灯
  GPIO_WriteHigh(LIGHT_RIGHT_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_RIGHT_GPIO_PIN);//关闭左转向灯
  GPIO_WriteHigh(LIGHT_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_FRONT_GPIO_PIN);//关闭前向灯
  GPIO_WriteHigh(LIGHT_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_BACK_GPIO_PIN);//关闭尾灯
  GPIO_WriteHigh(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PIN);//关闭左转向灯

  GPIO_WriteLow(BUZZER_GPIO_PORT, (GPIO_Pin_TypeDef)BUZZER_GPIO_PIN);//关闭蜂鸣器
  
  GPIO_WriteLow(MOTOR_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_FRONT_GPIO_PIN);//关闭进退电机
  GPIO_WriteLow(MOTOR_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_BACK_GPIO_PIN);//关闭进退电机

}
#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

