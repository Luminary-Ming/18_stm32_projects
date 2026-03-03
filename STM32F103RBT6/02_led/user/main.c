#include "led.h"

int main(void)
{
	led_init();  // ≥ı ºªØ led

	led_on(0);
	led_on(1); 
	led_on(2);
	
	while(1);
}



