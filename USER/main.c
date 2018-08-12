/**********************************************************
                       康威电子					 
功能：用MUX脚输出参考频率经R分频后频率，按键操作修改R值。
接口：
				模块		STM32
				GND			GND
				PLL_SCK	PCout(11)
				PLL_SDI	PCout(10)
				PLL_SEN	PCout(9)
				
时间：2015/11/3
版本：1.0
作者：康威电子
其他：

更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://kvdz.taobao.com/

**********************************************************/
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "usart.h"
#include "exti.h"
#include "wdg.h"
#include "timer.h"
#include "stmflash.h"
#include "usb_lib.h"
#include "math.h"
#include "hw_config.h"
#include "string.h"
#include "stdlib.h"
#include "lcd.h"
#include "task_manage.h"
#include "adf4002.h"
#include "adc.h"
#include "dac.h"

	 int adc_value;

int main(void)
 {
	u8 display[20];
	 
  SystemInit();
	delay_init(72);	     //延时初始化
	NVIC_Configuration();
	uart_init(256000);   //串口初始化
	key_init();//按键初始化
	initial_lcd();//液晶初始化
	InitPll();
	 
	AdcInnerInit(); 
	Timer2_Init(); //恒温盒光纤盘控制中断
	//Timer4_Init(); //恒温盒腔体恒温控制
	Dac1_Init();
	 
	LED_Init();
	 
	LCD_Clear();
	LCD_Refresh_Gram();
	USBRelinkConfig();
	 
	RDivideTest(1);	
	while(1)
	{
		KeyRead();//按键读取
		Set_PointFre(Keycode, 0);//按键处理
		if(_return){_return=0;LCD_Refresh_Gram();}//更新显示
		KEY_EXIT();
		
	  sprintf((char*)display, (const char*)"AD:%4d %4d", ADCValueInner, ADCValueInner1);
	  OLED_ShowString(0, 4, display);
		sprintf((char*)display, (const char*)"Out:%4d %2d", OutputInner, key_choice);
	  OLED_ShowString(0, 6, display);
		
		sprintf((char*)display, (const char*)"T%.2f", ((-(float) ADCValueInner)*0.4491+3795.33)/100);
		OLED_ShowString(80, 2, display);
		
		LCD_Refresh_Gram();
	}
 }
