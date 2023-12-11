#include "LED.h"
#include "Systick_timer.h"
#include "button_interrupt.h"


int main(void){
	// configure push-button, interrupt, and LED 
	configure_button();
	configure_EXTI13();
	configure_LED_pin();
	
	// turn on LED to verify configuration
	
	turn_on_LED();

	while(1){
		// background tasks
	}
}
