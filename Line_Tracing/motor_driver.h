﻿/*
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

#define DIR_PORT_DDR DDRG
#define DIR_PORT PORTG
#define IN1_R_F PORTG0
#define IN2_R_B PORTG1
#define IN3_L_F PORTG2
#define IN4_L_B PORTG3
#define PWM_PORT DDRB
#define M_PWM1_PORT DDRB5
#define M_PWM2_PORT DDRB6 
#define right 1
#define left 2

class motor_driver
{
public:
	motor_driver(){
		motor_init();
		};
	~motor_driver(){};

void motor_forward(uint8_t speed);
void motor_backward(uint8_t speed);
void motor_wheel_forward(uint8_t wheel,uint8_t speed);
void motor_wheel_backward(uint8_t wheel,uint8_t speed);
void motor_break(void);

protected:
	
void motor_init(void);


private:



};




#endif /* MOTOR_DRIVER_H_ */