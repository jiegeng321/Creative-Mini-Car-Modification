#include "remote.h"

//ѡ��16MHz��HSI��Ϊ��ʱ��Դ
void Clk_Init(void){

    CLK_DeInit();//����ΪĬ��ֵ
    CLK_HSICmd(ENABLE);//����HSI
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);//HSI��Ƶ
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);//CPU��Ƶ
}

//��ʼ��PD3Ϊ��������
void IR_GPIO_Init(void){

    GPIO_DeInit(GPIOD);//�ָ�ָ���˿�
    GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_IN_FL_NO_IT);//����PD3Ϊ�����������ж�
    
}

void TIM2_Init(void){
    
    TIM2_DeInit();
    TIM2_TimeBaseInit(TIM2_PRESCALER_16, 10000); //���10ms�����1us��1��ֻ�����ϼ���
    TIM2_ARRPreloadConfig(ENABLE);
    //TIM2_ClearFlag(TIM2_FLAG_UPDATE);//������±�־
    TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);//�������/����ж�
    //Send_Str("TIM2_Cmd before \n");
    TIM2_Cmd(ENABLE);//������ʱ��
    //Send_Str("TIM2_Cmd after \n");
    //TIM2_UpdateRequestConfig(TIM2_UPDATESOURCE_REGULAR);//�ж�Դѡ��Ϊֻ��������ܴ���
    //TIM2_GenerateEvent(TIM2_EVENTSOURCE_UPDATE);//���������¼����������жϣ��˴���������Ԥ��Ƶ����
    
    enableInterrupts(); //�����ж�
    
    //CH2�������ز���IC2ӳ����TI2FP2�ϡ�����Ԥ��ƵΪ1�����˲����Ҳ�����=��ʱ��Ƶ��
    //TIM2_PWMIConfig(TIM2_CHANNEL_2, TIM2_ICPOLARITY_RISING, TIM2_ICSELECTION_DIRECTTI, TIM2_ICPSC_DIV1, 0x00);
    //CH2�������ز���IC2ӳ����TI2FP2�ϡ�����Ԥ��ƵΪ1�����˲����Ҳ�����=��ʱ��Ƶ��
    TIM2_ICInit(TIM2_CHANNEL_2, TIM2_ICPOLARITY_RISING, TIM2_ICSELECTION_DIRECTTI, TIM2_ICPSC_DIV1, 0x00);
    TIM2_ITConfig(TIM2_IT_CC2, ENABLE);//��׽�Ƚ�2�ж�ʹ��
    TIM2_ClearITPendingBit(TIM2_IT_CC1);
    TIM2_ClearFlag(TIM2_FLAG_CC1);
    //TIM2_CCxCmd(TIM2_CHANNEL_2, ENABLE);//���벶��/�Ƚ����ʹ��
    
}




//TIM2Ϊ���벶��
void Remote_Init(){
  
    IR_GPIO_Init();
    TIM2_Init();
    //printf("Remote Init \n");
}


//RmtSta����ʶң��������״̬
//[7]:�յ����������־
//[6]:�õ���һ��������������Ϣ
//[5]:����	
//[4]:����������Ƿ��Ѿ�������								   
//[3:0]:�����ʱ��
u8 	RmtSta=0;	  	  
u16 Dval;		//�½���ʱ��������ֵ
u32 RmtRec=0;	//������յ�������	   		    
u8  RmtCnt=0;	//�������µĴ���	

/**
  * @brief Timer2 ����/�����/ɲ�� �жϳ���.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13){
  

    if(RmtSta&0x80){//RmtSta[7]�Ƿ���1,���ϴ�������(������)�����յ���
    	
        RmtSta&=~0X10;//ȡ���������Ѿ���������(��RmtSta[4]��0)
        if((RmtSta&0X0F)==0X00)//��RtmSta[3:0]�����ж�,Ϊ0�����һ�����
            RmtSta|=1<<6;	//����Ѿ����һ�ΰ����ļ�ֵ��Ϣ�ɼ�(��RmtSta[6]λ��1)
        if((RmtSta&0X0F)<14){//�ߵ�ƽ����ʱ��<130ms,���ܻ������������е�97.94ms�ߵ�ƽ�׶�
            RmtSta++;
        }else{//�������130ms,�ͱ�ʾң�����ϰ����Ѿ��ɿ���
        
            RmtSta&=~(1<<7);					//���������ʶ
            RmtSta&=0XF0;						//��ռ�����	
        }								 	   	
   }
   TIM2_ClearFlag(TIM2_FLAG_UPDATE);//������±�־
   TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
   
}




/**
  * @brief Timer2 ����/�Ƚ� �жϳ���.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_CAP_COM_IRQHandler, 14){
    
    BitStatus bs;
    bs = GPIO_ReadInputPin(GPIOD, GPIO_PIN_3);//��PD3�������ƽ״̬
    if (bs){//�����ز���
        TIM2_OC2PolarityConfig(TIM2_OCPOLARITY_LOW);//��Ϊ�½��ز���
        TIM2_SetCounter(0x0000);//��ն�ʱ����ֵ
        RmtSta|=0X10;//RmtSta[4]λ��1 ����������Ѿ������� 
        
    }else{//�½��ز���
        Dval = TIM2_GetCounter();//��ȡCCR2Ҳ������CC2IF�����־λ
        TIM2_OC2PolarityConfig(TIM2_OCPOLARITY_HIGH);//��Ϊ�����ز���
        if(RmtSta&0x10){//���һ�θߵ�ƽ����(���RmtSta[4]Ϊ1)
            
            if(RmtSta&0X80){//���յ���������(���RmtSta[7]Ϊ1)
                
                if(Dval>300&&Dval<800){			//560Ϊ��׼ֵ,560us(NEC�е�0λ)
                
                    RmtRec<<=1;					//��RmtRec���������һλ.������õ��ǵ�λ��ǰ,��λ�ں���㷨
                    RmtRec|=0;					//���յ�0
                    
                }else if(Dval>1400&&Dval<1800){	//1680Ϊ��׼ֵ,1680us(NEC�е�1λ)
                
                    RmtRec<<=1;					//����һλ.
                    RmtRec|=1;					//���յ�1
                    
                }else if(Dval>2200&&Dval<2600){	//2500Ϊ��׼ֵ2.5ms,�õ�������ֵ���ӵ���Ϣ(������)
                
                    RmtCnt++; 					//������������1��
                    RmtSta&=0XF0;				//��ռ�ʱ��
                    
                }
            }else if(Dval>4200&&Dval<4700){		//4500Ϊ��׼ֵ4.5ms(������)
                RmtSta|=1<<7;					//RmtSta[7]λ��1,��ǳɹ����յ���������
                RmtCnt=0;						//�����������������
                
            }
       }
       RmtSta&=~(1<<4);//RmtSta[4]λ��0,����������ز����־
       
    }
    TIM2_ClearITPendingBit(TIM2_IT_CC2);
    
}

//����������
//����ֵ:
//0,û���κΰ�������
//����,���µİ�����ֵ.
u8 Remote_Scan(void){        
	u8 sta=0;       
    u8 t1,t2;  
   
	if(RmtSta&(1<<6))//�õ�һ��������������Ϣ��(RmtSta[7]λ�Ƿ�Ϊ1)
	{ 
	    t1=RmtRec>>24;			//�õ���ַ��(u32��RmtRec������24λ,ֻ������ַ���8λ)�޷���
	    t2=(RmtRec>>16)&0xff;	//�õ���ַ����(����16λ��ֻ������8λ)
 	//    if((t1==REMOTE_IDH)&&t2==REMOTE_IDL)//����ң��ʶ����(ID)����ַ 
	//    { 
	        t1=RmtRec>>8;//�õ�������
	        t2=RmtRec; 	//�õ����Ʒ���
	        if(t1==(u8)~t2)sta=t1;//��ֵ��ȷ
	//	}   
		if((sta==0)||((RmtSta&0X80)==0))//�������ݴ���/ң���Ѿ�û�а�����
		{
		 	RmtSta&=~(1<<6);//������յ���Ч������ʶ(RmtSta[7]λ��Ϊ1)
			RmtCnt=0;		//�����������������
		}
	}  
    return sta;
}
