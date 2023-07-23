#include "stm8s.h"
#include "remote.h"

#define LED_GPIO_PORT  (GPIOD)
#define LED_GPIO_PIN  (GPIO_PIN_2)//ָʾ��/���ڵ�

#define BUZZER_GPIO_PORT  (GPIOD)
#define BUZZER_GPIO_PIN  (GPIO_PIN_4) //������

#define MOTOR_FRONT_GPIO_PORT  (GPIOC)
#define MOTOR_FRONT_GPIO_PIN  (GPIO_PIN_6)//ǰ��ֱ�����
#define MOTOR_BACK_GPIO_PORT  (GPIOC)
#define MOTOR_BACK_GPIO_PIN  (GPIO_PIN_7) 

#define MOTOR_A1_GPIO_PORT  (GPIOC)
#define MOTOR_A1_GPIO_PIN  (GPIO_PIN_4)//���Ҳ������ A-
#define MOTOR_A2_GPIO_PORT  (GPIOC)
#define MOTOR_A2_GPIO_PIN  (GPIO_PIN_5)//���Ҳ������ A+
#define MOTOR_B1_GPIO_PORT  (GPIOC)
#define MOTOR_B1_GPIO_PIN  (GPIO_PIN_3)//���Ҳ������ B-
#define MOTOR_B2_GPIO_PORT  (GPIOA)
#define MOTOR_B2_GPIO_PIN  (GPIO_PIN_3) //���Ҳ������ B+

#define A_a_1() GPIO_WriteHigh(MOTOR_A1_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_A1_GPIO_PIN);//A-
#define A_a_0() GPIO_WriteLow(MOTOR_A1_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_A1_GPIO_PIN);//A-
#define A_b_1() GPIO_WriteHigh(MOTOR_A2_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_A2_GPIO_PIN);//A+
#define A_b_0() GPIO_WriteLow(MOTOR_A2_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_A2_GPIO_PIN);//A+
#define B_a_1() GPIO_WriteHigh(MOTOR_B1_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_B1_GPIO_PIN);//B-
#define B_a_0() GPIO_WriteLow(MOTOR_B1_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_B1_GPIO_PIN);//B-
#define B_b_1() GPIO_WriteHigh(MOTOR_B2_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_B2_GPIO_PIN);//B+
#define B_b_0() GPIO_WriteLow(MOTOR_B2_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_B2_GPIO_PIN);//B+

#define LIGHT_FRONT_GPIO_PORT  (GPIOD)
#define LIGHT_FRONT_GPIO_PIN  (GPIO_PIN_5)//ǰ��
#define LIGHT_BACK_GPIO_PORT  (GPIOD)
#define LIGHT_BACK_GPIO_PIN  (GPIO_PIN_6)//β��
#define LIGHT_LEFT_GPIO_PORT  (GPIOA)
#define LIGHT_LEFT_GPIO_PIN  (GPIO_PIN_1)//��ת��
#define LIGHT_RIGHT_GPIO_PORT  (GPIOA)
#define LIGHT_RIGHT_GPIO_PIN  (GPIO_PIN_2)//��ת��


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
  

    Clk_Init();//��ʱ�ӳ�ʼ��
    Remote_Init();//IR�����漰��صĳ�ʼ��
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
    
    u8 key;//������ռ�ֵ
    while(1){

            key=Remote_Scan();
             if(key==up&&motor_front_back_speed==2){//���˵�� ǰ�� ȫ��
                GPIO_WriteLow(MOTOR_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_FRONT_GPIO_PIN);
                for(int i=0;i<RmtCnt;i++){

                  GPIO_WriteHigh(MOTOR_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_BACK_GPIO_PIN);
                  delay_ms(115);
                }
                GPIO_WriteLow(MOTOR_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_FRONT_GPIO_PIN);
                GPIO_WriteLow(MOTOR_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_BACK_GPIO_PIN);
              }
             if(key==up&&motor_front_back_speed==1){//���˵�� ǰ�� ����
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
             if(key==down&&motor_front_back_speed==2){//���˵�� ���� ȫ��
               GPIO_WriteLow(MOTOR_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_BACK_GPIO_PIN);
               for(int i=0;i<RmtCnt;i++){
                GPIO_WriteHigh(MOTOR_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_FRONT_GPIO_PIN);
                delay_ms(115);
               }
               GPIO_WriteLow(MOTOR_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_FRONT_GPIO_PIN);
               GPIO_WriteLow(MOTOR_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_BACK_GPIO_PIN);
              }
              if(key==down&&motor_front_back_speed==1){//���˵�� ���� ����
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
             
              if(key==left&&motor_left_right_speed==2){//������� ��ת
                for(int i=0;i<RmtCnt;i++){
                  steper_one_step(1,5);
                }
                steper_stop();
              }
             
              if(key==left&&motor_left_right_speed==1){//������� ��ת
                for(int i=0;i<RmtCnt;i++){
                  steper_one_step(1,10);
                }
                steper_stop();
              }
              if(key==right&&motor_left_right_speed==2){//������� ��ת
                for(int i=0;i<RmtCnt;i++){
                  steper_one_step(0,5);
                }
                steper_stop();
              }
             
              if(key==right&&motor_left_right_speed==1){//������� ��ת
                for(int i=0;i<RmtCnt;i++){
                  steper_one_step(0,10);
                }
                steper_stop();
              }
             
            if(key&&RmtCnt==1){

              if(key==num_8){//������
                for(int i=0;i<150;i+=1){
		  GPIO_WriteReverse(BUZZER_GPIO_PORT, (GPIO_Pin_TypeDef)BUZZER_GPIO_PIN);
		  delay_ms(1);
		  }
                GPIO_WriteLow(BUZZER_GPIO_PORT, (GPIO_Pin_TypeDef)BUZZER_GPIO_PIN);
              }
              
              //if(key==up){//���˵�� ǰ��
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
              //if(key==new1){//���˵�� ǰ��
                //GPIO_WriteLow(MOTOR_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_FRONT_GPIO_PIN);
                
                //GPIO_WriteHigh(MOTOR_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_BACK_GPIO_PIN);
                
                //delay_ms(1000);
                //GPIO_WriteLow(MOTOR_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_FRONT_GPIO_PIN);
               // GPIO_WriteLow(MOTOR_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_BACK_GPIO_PIN);
             // }
              
              //if(key==down){//���˵�� ����
              //  GPIO_WriteHigh(MOTOR_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_FRONT_GPIO_PIN);
              //  GPIO_WriteLow(MOTOR_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_BACK_GPIO_PIN);
              //  delay_ms(motor_front_back_time_ms);
              //  GPIO_WriteLow(MOTOR_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_FRONT_GPIO_PIN);
              //  GPIO_WriteLow(MOTOR_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_BACK_GPIO_PIN);
              //}
              
              //if(key==left){//������� ��ת
              //  for(int i=0;i<steper_cycles_per_press;i++){
              //    steper_one_cycle(1,25);
              //  }
              //  steper_stop();
              //}
              
              //if(key==right){//������� ��ת
              //  for(int i=0;i<steper_cycles_per_press;i++){
              //    steper_one_cycle(0,25);
              //  }
              //  steper_stop();
              //}
              
              if(key==num_5){//ǰ��
               GPIO_WriteReverse(LIGHT_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_FRONT_GPIO_PIN);
              }
              if(key==num_0){//β��
               GPIO_WriteReverse(LIGHT_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_BACK_GPIO_PIN);
              }

            
              if(key==num_7){//��ת���
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
                    TIM1_Cmd(ENABLE);//������ʱ��
                  }
                  GPIO_WriteHigh(LIGHT_RIGHT_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_RIGHT_GPIO_PIN);
                }else{
                  tim1_light_right_start_f=0;
                  if(tim1_light_right_start_f==0 && tim1_light_left_start_f==0){
                    TIM1_Cmd(DISABLE);//�رն�ʱ��
                  }
                  GPIO_WriteHigh(LIGHT_RIGHT_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_RIGHT_GPIO_PIN);
                }
              }

              if(key==num_9){//��ת���
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
                    TIM1_Cmd(ENABLE);//������ʱ��
                  }
                  GPIO_WriteHigh(LIGHT_LEFT_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_LEFT_GPIO_PIN);
                }else{
                  tim1_light_left_start_f=0;
                  if(tim1_light_left_start_f==0 && tim1_light_right_start_f==0){
                    TIM1_Cmd(DISABLE);//�رն�ʱ��
                  }
                  GPIO_WriteHigh(LIGHT_LEFT_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_LEFT_GPIO_PIN);
                }
              }
              if (key==ok){//ֹͣ�����豸
                stop_all();
              }
              if(key==star){//ָʾ��/���ڵ�
               GPIO_WriteReverse(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PIN);
              }
              if (key==call){//����ͣ��
                if(tim1_emergency_start_f==0){
                  tim1_emergency_start_f=1;
                  tim1_light_left_start_f=1;
                  tim1_light_right_start_f=1;
                  TIM1_Cmd(DISABLE);//�رն�ʱ��
                  GPIO_WriteHigh(LIGHT_LEFT_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_LEFT_GPIO_PIN);
                  GPIO_WriteHigh(LIGHT_RIGHT_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_RIGHT_GPIO_PIN);
                  TIM1_Cmd(ENABLE);//������ʱ��
                }else{
                  tim1_emergency_start_f=0;
                  tim1_light_left_start_f=0;
                  tim1_light_right_start_f=0;
                  TIM1_Cmd(DISABLE);//�رն�ʱ��
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

            delay_ms(200);//������
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
"PUSH A          \n"    //1T,ѹջ
"DELAY_XUS:      \n"
"LD A,fac_us     \n"    //1T,fac_us���ص��ۼ���A
"DELAY_US_1:     \n"
"NOP             \n"    //1T,nop��ʱ
"DEC A           \n"    //1T,A--
"JRNE DELAY_US_1 \n"    //������0,����ת(2T)��DELAY_US_1����ִ��,������0,����ת(1T).
"NOP             \n"    //1T,nop��ʱ
"DECW X          \n"    //1T,x--
"JRNE DELAY_XUS  \n"    //������0,����ת(2T)��DELAY_XUS����ִ��,������0,����ת(1T).
"POP A           \n"    //1T,��ջ
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
  TIM1_ClearFlag(TIM1_FLAG_UPDATE);//������±�־
  TIM1_ClearITPendingBit(TIM1_IT_UPDATE);
   
}

void TIM1_Init(void){
    
    TIM1_DeInit();
    TIM1_TimeBaseInit(15,TIM1_COUNTERMODE_UP, 10000,50); //���10ms�����1us��1��ֻ�����ϼ���
    TIM1_ARRPreloadConfig(ENABLE);
    TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);//�������/����ж�

}

void steper_one_step(uint8_t direction, uint8_t speed){//72��

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


void steper_one_cycle(uint8_t direction, uint8_t speed){//360��
  for(int i=0;i<5;i++){
    steper_one_step(direction,speed);
  }
}

void stop_all(void){
  steper_stop();//�رղ������
  
  TIM1_Cmd(DISABLE);//�رն�ʱ��
  GPIO_WriteHigh(LIGHT_LEFT_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_LEFT_GPIO_PIN);//�ر���ת���
  GPIO_WriteHigh(LIGHT_RIGHT_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_RIGHT_GPIO_PIN);//�ر���ת���
  GPIO_WriteHigh(LIGHT_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_FRONT_GPIO_PIN);//�ر�ǰ���
  GPIO_WriteHigh(LIGHT_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)LIGHT_BACK_GPIO_PIN);//�ر�β��
  GPIO_WriteHigh(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PIN);//�ر���ת���

  GPIO_WriteLow(BUZZER_GPIO_PORT, (GPIO_Pin_TypeDef)BUZZER_GPIO_PIN);//�رշ�����
  
  GPIO_WriteLow(MOTOR_FRONT_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_FRONT_GPIO_PIN);//�رս��˵��
  GPIO_WriteLow(MOTOR_BACK_GPIO_PORT, (GPIO_Pin_TypeDef)MOTOR_BACK_GPIO_PIN);//�رս��˵��

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

