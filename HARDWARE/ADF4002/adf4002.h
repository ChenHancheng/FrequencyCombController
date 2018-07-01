
/**********************************************************
                       �����Ƽ�
������������뵽�Ա��꣬�������ӽ߳�Ϊ������ ^_^
https://kvdz.taobao.com/
**********************************************************/

#ifndef _adf4002_h_
#define _adf4002_h_

#include "sys.h"

#define R_Address 0X000000
#define N_Address 0X000001
#define F_Address 0X000002
#define I_Address 0X000003
#define Pre_R 0X000000          //X000,0000,DAT(14),00
#define Pre_N 0X000000          //XX0,DAT(13),XXXXXX,01

//�ܽ�����
//GND
#define PLL_SCK PCout(11)// PA8
#define PLL_SDI PCout(10)// PD2
#define PLL_SEN PCout(9)// PD2

#define PLL_SCK_0 PLL_SCK = 0
#define PLL_SCK_1 PLL_SCK = 1

#define PLL_SDI_0 PLL_SDI = 0
#define PLL_SDI_1 PLL_SDI = 1

#define PLL_SEN_0 PLL_SEN = 0
#define PLL_SEN_1 PLL_SEN = 1
                                                                                                                                                                                                                                                                                                                        

void InitPll(void);
void testADF4002(void);
void Delay(unsigned int z);
void DelayMs(void);
void SendDataPll(unsigned long int Data);
void SetFrequency(u32 fvco);

void RDivideTest(u16 RData);

#endif
