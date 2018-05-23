/*
 * UART.cpp
 *
 * Created: 2018-04-21 오후 5:23:45
 *  Author: 6층교수실PC
 */ 
#include "UART.h"

UART::UART(uint8_t uart_sel, uint8_t baud)
{
	uint8_t baud_rate = baud;
	
	uart_select = uart_sel;
	
	if (uart_select == 0)
	{
		PORTE |= (1<<PORTE1) | (1<<PORTE0);
		DDRE &= ~(1<<PORTE0); // RXD0
		DDRE |= (1<<PORTE1);  // TXD0
		
		UBRR0L = baud_rate;
		
		UCSR0A = 0x00;
		UCSR0B |= (1<<RXEN0) | (1<<TXEN0);  // TXD, RXD Enable
		UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);  // 8bit data
	}
	else if (uart_select == 1)
	{
		PORTD |= (1 << PORTD3) | (1<<PORTD2);
		DDRD &= ~(1 << DDRD2); // RXD1
		DDRD |= (1 << DDRD3);  // TXD1

		UBRR1L = baud_rate;
		
		UCSR1A = 0x00;
		UCSR1B |= (1<<RXEN1) | (1<<TXEN1); // TXD, RXD Enable
		UCSR1C |= (1<<UCSZ11) | (1<<UCSZ10); // 8bit data
	}
}

UART::UART(uint8_t uart_sel)
{
	uint8_t baud_rate = BAUD_9600; // 기본값
	
	uart_select = uart_sel;
	
	if (uart_select == 0)
	{
		UBRR0L = baud_rate;
		
		UCSR0A = 0x00;
		UCSR0B |= (1<<RXEN0) | (1<<TXEN0); // 0x18;
		UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00); // 0x06;
	}
	else if (uart_select == 1)
	{
		UBRR1L = baud_rate;
		
		UCSR1A = 0x00;
		UCSR1B |= (1<<RXEN1) | (1<<TXEN1); // 0x18;
		UCSR1C |= (1<<UCSZ11) | (1<<UCSZ10); // 0x06;
	}	
}

	
uint8_t UART::IsRxD(void)
{
	if (uart_select == 0)
		return ( UCSR0A & (1<<RXC0) );
	else if (uart_select == 1)
		return ( UCSR1A & (1<<RXC1) );
}

uint8_t UART::IsTxReady(void)
{
	if (uart_select == 0)
		return ( UCSR0A & (1<<UDRE0) );
	else if (uart_select == 1)
		return ( UCSR1A & (1<<UDRE1) );
}

void UART::TxChar(char ch)
{
	while ( !IsTxReady() );
	
	if (uart_select == 0)
	{
		UDR0 = ch;
		UCSR0A |= 0x20;
	}
	else if (uart_select == 1)
	{
		UDR1 = ch;
		UCSR1A |= 0x20;
	}
}

char UART::RxChar(void)
{
	while ( !IsRxD() ) ;
	if (uart_select == 0)
		return UDR0;
	else if (uart_select == 1)
		return UDR1;
}

void UART::TxString(char *str)
{
	int i = 0;
	while(str[i] != 0)
	{
		TxChar(str[i]);
		i++;
	}
}

void UART::RxString(char *RxD_Data)
{
	uint8_t RxD_Flag = 1, index = 0;
	while (RxD_Flag)
	{
		RxD_Data[index] = RxChar();

		if ( RxD_Data[index] == '\r')
		{
			RxD_Data[index] = 0;
			RxD_Flag = 0;
		}
		else if ( RxD_Data[index] == '\n')
		{
			RxD_Data[index] = 0;
			RxD_Flag = 0;
		}

		index++;
	}
	//index = 0;
}