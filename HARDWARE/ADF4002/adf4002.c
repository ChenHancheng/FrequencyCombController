
/**********************************************************
                       �����Ƽ�
������������뵽�Ա��꣬�������ӽ߳�Ϊ������ ^_^
https://kvdz.taobao.com/
**********************************************************/

#include "adf4002.h"
#include "sys.h"

long int ReadData;

//long int Reg0x00 = 0x001F40;//r
//////long int Reg0x01 = 0x0DAC01;//n

long int Reg0x02 = 0x0D80C2; //function
long int Reg0x03 = 0x0D80C3;//init
//long int Reg0x02 = 0x0D80B2; //function DVDD
//long int Reg0x03 = 0x0D80B3;//init DVDD

long int Reg0x02_LEDON  = 0x0D80B2; //function
long int Reg0x02_LEDOFF = 0x0D80F2; //function


void Delay(unsigned int z)
{
  unsigned int i,j;
  
  for(i = z; i > 0; i--)
    for(j = 10; j > 0; j--) ;
}

void DelayMs(void)
{
  unsigned int i, j;
  
  for(i = 0; i < 1000; i++)
  {
    for(j = 0; j < 1000; j++)
    {
      Delay(1000);
    }
  }
}

void InitPll(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  //��ADF4002����ͨ�ŵ�IO�ڳ�ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PC�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//MUXOUT��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//��������ģʽ
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	PLL_SCK_0;
	PLL_SDI_0;
	PLL_SEN_0;   
	SendDataPll(Reg0x03);//init
	SendDataPll(Reg0x02); //funtion
}
void testADF4002(void)
{
  InitPll();
  SendDataPll(Reg0x02_LEDOFF); Delay(15);
  SendDataPll(Reg0x02_LEDON); Delay(15);
  SendDataPll(Reg0x02_LEDOFF); Delay(15);
  SendDataPll(Reg0x02_LEDON); Delay(15);
  
}
void SendDataPll(unsigned long int Data)          //���͵�����
{
  unsigned char i;

	PLL_SCK_0;
  PLL_SEN_0;  
  
  for(i = 0; i < 24; i++)
  {
    if(Data & 0x800000)
    {
      PLL_SDI_1;
    }
    else
    {
       PLL_SDI_0;
    }
    Data <<= 1;     
    PLL_SCK_1;
    
    Delay(100);
    
    PLL_SCK_0;
    Delay(100);
  }
  PLL_SDI_0;
  
  PLL_SEN_0;
  Delay(100);
  PLL_SEN_1;
////  Delay(100);
////  PLL_SEN_0;
}
//void SetFrequency(u32 fvco)
//{
//  u16 R;
//  u16 NN;
//  double bl;
//  u32 S_R = 0, S_N = 0;
//  bl = (double)fvco/30000000;
//  if(bl < 1.01)
//  {
//    R = 8191;
//    NN = (u16)(bl*8191);
//  }
//  else if(bl < 10.01)
//  {
//     R = 800;
//     NN = (u16)(bl*800);
//  }
//  else if(bl < 100.01)
//  {
//    R = 80;
//    NN = (u16)(bl*80);
//  }
//  else
//  {
//    R = 1;
//    NN = (u16)bl; 
//  }
//  S_R = Pre_R + (R<<2) + R_Address;
//  S_N = Pre_N + (NN<<8) + N_Address;
////  SendDataPll(S_R);
////  SendDataPll(S_N);
////  
//  SendDataPll((1000<<2));
//  SendDataPll((1000<<8)+0x01);
//}
  


void SetFrequency(u32 fvco)
{
	 u16 NN;
  double bl;
	u16 R = 1;//R��Ƶ
	u32 S_R = 0, S_N = 0;
  bl = ((double)fvco)/100000000;
////  R = 1;
  NN = (u16)(bl*R);
  S_R = Pre_R + (R<<2) + R_Address;
  S_N = Pre_N + (NN<<8) + N_Address;
  SendDataPll(S_R);     
  SendDataPll(S_N);
  SendDataPll(Reg0x02); //funtion
  SendDataPll(Reg0x03);//init
  SendDataPll(S_N);
  SendDataPll(S_R);
}

void RDivideTest(u16 RData)
{
	u16 NN;
	u32 S_R = 0, S_N = 0;
	double bl = 10;
	
  NN = (u16)(bl*RData);
  S_R = Pre_R + (RData<<2) + R_Address;
  S_N = Pre_N + (NN<<8) + N_Address;
  SendDataPll(S_R);     
  SendDataPll(S_N);
  SendDataPll(Reg0x02); //funtion
  SendDataPll(Reg0x03);//init
  SendDataPll(S_N);
  SendDataPll(S_R);
}


