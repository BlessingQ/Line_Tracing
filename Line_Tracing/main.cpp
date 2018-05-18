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

