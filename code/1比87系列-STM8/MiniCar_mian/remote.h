#ifndef _REMOTE_H_
#define _REMOTE_H_

#include "stm8s.h"

#define REMOTE_IDH  0x0C
#define REMOTE_IDL  0xF3

#define num_0 0x98 //16750695 ff9867
#define num_1 0xa2 //16753245 ffa25d
#define num_2 0x62 //16736925 ff629d
#define num_3 0xe2 //16769565 ffe21d
#define num_4 0x22 //16720605 ff22dd
#define num_5 0x02 //16712445 ff02fd
#define num_6 0xc2 //16761405 ffc23d
#define num_7 0xe0 //16769055 ffe01f
#define num_8 0xa8 //16754775 ffa857
#define num_9 0x90 //16748655 ff906f

#define star 0x68 //16738455 ff6897
#define call 0xb0 //16756815 ffb04f

#define up 0x18 //16718055 ff18e7
#define left 0x10 //16716015 ff10ef
#define right 0x5a //16734885 ff5aa5
#define down 0x4a //16730805 ff4ab5
#define ok 0x38 //16726215 ff38c7

#define new1 0x30 //16724175 ff30cf
#define new3 0x7a //16743045 ff7a85
#define new7 0x42 //16728765 ff42bd
#define new9 0x52 //16732845 ff52ad

extern u8 RmtCnt;//按键按下的次数
void Clk_Init(void);
void Remote_Init(void);
u8 Remote_Scan(void);

#endif
