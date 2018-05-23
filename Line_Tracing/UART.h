/*
 * UART.h
 *
 * Created: 2018-04-21 오후 5:24:01
 *  Author: Rho Jinho
 */ 


#ifndef UART_H_
#define UART_H_

#define F_CPU	16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define BAUD_9600	103
#define BAUD_14400	68
#define BAUD_57600	16
#define BAUD_115200	8

class UART
{
private:
	uint8_t uart_select;
	uint8_t baudrate;
	
public:
	UART(uint8_t uart_sel, uint8_t baud);
	UART(uint8_t uart_sel);
	~UART(){};

	uint8_t IsRxD(void);
	void TxChar(char ch);
	char RxChar(void);
	void TxString(char *str);
	void RxString(char *RxD_Data);

protected:
	uint8_t IsTxReady(void);
};



#endif /* UART_H_ */