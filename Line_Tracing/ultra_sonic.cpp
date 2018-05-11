/*
 * ultrasonic.cpp
 *
 * Created: 2018-05-09 오후 7:00:07
 *  Author: stc_168
 */ 

#include "ultra_sonic.h"




void ultra_sonic::ultra_sonic_init(void){
	DDRE |= (1<< DDRE6);
	DDRE &= ~(1<<DDRE7);
	TCCR1B |= (1<<CS12) | (1 << CS10); //분주비 1024
}



uint8_t ultra_sonic::measure_distance(void)
{
	PORTE &= ~(1 << PORTE6);
	_delay_us(1);
	PORTE |= (1 << PORTE6);
	_delay_us(10);
	PORTE &= ~(1 << PORTE6);
	
	TCNT1 = 0;
	while( !(PINE & ( 1 << PINE7) ) )
	if(TCNT1 > 65000) return 0;
	
	TCNT1 = 0;
	while (PINE & ( 1 << PINE7 ) )
	{
		if (TCNT1 > 65000)
		{
			TCNT1 = 0;
			break;
		}
	}
	
	double pulse_width = 1000000.0 * TCNT1 * PRESCALER / F_CPU;
	
	return pulse_width / 58;
}