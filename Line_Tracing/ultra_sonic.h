/*
 * ultra_sonic.h
 *
 * Created: 2018-05-09 오후 7:00:46
 *  Author: stc_168
 */ 


#ifndef ULTRA_SONIC_H_
#define ULTRA_SONIC_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>



#define PRESCALER 1024




class ultra_sonic
{
public:
	ultra_sonic(){
		ultra_sonic();
		};
	~ultra_sonic(){};

uint8_t measure_distance(void);


protected:
	

void ultra_sonic_init(void);

private:



};








#endif /* ULTRA_SONIC_H_ */