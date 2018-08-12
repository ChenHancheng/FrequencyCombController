#include "timer.h"

//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = 100-1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到100为10ms
	TIM_TimeBaseStructure.TIM_Prescaler = 7200-1; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM2, //TIM2
		TIM_IT_Update  |  //TIM 中断源
		TIM_IT_Trigger,   //TIM 触发中断源 
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设
}
void Timer4_Init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = 5000-1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler = 7200-1; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM4, //TIM2
		TIM_IT_Update  |  //TIM 中断源
		TIM_IT_Trigger,   //TIM 触发中断源 
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM4, ENABLE);  //使能TIMx外设
}

void TIM2_IRQHandler(void)   //TIM2中断 100Hz中断率,用来控制光纤盘温度
{
	TIM_ClearFlag(TIM2, TIM_IT_Update);
	ADCValueInner = Get_Adc(0);
	ADCValueInner1 = Get_Adc(1); 
	//没有锁定，温度控制为内控模式
	if(key_choice == 0 ) {
		ADCValueInner = KalmanFilter(ADCValueInner); //内控模式下，卡尔曼滤波的状态方程为x(k) = x(k-1),Q1=xx;	
		
	  ErrorInner = ADCValueInner - SetPointInner;
	  ErrorInnerSum = ErrorInnerSum + ErrorInner;
		
		if(ErrorInnerSum > IntegerInnerUpLim) ErrorInnerSum = IntegerInnerUpLim;
    if(ErrorInnerSum < IntegerInnerDownLim) ErrorInnerSum = IntegerInnerDownLim;
	
	  OutputInner = PInner*ErrorInner/100 + IntegrateInner*ErrorInnerSum/100+1500;
	}
	//重频锁定，温度控制为外部控制模式
	else{
		ADCValueInner1 = KalmanFilter(ADCValueInner1); //外控模式下，卡尔曼滤波的状态方程为x(k) = x(k-1),Q2=xx;(Q2>Q1)
		
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

void TIM4_IRQHandler(void)   //TIM4中断 0.5Hz中断率，用来控制恒温盒温度
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

