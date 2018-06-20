
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "ultra_sonic.h"
#include "motor_driver.h"
#include "UART.h"
#include "redray.h"
#include "Bat_adc.h"


#define left_edge	 0
#define left_middle	 1
#define center	     2
#define right_middle 3
#define right_edge   4



volatile uint16_t distance;
volatile uint16_t timer_cnt;
char buffer[50];
unsigned int val , right_curve = 0 , left_curve = 0;
UART uart0 = UART(0,BAUD_9600); //USB UART
UART uart1 = UART(1,BAUD_9600); //BlueTooth UART
BAT_ADC bat = BAT_ADC(5);
int bat_status = 0;

ISR(TIMER0_OVF_vect)
{
	bat.cnt++;
	timer_cnt++;
	if(bat.cnt > (250 * bat.ISR_Sec))
	{
	 bat_status = bat.Bat_status();
	 sprintf(buffer,"BAT:%d\r\n",bat_status);  //배터리 전압 UART1번으로 전송
	 uart1.TxString(buffer);
	 
	bat.cnt = 0;
	}
	
	if(timer_cnt > 62)
	{
	 sprintf(buffer,"val : 0x%02X\r\n",val);
	 uart1.TxString(buffer);
	 timer_cnt = 0;
	}
	
}

int main(void)
{
	motor_driver mymotor;
	ultra_sonic mysonic;
	
	DDRF &= ~(1<<PINF0) & ~(1<<PINF1) & ~(1<<PINF2) & ~(1<<PINF3) & ~(1<<PINF4);
	PORTF = 0x00;
	
	
	
	sprintf(buffer,"System On! \r\n");
	uart1.TxString(buffer);
	
	//DDRG = 0x1f;
	//PORTG = 0x1f;
	
	
	while (1) 
    {
		
		val = PINF & (~(1<<PINF7) & ~(1<<PINF6) & ~(1<<PINF5));
		

		if( val == 0x04 )
			while((PINF & (~(1<<PINF7) & ~(1<<PINF6) & ~(1<<PINF5)))== 0x04)
			{
				mymotor.motor_forward(30);
			}
			
		else if(val ==0x00)
		{
			
			mymotor.motor_break();
			_delay_ms(10);
			while((PINF & (~(1<<PINF7) & ~(1<<PINF6) & ~(1<<PINF5))) == 0x00)
			{
			mymotor.motor_wheel_forward(left,40);
			mymotor.motor_wheel_backward(right,20);	
			}
			
		}		
		
		else if ( val == 0x80)
		{
			while((PINF & (~(1<<PINF7) & ~(1<<PINF6) & ~(1<<PINF5)))== 0x80)
			{
			mymotor.motor_wheel_forward(right,35);
			mymotor.motor_wheel_forward(left,10);	
			}
		}
		else if (val == 0x40)
		{
			while((PINF & (~(1<<PINF7) & ~(1<<PINF6) & ~(1<<PINF5)))== 0x40)
			{
				mymotor.motor_wheel_forward(left,35);
				mymotor.motor_wheel_forward(right,10);
			}
		}
		else if (val == 0x02)
		{
			while((PINF & (~(1<<PINF7) & ~(1<<PINF6) & ~(1<<PINF5)))== 0x02)
			{
				mymotor.motor_wheel_forward(left,50);
				mymotor.motor_wheel_forward(right,10);
			}
		}
		else if (val == 0x08)
		{
			while((PINF & (~(1<<PINF7) & ~(1<<PINF6) & ~(1<<PINF5)))== 0x08)
			{
				mymotor.motor_wheel_forward(right,50);
				mymotor.motor_wheel_forward(left,10);
			}
		}
		else if (val == 0x07)
		{
			while((PINF & (~(1<<PINF7) & ~(1<<PINF6) & ~(1<<PINF5)))== 0x07)
			{
				mymotor.motor_wheel_forward(left,50);
				mymotor.motor_wheel_backward(right,1);
				
			}
		}

		
		/******
		 if(val&0b00000010) //오른쪽 중간 탐색
		 {
		
			 mymotor.motor_wheel_forward(left, 40);
			 mymotor.motor_wheel_forward(right, 20);
			 
		 }
		 else if(val&0b00011100) //왼쪽 3개 탐색
		 {
		
			 mymotor.motor_wheel_backward(left, 0);
			 mymotor.motor_wheel_forward(right, 40);
		 }
		 else if(val&0b00001100) // 왼쪽중간, 중간 탐색
		 {
			
			 mymotor.motor_wheel_forward(left, 20);
			 mymotor.motor_wheel_forward(right, 40);
		 }
		 else if(val&0b00000100)  // 중간 탐색
		 {
			
			 mymotor.motor_forward(25);
		 }
		 else if(val&0b00011000) // 왼쪽 2개 탐색
		 {
			
			 mymotor.motor_wheel_forward(left, 20);
			 mymotor.motor_wheel_forward(right, 45);
		 }
		 else if(val&0b00001000) // 왼쪽 중간탐색
		 {
			
			 mymotor.motor_wheel_forward(left, 20);
			 mymotor.motor_wheel_forward(right, 40);
		 }
		 else if(val&0b00010000) // 왼쪽 가장자리 탐색
		 {
		
			 mymotor.motor_wheel_backward(left, 0);
			 mymotor.motor_wheel_forward(right, 45);
		 }
		 else if((val & 0b00000000)==0) // 아무것도 탐색되지 않음
		 {
			
			mymotor.motor_break();
			_delay_ms(30);
			while((PINF & (~(1<<PINF7) & ~(1<<PINF6) & ~(1<<PINF5))) == 0b00000000)
			{
			 mymotor.motor_wheel_forward(right, 30);
			 mymotor.motor_wheel_backward(left, 40);
				if((PINF & (~(1<<PINF7) & ~(1<<PINF6) & ~(1<<PINF5))) != 0b00000000)
				{
				break;
				}
			}
			
		 }
		else if((val & 0b00001111)) //오른쪽 4개 탐색
		{
	
			mymotor.motor_wheel_forward(left,30);
			mymotor.motor_wheel_backward(right,0);
		}
		else if((val & 0b00001110)) //오른쪽 3개 탐색
		{
		
			mymotor.motor_wheel_forward(left,40);
			mymotor.motor_wheel_backward(right,0);
		}
		else if((val & 0b00000011)) //오른쪽 2개 탐색
		{
			
			mymotor.motor_wheel_forward(left,50);
			mymotor.motor_wheel_backward(right, 40);
		} 
		else if((val & 0b00000001)) // 오른쪽 1개 탐색
		{
			mymotor.motor_wheel_forward(left,60);
			mymotor.motor_wheel_backward(right,45);
			
			
		}
		else if((val & 0b0000110))  // 중간 오른쪽 탐색
		{
		mymotor.motor_wheel_forward(left, 50);
		mymotor.motor_wheel_forward(right, 25);
		}
		else if((val & 0b00011110)) // 왼쪽 4개 탐색
		{
			mymotor.motor_wheel_backward(left,0);
			mymotor.motor_wheel_forward(right,40);
		}
		else if((val & 0b00001110)) // 가운데 3개 탐색
		{
			
			mymotor.motor_forward(35);
		}
		else if((val & 0b00011000)) //왼쪽 2개 탐색
		{
			
			mymotor.motor_wheel_backward(left,0);
			mymotor.motor_wheel_forward(right,80);
		}
		else if ((val & 0x0A)) // 왼쪽 중간, 오른쪽 중간 탐색
		{
			mymotor.motor_wheel_forward(right,30);
			mymotor.motor_wheel_backward(left,0);
		}
		*//////
	
		
		



		
		
	
		/*************
		//myray.ray();//적외선
		distance = mysonic.measure_distance();
		//mymotor.motor_break();
		//_delay_ms(4000);
		sprintf(buffer,"distance : %d \r\n",distance);
		myuart.TxString(buffer);
		
		if(distance < 20 ){
			mymotor.motor_break();
			//distance = mysonic.measure_distance();
		}
		else mymotor.motor_forward(20);
	*/
	
	/*
	mymotor.motor_wheel_forward(right,40);
	_delay_ms(2000);
	mymotor.motor_break();
	_delay_ms(2000);
	mymotor.motor_wheel_forward(left,40);
	_delay_ms(2000);
	mymotor.motor_break();
	_delay_ms(2000);
	mymotor.motor_backward(40);
	_delay_ms(3000);
	mymotor.motor_break();
	_delay_ms(1000);
	mymotor.motor_wheel_backward(right,30);
	_delay_ms(2000);
	mymotor.motor_break();
	_delay_ms(2000);
	mymotor.motor_wheel_backward(left,30);
	_delay_ms(2000);
	mymotor.motor_break();
	_delay_ms(2000);
		
		*/
	
    }
}

