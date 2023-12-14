#include "LED.h"
#include "Systick_timer.h"
#include "button_interrupt.h"

<<<<<<< HEAD
// 11/27/23 marco is still a stinky poo

//gonna kms
=======

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
>>>>>>> bf9456bf0fe444b5ac795c10e38ec069ccc35a51
