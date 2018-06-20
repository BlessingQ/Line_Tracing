/*
 * Bat_adc.h
 *
 * Created: 2018-06-15 오전 11:40:38
 *  Author: stc_168
 */ 


#ifndef BAT_ADC_H_
#define BAT_ADC_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


class BAT_ADC
{
public:


	BAT_ADC(uint8_t sec);

	~BAT_ADC(){};
	
volatile uint16_t cnt = 0;	
volatile uint8_t ISR_Sec;
int bat_val;
float f,vin;	

int Bat_status(void);
		
protected:
	
	
private:


void BAT_ADC_Init(uint8_t sec);



};









#endif /* BAT_ADC_H_ */