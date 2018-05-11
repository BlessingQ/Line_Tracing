/*
 * motor_driver.cpp
 *
 * Created: 2018-05-09 오후 7:18:00
 *  Author: stc_168
 */ 

#include "motor_driver.h"


void motor_driver::motor_init(void){
	DRI_PORT_DDR |= (1<<IN1) | (1<<IN2)| (1<<IN3)| (1<<IN4);    //방향 포트 출력설정
	DRI_PORT &= ~(1<<IN1) | ~(1<<IN2)| ~(1<<IN3)| ~(1<<IN4);    //방향 포트 0으로 초기화
	PWM_PORT = (1<<M_PWM1_PORT) | (1<<M_PWM2_PORT);
	
	TCCR1A |= (1 << WGM11) | (1<<COM1A1)| (1<<COM1B1);       //FAST PWM
	TCCR1B |= (1 << CS11) | (1<<CS10) | (1<< WGM12) | (1<<WGM13);    //Prescaler 64
	
	ICR1 = 249;
}

void motor_driver::motor_forward(void){
	
	DRI_PORT |= (1<<IN1) | (1<<IN3);   //양바퀴 정회전
	DRI_PORT &=	~(1<<IN2) | ~(1<<IN4);
		
		OCR1A = 248;  //50% 속도
		OCR1B = 248;   //75% 속도
}

void motor_driver::motor_break(void){
	DRI_PORT &= ~(1<<IN1) & ~(1<<IN2) & ~(1<<IN3) & ~(1<<IN4);
}