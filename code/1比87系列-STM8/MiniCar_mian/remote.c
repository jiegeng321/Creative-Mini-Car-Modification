#include "remote.h"

//选用16MHz的HSI作为主时钟源
void Clk_Init(void){

    CLK_DeInit();//设置为默认值
    CLK_HSICmd(ENABLE);//启用HSI
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);//HSI分频
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);//CPU分频
}

//初始化PD3为浮空输入
void IR_GPIO_Init(void){

    GPIO_DeInit(GPIOD);//恢复指定端口
    GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_IN_FL_NO_IT);//设置PD3为浮空输入无中断
    
}

void TIM2_Init(void){
    
    TIM2_DeInit();
    TIM2_TimeBaseInit(TIM2_PRESCALER_16, 10000); //最大10ms溢出，1us加1，只有向上计数
    TIM2_ARRPreloadConfig(ENABLE);
    //TIM2_ClearFlag(TIM2_FLAG_UPDATE);//清除更新标志
    TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);//允许更新/溢出中断
    //Send_Str("TIM2_Cmd before \n");
    TIM2_Cmd(ENABLE);//开启定时器
    //Send_Str("TIM2_Cmd after \n");
    //TIM2_UpdateRequestConfig(TIM2_UPDATESOURCE_REGULAR);//中断源选择为只有溢出才能触发
    //TIM2_GenerateEvent(TIM2_EVENTSOURCE_UPDATE);//产生更新事件，不触发中断（此处即更新了预分频器）
    
    enableInterrupts(); //开启中断
    
    //CH2、上升沿捕获、IC2映射在TI2FP2上、捕获预分频为1、无滤波器且采样率=主时钟频率
    //TIM2_PWMIConfig(TIM2_CHANNEL_2, TIM2_ICPOLARITY_RISING, TIM2_ICSELECTION_DIRECTTI, TIM2_ICPSC_DIV1, 0x00);
    //CH2、上升沿捕获、IC2映射在TI2FP2上、捕获预分频为1、无滤波器且采样率=主时钟频率
    TIM2_ICInit(TIM2_CHANNEL_2, TIM2_ICPOLARITY_RISING, TIM2_ICSELECTION_DIRECTTI, TIM2_ICPSC_DIV1, 0x00);
    TIM2_ITConfig(TIM2_IT_CC2, ENABLE);//捕捉比较2中断使能
    TIM2_ClearITPendingBit(TIM2_IT_CC1);
    TIM2_ClearFlag(TIM2_FLAG_CC1);
    //TIM2_CCxCmd(TIM2_CHANNEL_2, ENABLE);//输入捕获/比较输出使能
    
}




//TIM2为输入捕获
void Remote_Init(){
  
    IR_GPIO_Init();
    TIM2_Init();
    //printf("Remote Init \n");
}


//RmtSta来标识遥控器接收状态
//[7]:收到了引导码标志
//[6]:得到了一个按键的所有信息
//[5]:保留	
//[4]:标记上升沿是否已经被捕获								   
//[3:0]:溢出计时器
u8 	RmtSta=0;	  	  
u16 Dval;		//下降沿时计数器的值
u32 RmtRec=0;	//红外接收到的数据	   		    
u8  RmtCnt=0;	//按键按下的次数	

/**
  * @brief Timer2 更新/上溢出/刹车 中断程序.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13){
  

    if(RmtSta&0x80){//RmtSta[7]是否置1,则上次有数据(引导码)被接收到了
    	
        RmtSta&=~0X10;//取消上升沿已经被捕获标记(将RmtSta[4]置0)
        if((RmtSta&0X0F)==0X00)//对RtmSta[3:0]进行判断,为0代表第一次溢出
            RmtSta|=1<<6;	//标记已经完成一次按键的键值信息采集(将RmtSta[6]位置1)
        if((RmtSta&0X0F)<14){//高电平持续时间<130ms,可能还处于连发码中的97.94ms高电平阶段
            RmtSta++;
        }else{//如果超过130ms,就表示遥控器上按键已经松开了
        
            RmtSta&=~(1<<7);					//清空引导标识
            RmtSta&=0XF0;						//清空计数器	
        }								 	   	
   }
   TIM2_ClearFlag(TIM2_FLAG_UPDATE);//清除更新标志
   TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
   
}




/**
  * @brief Timer2 捕获/比较 中断程序.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_CAP_COM_IRQHandler, 14){
    
    BitStatus bs;
    bs = GPIO_ReadInputPin(GPIOD, GPIO_PIN_3);//读PD3的输入电平状态
    if (bs){//上升沿捕获
        TIM2_OC2PolarityConfig(TIM2_OCPOLARITY_LOW);//改为下降沿捕获
        TIM2_SetCounter(0x0000);//清空定时器的值
        RmtSta|=0X10;//RmtSta[4]位置1 标记上升沿已经被捕获 
        
    }else{//下降沿捕获
        Dval = TIM2_GetCounter();//读取CCR2也可以清CC2IF捕获标志位
        TIM2_OC2PolarityConfig(TIM2_OCPOLARITY_HIGH);//改为上升沿捕获
        if(RmtSta&0x10){//完成一次高电平捕获(如果RmtSta[4]为1)
            
            if(RmtSta&0X80){//接收到了引导码(如果RmtSta[7]为1)
                
                if(Dval>300&&Dval<800){			//560为标准值,560us(NEC中的0位)
                
                    RmtRec<<=1;					//将RmtRec这个数左移一位.这里采用的是低位在前,高位在后的算法
                    RmtRec|=0;					//接收到0
                    
                }else if(Dval>1400&&Dval<1800){	//1680为标准值,1680us(NEC中的1位)
                
                    RmtRec<<=1;					//左移一位.
                    RmtRec|=1;					//接收到1
                    
                }else if(Dval>2200&&Dval<2600){	//2500为标准值2.5ms,得到按键键值增加的信息(连发码)
                
                    RmtCnt++; 					//按键次数增加1次
                    RmtSta&=0XF0;				//清空计时器
                    
                }
            }else if(Dval>4200&&Dval<4700){		//4500为标准值4.5ms(引导码)
                RmtSta|=1<<7;					//RmtSta[7]位置1,标记成功接收到了引导码
                RmtCnt=0;						//清除按键次数计数器
                
            }
       }
       RmtSta&=~(1<<4);//RmtSta[4]位置0,即清除上升沿捕获标志
       
    }
    TIM2_ClearITPendingBit(TIM2_IT_CC2);
    
}

//处理红外键盘
//返回值:
//0,没有任何按键按下
//其他,按下的按键键值.
u8 Remote_Scan(void){        
	u8 sta=0;       
    u8 t1,t2;  
   
	if(RmtSta&(1<<6))//得到一个按键的所有信息了(RmtSta[7]位是否为1)
	{ 
	    t1=RmtRec>>24;			//得到地址码(u32的RmtRec码右移24位,只保留地址码的8位)无符号
	    t2=(RmtRec>>16)&0xff;	//得到地址反码(右移16位后只保留低8位)
 	//    if((t1==REMOTE_IDH)&&t2==REMOTE_IDL)//检验遥控识别码(ID)及地址 
	//    { 
	        t1=RmtRec>>8;//得到控制码
	        t2=RmtRec; 	//得到控制反码
	        if(t1==(u8)~t2)sta=t1;//键值正确
	//	}   
		if((sta==0)||((RmtSta&0X80)==0))//按键数据错误/遥控已经没有按下了
		{
		 	RmtSta&=~(1<<6);//清除接收到有效按键标识(RmtSta[7]位设为1)
			RmtCnt=0;		//清除按键次数计数器
		}
	}  
    return sta;
}
