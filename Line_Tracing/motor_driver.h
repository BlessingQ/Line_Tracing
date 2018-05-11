/*
 * motor_driver.h
 *
 * Created: 2018-05-09 오후 7:17:46
 *  Author: stc_168
 */ 


#ifndef MOTOR_DRIVER_H_
#define MOTOR_DRIVER_H_


#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define DRI_PORT_DDR DDRE
#define DRI_PORT PORTE
#define IN1 PORTE0
#define IN2 PORTE1
#define IN3 PORTE2
#define IN4 PORTE3
#define PWM_PORT DDRB
#define M_PWM1_PORT DDRB5
#define M_PWM2_PORT DDRB6 


class motor_driver
{
public:
	motor_driver(){
		motor_init();
		};
	~motor_driver(){};

void motor_forward(void);
void motor_break(void);

protected:
	
void motor_init(void);


private:



};




#endif /* MOTOR_DRIVER_H_ */