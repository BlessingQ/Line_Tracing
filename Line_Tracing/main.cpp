
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


volatile uint16_t flag = 0;
volatile uint16_t distance;
volatile uint16_t timer_cnt;
char buffer[50];
unsigned int val , right_curve = 0 , left_curve = 0;
UART uart0 = UART(0,BAUD_9600); //USB UART
UART uart1 = UART(1,BAUD_9600); //BlueTooth UART
motor_driver mymotor;
ultra_sonic mysonic;
BAT_ADC bat = BAT_ADC(5);
int bat_status = 0;

typedef struct state_s {
	uint8_t state;
	uint8_t flag;
	uint16_t timer, period, delay;
	int32_t (*cbf)(void *, int32_t);
} STATE_T;

int32_t dummy(void *p0, int32_t p1)
{
	return 0;	
}

STATE_T sts_uart0 = { 0, 0, 100, 100, 0, dummy };
STATE_T	sts_uart1 = { 0, 0, 1000, 1000, 0, dummy };

ISR(TIMER0_COMP_vect)
{
	flag++;
	sts_uart0.cbf(&sts_uart0, 0);
}

int32_t func_cbf(void *p0, int32_t p1)
{
	STATE_T *pS = (STATE_T *)p0;
	//int32_t *pint = (int32_t *)p0;
	
	if (p1 == 0) {
		pS->timer--;
		//sts_uart0.timer %= sts_uart0.period;

		if (pS->timer == 0) {
			pS->flag = 1;
			pS->timer = pS->period;
			//uart1.TxString("*");
		}

		if (pS->delay != 0) {
			pS->delay--;
		} else {
			// if (sts_uart0.cbf != NULL)
			pS->cbf((void *)pS, 1);
		}
	} else if (p1 == 1)	{	
		pS->state = 0;
	} 
}

// int32_t a = 100;
// sts_uart1.cbf((void *)&a, 1);
//
void uart1_proc(void)
{
	volatile STATE_T *pS = &sts_uart0;
	
	if (pS->flag == 1) {
		pS->flag = 0;
		//uart1.TxString("*");
		
		if (pS->state == 0) {
			bat_status = bat.Bat_status();
			sprintf(buffer,"BAT:%d\r\n",bat_status);  //배터리 전압 UART1번으로 전송
			uart1.TxString(buffer);
			sprintf(buffer,"Val : 0x%02X\r\n",val);
			uart1.TxString(buffer);
			pS->state = 1;
			pS->delay = 1000;

		} //else if (pS->state == 1) {

		//} else {
			//void(*);
		//}
	}
}

//void uart1_proc(void)
//{
	//volatile STATE_T *pS = &sts_uart0;
	//
	//if (pS->flag == 1) {
		//pS->flag = 0;
		////uart1.TxString("*");
			//
		//if (pS->state == 0) {
			//bat_status = bat.Bat_status();
			//sprintf(buffer,"BAT:%d\r\n",bat_status);  //배터리 전압 UART1번으로 전송
			//uart1.TxString(buffer);
			//pS->state = 1;
			//pS->delay = 10;
//
			//} else if (pS->state == 1) {
			//pS->delay--;
			//if (pS->delay == 0) pS->state = 0;
			//} else {
			////void(*);
		//}
	//}
//}

int vin_main(void)
{
	volatile STATE_T *pS;
	
	pS= &sts_uart0;
	pS->cbf = func_cbf;
	
	while (1) {
		uart1_proc();
		
	}
	
}



int main(void)
{
	
	volatile STATE_T *pS;
	
	pS= &sts_uart0;
	pS->cbf = func_cbf;
	
	
	
	DDRF &= ~(1<<PINF0) & ~(1<<PINF1) & ~(1<<PINF2) & ~(1<<PINF3) & ~(1<<PINF4);
	PORTF = 0x00;
	
	
	
	sprintf(buffer,"System On! \r\n");
	uart1.TxString(buffer);
	
	//DDRG = 0x1f;
	//PORTG = 0x1f;
	
	
	while (1) 
    {
		uart1_proc();
		
		val = PINF & (~(1<<PINF7) & ~(1<<PINF6) & ~(1<<PINF5));
		

		if( val == 0x04 )
			while((PINF & (~(1<<PINF7) & ~(1<<PINF6) & ~(1<<PINF5)))== 0x04)
			{
				mymotor.motor_forward(40);
				sprintf(buffer,"left30, right30 / val : 0x04\r\n");
				uart1.TxString(buffer);
			}
			
		else if(val ==0x00)
		{
			
			//mymotor.motor_break();
			//_delay_ms(30);
			while((PINF & (~(1<<PINF7) & ~(1<<PINF6) & ~(1<<PINF5))) == 0x00)
			{
			mymotor.motor_wheel_forward(left,60);
			mymotor.motor_wheel_backward(right,50);	
			sprintf(buffer,"left50, right40 / val : 0x00\r\n");
			uart1.TxString(buffer);
			}
			
		}		
		
		else if ( val == 0x80)
		{
			while((PINF & (~(1<<PINF7) & ~(1<<PINF6) & ~(1<<PINF5)))== 0x80)
			{
			mymotor.motor_wheel_forward(right,45);
			mymotor.motor_wheel_forward(left,30);	
			sprintf(buffer,"left10, right35 / val : 0x80\r\n");
			uart1.TxString(buffer);
			}
		}
		else if (val == 0x40)
		{
			while((PINF & (~(1<<PINF7) & ~(1<<PINF6) & ~(1<<PINF5)))== 0x40)
			{
				mymotor.motor_wheel_forward(left,45);
				mymotor.motor_wheel_forward(right,30);
				sprintf(buffer,"left35, right10 / val : 0x40\r\n");
				uart1.TxString(buffer);
			}
		}
		else if (val == 0x02)
		{
			while((PINF & (~(1<<PINF7) & ~(1<<PINF6) & ~(1<<PINF5)))== 0x02)
			{
				mymotor.motor_wheel_forward(left,35);
				mymotor.motor_wheel_forward(right,25);
				sprintf(buffer,"left30, right10 / val : 0x02\r\n");
				uart1.TxString(buffer);
			}
		}
		else if (val == 0x08)
		{
			while((PINF & (~(1<<PINF7) & ~(1<<PINF6) & ~(1<<PINF5)))== 0x08)
			{
				mymotor.motor_wheel_forward(right,40);
				mymotor.motor_wheel_forward(left,30);
				sprintf(buffer,"left10, right30 / val : 0x08\r\n");
				uart1.TxString(buffer);
			}
		}
		else if (val == 0x07)
		{
			while((PINF & (~(1<<PINF7) & ~(1<<PINF6) & ~(1<<PINF5)))== 0x07)
			{
				mymotor.motor_wheel_forward(left,60);
				mymotor.motor_wheel_backward(right,1);
				sprintf(buffer,"left50, right20 / val : 0x07\r\n");
				uart1.TxString(buffer);
				
			}
		}
		else if(val == 0x01)
		{
			mymotor.motor_wheel_forward(left,60);
			mymotor.motor_wheel_backward(right,30);
			sprintf(buffer,"left50, right-20 / val : 0x01\r\n");
			uart1.TxString(buffer);
		}
		else if(val == 0x10)
		{
			mymotor.motor_wheel_forward(right,60);
			mymotor.motor_wheel_backward(left,30);
			sprintf(buffer,"left-20, right50 / val : 0x10\r\n");
			uart1.TxString(buffer);
		}

		else if(val == 0x0e)
		{
			mymotor.motor_wheel_forward(left,60);
			mymotor.motor_wheel_backward(right,30);
			sprintf(buffer,"left50, right-20 / val : 0x0E\r\n");
			uart1.TxString(buffer);
		}
		else if (val == 0x1e)
		{
			mymotor.motor_wheel_forward(left,60);
			mymotor.motor_wheel_backward(right,30);
			sprintf(buffer,"left50, right-20 / val : 0x1E\r\n");
			uart1.TxString(buffer);
		}
		else if (val == 0x0F)
		{
			mymotor.motor_wheel_forward(left,70);
			mymotor.motor_wheel_backward(right,30);
			sprintf(buffer,"left50, right-20 / val : 0x0F\r\n");
			uart1.TxString(buffer);
			
		}
		else if (val == 0x03) // 오른쪽 2개 탐색
		{
			mymotor.motor_wheel_forward(left,60);
			mymotor.motor_wheel_forward(right,35);
			sprintf(buffer,"left50, right20 / val : 0x0F\r\n");
			uart1.TxString(buffer);
		}
		
		
		
		
		/*
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
			mymotor.motor_wheel_backward(right, 0);
		} 
		else if((val & 0b00000001)) // 오른쪽 1개 탐색
		{
			mymotor.motor_wheel_forward(left,60);
			mymotor.motor_wheel_backward(right,0);
			
			
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
			mymotor.motor_wheel_forward(right,50);
		}
		else if ((val & 0x0A)) // 왼쪽 중간, 오른쪽 중간 탐색
		{
			mymotor.motor_wheel_forward(right,30);
			mymotor.motor_wheel_backward(left,0);
		}
	
		*/
		



		
		
	
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
	mymotor.motor_wheel_forward(left,50);
	_delay_ms(2000);
	mymotor.motor_break();
	_delay_ms(2000);
	mymotor.motor_break();
	_delay_ms(1000);
	mymotor.motor_wheel_backward(right,30);
	_delay_ms(2000);
	mymotor.motor_break();
	_delay_ms(2000);
	mymotor.motor_wheel_backward(left,50);
	_delay_ms(2000);
	mymotor.motor_break();
	_delay_ms(2000);
		
		*/
	
    }
}

