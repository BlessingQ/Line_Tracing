/*
 * Bat_adc.cpp
 *
 * Created: 2018-06-15 오전 11:40:23
 *  Author: stc_168
 */ 


#include "Bat_adc.h"


BAT_ADC::BAT_ADC(uint8_t sec)
{
	ISR_Sec = sec; 
	
	ADMUX |= (1<<REFS0) | (1<<MUX2) |(1<<MUX1) |(1<<MUX0);   //ADC PF7번 하나 사용
	ADCSRA |= (1<<ADEN) | (1<<ADSC) | (1<<ADFR) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);  
	
	
	TCCR0 |= 0x0c; //(1<<CS02) | (1<<CS01);
	TIMSK |= (1<<TOIE0) |(1<<OCIE0);
	OCR0 = 249;
	
	sei();
}

int BAT_ADC::Bat_status(void)
{

bat_val = ADCW;             // 배터리 전압 읽어들임
f=(float)bat_val*5.0/1023.0;
vin = f / (4700.0/ (22000.0+4700.0) );

return vin;
}