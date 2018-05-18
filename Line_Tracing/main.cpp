/*
 * Line_Tracing.cpp
 *
 * Created: 2018-05-09 오후 6:57:25
 * Author : stc_168
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "ultra_sonic.h"
#include "motor_driver.h"

/*w 적외선 감지
int main(void)
{
	int ad_val;
	float f;
	DDRD=0xff;
	ADMUX=0x40;
	ADCSRA=0xe7;
	
	while (1)
	{
		_delay_ms(1);
		ad_val=ADCW;
		f=(float)ad_val*5.0/1023.0;
		if (f>1.5)
		{
			PORTD=0xff;
			_delay_ms(100);
		}
		else
		PORTD=0x00;
	}
}
*/

int main(void)
{
	motor_driver mymotor;
		
    while (1) 
    {
		
		//mymotor.motor_break();
		//_delay_ms(4000);
		mymotor.motor_forward(30);
		_delay_ms(2000);
		mymotor.motor_break();
		_delay_ms(2000);
		mymotor.motor_break();
		_delay_ms(2000);
		mymotor.motor_wheel_forward(1,40);
		_delay_ms(2000);
		mymotor.motor_break();
		_delay_ms(2000);
		mymotor.motor_wheel_forward(2,40);
		_delay_ms(2000);
		//mymotor.motor_backward(70);
		//mymotor.motor_wheel_forward(1,50);
		//mymotor.motor_wheel_forward(2,50);
		//_delay_ms(2000);
		
		
    }
}

