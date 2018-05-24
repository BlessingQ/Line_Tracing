
#ifndef REDRAY_H_
#define REDRAY_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//#include <stdio.h>

class redray
{
public:
	redray(){
		ray_init();
	};
	~redray(){};
	void ray(void);
protected:
	void ray_init(void);
};


#endif /* REDRAY_H_ */