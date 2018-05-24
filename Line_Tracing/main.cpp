
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "ultra_sonic.h"
#include "motor_driver.h"
#include "UART.h"
#include "redray.h"
redray myray; // 적외선

volatile uint16_t distance;

int main(void)
{
	motor_driver mymotor;
	ultra_sonic mysonic;
	UART myuart = UART(0,8);
	char buffer[20];
		
	myuart.TxString("System On! \r\n");
    while (1) 
    {
		myray.ray();//적외선
	distance = mysonic.measure_distance();
		//mymotor.motor_break();
		//_delay_ms(4000);
		sprintf(buffer,"distance : %d \r\n",distance);
		myuart.TxString(buffer);
		
		if(distance < 8 ){
			mymotor.motor_break();
			//distance = mysonic.measure_distance();
		}
		else mymotor.motor_forward(60);
		
		//mymotor.motor_break();
		//_delay_ms(2000);
		//mymotor.motor_wheel_forward(1,40);
		//_delay_ms(2000);
		//mymotor.motor_break();
		//_delay_ms(2000);
		//mymotor.motor_wheel_forward(2,40);
		//_delay_ms(2000);
		//mymotor.motor_break();
		//_delay_ms(2000);
		//mymotor.motor_backward(80);
		//_delay_ms(3000);
		//mymotor.motor_break();
		//_delay_ms(1000);
		////mymotor.motor_backward(70);
		////mymotor.motor_wheel_forward(1,50);
		////mymotor.motor_wheel_forward(2,50);
		////_delay_ms(2000);
		
    }
}

