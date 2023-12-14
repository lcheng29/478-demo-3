#include "stm32l476xx.h"
#include "ADC.h"


#define LED_PIN    5
#define	Button_PIN	13
#define EXTI_PIN Button_PIN
volatile unsigned long counter = 0;

void configure_LED_pin(){
  // 1. Enable the clock to GPIO Port A	
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;   
		
	// 2. Configure GPIO Mode to 'Output': Input(00), Output(01), AlterFunc(10), Analog(11)
	GPIOA->MODER &= ~(3UL<<(2*LED_PIN));  
	GPIOA->MODER |=   1UL<<(2*LED_PIN);      // Output(01)

	// 3. Configure GPIO Output Type to 'Push-Pull': Output push-pull (0), Output open drain (1) 
	GPIOA->OTYPER &= ~(1<<LED_PIN);      // Push-pull
	
	// 4. Configure GPIO Push-Pull to 'No Pull-up or Pull-down': No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR  &= ~(3<<(2*LED_PIN));  // No pull-up, no pull-down
}

// Modular function to turn on the LD2 LED.
void turn_on_LED(){
	GPIOA->ODR |= 1 << LED_PIN;
}

// Modular function to turn off the LD2 LED.
void turn_off_LED(){
	GPIOA->ODR &= ~(1 << LED_PIN);
}

// Modular function to toggle the LD2 LED.
void toggle_LED(){
	GPIOA->ODR ^= (1 << LED_PIN);
}

int main(void){

	// Initialize ADC: Set up ADC1 for sampling from external input channel PA1 (ADC1_IN6). 
	// Configure for 12-bit resolution, right data alignment, single-ended, continuous mode, 
	// and interrupt at the end of every conversion.
	ADC_Init();
	
	// After initialization, begin ADC conversion with a software trigger by setting ADSTART bit in ADC1_CR.
	ADC1->CR |= ADC_CR_ADSTART;		
	
	while(1){
		if (adc_result > 3500){
		turn_on_LED();
	}
	else {
		if (adc_result < 1500)
		turn_off_LED();
	}
	return 0;
}

}