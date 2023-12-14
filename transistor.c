#include "LED.h"

// PA5  <--> Green LED
#define trans_pin    5

// PA5  <--> Water Pump
// Pin PA5 outputs a sufficient voltage to the base of the BJT, switching the transistor and pump on and off

void configure_trans_pin(){
  // 1. Enable the clock to GPIO Port A	
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;   
		
	// 2. Configure GPIO Mode to 'Output': Input(00), Output(01), AlterFunc(10), Analog(11)
	GPIOA->MODER &= ~(3UL<<(2*trans_pin));  
	GPIOA->MODER |=   1UL<<(2*trans_pin);      // Output(01)

	// 3. Configure GPIO Output Type to 'Push-Pull': Output push-pull (0), Output open drain (1) 
	GPIOA->OTYPER &= ~(1<<trans_pin);      // Push-pull
	
	// 4. Configure GPIO Push-Pull to 'No Pull-up or Pull-down': No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR  &= ~(3<<(2*trans_pin));  // No pull-up, no pull-down
}


// Modular function to turn on the transistor.
void turn_on_trans(){
	GPIOA->ODR |= (1 << trans_pin);
}

// Modular function to turn off the transistor.
void turn_off_trans(){
	GPIOA->ODR &= ~(1 << trans_pin);
}

// Modular function to toggle the transistor.
void toggle_trans(){
	GPIOA->ODR ^= (1 << trans_pin);
}


