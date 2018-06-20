
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
	f=(float)ad_val * 5.0 / 1023.0;
	if (f>1.5)
	{
		
		PORTB &= ~(1<<IN1_R_F) & ~(1<<IN2_R_B) & ~(1<<IN3_L_F) & ~(1<<IN4_L_B);
		_delay_ms(100);
	}
	else
	
	PORTB |= (1<<IN1_R_F) | (1<<IN3_L_F);
}