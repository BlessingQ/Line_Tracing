/*
 * motor_driver.cpp
 *
 * Created: 2018-05-09 오후 7:18:00
 *  Author: stc_168
 */ 

#include "motor_driver.h"




void motor_driver::motor_init(void){
	DIR_PORT_DDR |= (1<<IN1_R_F) | (1<<IN2_R_B)| (1<<IN3_L_F)| (1<<IN4_L_B);    //모터 방향 포트 출력설정
	DIR_PORT &= ~(1<<IN1_R_F) & ~(1<<IN2_R_B) & ~(1<<IN3_L_F) & ~(1<<IN4_L_B);    //모터 방향 포트 0으로 초기화
	PWM_PORT = (1<<M_PWM1_PORT) | (1<<M_PWM2_PORT);				//PWM PORT PB5 , PB6 출력설정
	
	TCCR1A |= (1 << WGM11) | (1<<COM1A1)| (1<<COM1B1);       //FAST PWM
	//TCCR1B |= (1 << CS11) | (1<<CS10) | (1<< WGM12) | (1<<WGM13);    //Prescaler 64 
	TCCR1B |= (1 << CS11) | (1<< WGM12) | (1<<WGM13);    //Prescaler 8 
	
	ICR1 = 200;   //10Khz 설정
}

void motor_driver::motor_forward(uint8_t speed){
	
	
	
	DIR_PORT |= (1<<IN1_R_F) | (1<<IN3_L_F);   //양바퀴 정회전
	DIR_PORT &=	~(1<<IN2_R_B) & ~(1<<IN4_L_B);
	
	//OCR1A = 249;  // 속도
	//OCR1B = 249;  // 속도
	
	speed = (speed * 2);	
	OCR1A = (uint8_t)speed;  // 속도
	OCR1B = (uint8_t)speed;  // 속도
}
void motor_driver::motor_backward(uint8_t speed)
{
	
	
	DIR_PORT |= (1<<IN2_R_B) | (1<<IN4_L_B);   //양바퀴 정회전
	DIR_PORT &=	~(1<<IN1_R_F) & ~(1<<IN3_L_F);
	
	//OCR1A = 249;  // 속도
	//OCR1B = 249;  // 속도
	
	speed = (speed * 2);
	OCR1A = (uint8_t)speed;  // 속도
	OCR1B = (uint8_t)speed;  // 속도
}
void motor_driver::motor_wheel_forward(uint8_t wheel,uint8_t speed){
	
	if(wheel == 1){
		
		DIR_PORT |= (1<<IN1_R_F);
		DIR_PORT &= ~(1<<IN2_R_B);
		
		//OCR1A = 249;  // 속도
		
		speed = (speed * 2);
		OCR1A = (uint8_t)speed;  // 속도
		//OCR1B = speed;  // 속도
	}
	if(wheel == 2){
		
		DIR_PORT |= (1<<IN3_L_F);
		DIR_PORT &= ~(1<<IN4_L_B);
		
		//OCR1B = 249;  // 속도
		
		speed = (speed * 2);
		//OCR1A = speed;  // 속도
		OCR1B = (uint8_t)speed;  // 속도
	}
}
void motor_driver::motor_wheel_backward(uint8_t wheel,uint8_t speed){
	
	if(wheel == 1){
		DIR_PORT |= (1<<IN2_R_B);
		DIR_PORT &= ~(1<<IN1_R_F);
		
		speed = (speed * 2);
		OCR1A = (uint8_t)speed;  // 속도
		
	}
	if(wheel == 2){
		DIR_PORT |= (1<<IN4_L_B);
		DIR_PORT &= ~(1<<IN3_L_F);
		
		speed = (speed * 2);
		OCR1B = (uint8_t)speed;  // 속도
	}
}


void motor_driver::motor_break(void){
	
	OCR1A = 1;
	OCR1B = 1;
	DIR_PORT &= ~(1<<IN1_R_F) & ~(1<<IN2_R_B) & ~(1<<IN3_L_F) & ~(1<<IN4_L_B);
	
}


