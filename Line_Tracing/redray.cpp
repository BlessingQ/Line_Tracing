
#include "redray.h"

void redray::ray_init(void)
{
	DDRD=0xff;
	ADMUX=0x40;
	ADCSRA=0xe7;
}

void redray::ray(void)
{
	int ad_val;
	float f;
	
	_delay_ms(1);
	ad_val=ADCW;
	f=(float)ad_val*5.0/1023.0;
	if (f>1.5)
	{
		PORTD = 0xff;
		//PORTB &= ~(1<<IN1) & ~(1<<IN2) & ~(1<<IN3) & ~(1<<IN4);
		_delay_ms(100);
	}
	else
	PORTD = 0x00;
	//PORTB |= (1<<IN1) | (1<<IN3);
}