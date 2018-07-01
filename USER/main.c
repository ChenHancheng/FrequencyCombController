/**********************************************************
                       ��������					 
���ܣ���MUX������ο�Ƶ�ʾ�R��Ƶ��Ƶ�ʣ����������޸�Rֵ��
�ӿڣ�
				ģ��		STM32
				GND			GND
				PLL_SCK	PCout(11)
				PLL_SDI	PCout(10)
				PLL_SEN	PCout(9)
				
ʱ�䣺2015/11/3
�汾��1.0
���ߣ���������
������

������������뵽�Ա��꣬�������ӽ߳�Ϊ������ ^_^
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
	delay_init(72);	     //��ʱ��ʼ��
	NVIC_Configuration();
	uart_init(256000);   //���ڳ�ʼ��
	key_init();//������ʼ��
	initial_lcd();//Һ����ʼ��
	InitPll();
	 
	AdcInnerInit(); 
	Timer2_Init(); //���ºй����̿����ж�
	//Timer4_Init(); //���º�ǻ����¿���
	Dac1_Init();
	 
	LED_Init();
	 
	LCD_Clear();
	LCD_Refresh_Gram();
	USBRelinkConfig();
	 
	RDivideTest(0); //ADF4002 ������Ƶ����Ϊ��Ƶ������ʹ��
	 
	while(1)
	{
		KeyRead();//������ȡ
		Set_PointFre(Keycode, 0);//��������
		if(_return){_return=0;LCD_Refresh_Gram();}//������ʾ
		KEY_EXIT();
		USB_TxWrite("��ð�����\r\n", 12);
		
		
	  sprintf((char*)display, (const char*)"ADInner:%4d", ADCValueInner);
	  OLED_ShowString(0, 4, display);
		sprintf((char*)display, (const char*)"OpInner:%4d", OutputInner);
	  OLED_ShowString(0, 6, display);
		
		LCD_Refresh_Gram();
	}
 }