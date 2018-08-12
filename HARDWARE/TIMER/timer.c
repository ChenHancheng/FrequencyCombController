#include "timer.h"

//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
int count=0;

unsigned char key_choice = 0;

int ADCValueInner=0;
int SetPointInner=ADVALUE_MID;
int ErrorInner=0,  ErrorInnerSum=0;
int IntegerInnerUpLim=3000, IntegerInnerDownLim=-3000;
int OutputInner=0, PInner=-36, IntegrateInner=-2;

int ADCValueInner1=0;
int SetPointInner1=1862;
int ErrorInner1=0,  ErrorInnerSum1=0;
int IntegerInnerUpLim1=3000, IntegerInnerDownLim1=-3000;
//int OutputInner1=0, PInner1=0, IntegrateInner1=0;

int ADCValueOuter=0;
int SetPointOuter=ADVALUE_MID;
int ErrorOuter=0,  ErrorOuterSum=0;
int IntegerOuterUpLim=3000, IntegerOuterDownLim=-3000;
int OutputOuter=0, POuter=-30, IntegrateOuter=-2;

void Timer2_Init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = 100-1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������100Ϊ10ms
	TIM_TimeBaseStructure.TIM_Prescaler = 7200-1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM2, //TIM2
		TIM_IT_Update  |  //TIM �ж�Դ
		TIM_IT_Trigger,   //TIM �����ж�Դ 
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx����
}
void Timer4_Init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = 5000-1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler = 7200-1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM4, //TIM2
		TIM_IT_Update  |  //TIM �ж�Դ
		TIM_IT_Trigger,   //TIM �����ж�Դ 
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx����
}

void TIM2_IRQHandler(void)   //TIM2�ж� 100Hz�ж���,�������ƹ������¶�
{
	TIM_ClearFlag(TIM2, TIM_IT_Update);
	ADCValueInner = Get_Adc(0);
	ADCValueInner1 = Get_Adc(1); 
	//û���������¶ȿ���Ϊ�ڿ�ģʽ
	if(key_choice == 0 ) {
		ADCValueInner = KalmanFilter(ADCValueInner); //�ڿ�ģʽ�£��������˲���״̬����Ϊx(k) = x(k-1),Q1=xx;	
		
	  ErrorInner = ADCValueInner - SetPointInner;
	  ErrorInnerSum = ErrorInnerSum + ErrorInner;
		
		if(ErrorInnerSum > IntegerInnerUpLim) ErrorInnerSum = IntegerInnerUpLim;
    if(ErrorInnerSum < IntegerInnerDownLim) ErrorInnerSum = IntegerInnerDownLim;
	
	  OutputInner = PInner*ErrorInner/100 + IntegrateInner*ErrorInnerSum/100+1500;
	}
	//��Ƶ�������¶ȿ���Ϊ�ⲿ����ģʽ
	else{
		ADCValueInner1 = KalmanFilter(ADCValueInner1); //���ģʽ�£��������˲���״̬����Ϊx(k) = x(k-1),Q2=xx;(Q2>Q1)
		
		ErrorInner1 = (ADCValueInner1 - SetPointInner1);
	  ErrorInnerSum1 = ErrorInnerSum1 + ErrorInner1;
		
		if(ErrorInnerSum1 > IntegerInnerUpLim1) ErrorInnerSum1 = IntegerInnerUpLim1;
    if(ErrorInnerSum1 < IntegerInnerDownLim1) ErrorInnerSum1 = IntegerInnerDownLim1;
	
	  OutputInner = PInner*ErrorInner1/800 + IntegrateInner*ErrorInnerSum1/1000+1500;
	}
	
	OutputInner = OutputInner>3299?3299:OutputInner;
	OutputInner = OutputInner<0?0:OutputInner;
	Dac1_Set_Vol(OutputInner);
}

void TIM4_IRQHandler(void)   //TIM4�ж� 0.5Hz�ж��ʣ��������ƺ��º��¶�
{
	TIM_ClearFlag(TIM4, TIM_IT_Update);

	ADCValueOuter = Get_Adc(2);
	
	ErrorOuter = ADCValueOuter - SetPointOuter;
	ErrorOuterSum = ErrorOuterSum + ErrorOuter;
	
	if(ErrorOuterSum > IntegerOuterUpLim) ErrorOuterSum = IntegerOuterUpLim;
  if(ErrorOuterSum < IntegerOuterDownLim) ErrorOuterSum = IntegerOuterDownLim;
	
	OutputOuter = POuter*ErrorOuter/800 + IntegrateOuter*ErrorOuterSum/1000+1500;
	
	OutputOuter = OutputOuter>3299?3299:OutputOuter;
	OutputOuter = OutputOuter<0?0:OutputOuter;
	
	Dac1_Set_Vol(OutputOuter);
}

