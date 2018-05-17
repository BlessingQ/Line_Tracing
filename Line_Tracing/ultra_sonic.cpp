/*
 * ultrasonic.cpp
 *
 * Created: 2018-05-09 오후 7:00:07
 *  Author: stc_168
 */ 

#include "ultra_sonic.h"

#define ultra_sonic_ddr DDRE
#define trigger_pin_ddr DDRE6
#define echo_pin_ddr DDRE7
#define ultra_sonic_port PORTE
#define trigger_port PORTE6
#define echo_port PORTE7
#define ultra_sonic_pin PINE
#define echo_pin PINE7




void ultra_sonic::ultra_sonic_init(void){
	ultra_sonic_ddr |= (1<< trigger_pin_ddr);
	ultra_sonic_ddr &= ~(1<<echo_pin_ddr);
	TCCR3B |= (1<<CS32) | (1<<CS30); //분주비 1024
}



uint8_t ultra_sonic::measure_distance(void)
{
	ultra_sonic_port &= ~(1 << trigger_port);
	_delay_us(1);
	ultra_sonic_port |= (1 << trigger_port);
	_delay_us(10);
	ultra_sonic_port &= ~(1 << trigger_port);
	
	TCNT3 = 0;
	while( !(ultra_sonic_pin & ( 1 << echo_pin) ) )
	if(TCNT1 > 65000) return 0;
	
	TCNT3 = 0;
	while (ultra_sonic_pin & ( 1 << echo_pin) )
	{
		if (TCNT3 > 65000)
		{
			TCNT3 = 0;
			break;
		}
	}
	
	double pulse_width = 1000000.0 * TCNT3 * PRESCALER / F_CPU;
	
	return pulse_width / 58;
}