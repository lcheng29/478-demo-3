#include "stm32l476xx.h"
#include "ADC.h"
#define PUMP_PIN    5
#define	Button_PIN	13
#define EXTI_PIN Button_PIN
volatile unsigned long counter = 0;
// PA5  <--> Water Pump

void configure_PUMP_pin(){
  // 1. Enable the clock to GPIO Port A	
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;   
		
	// 2. Configure GPIO Mode to 'Output': Input(00), Output(01), AlterFunc(10), Analog(11)
	GPIOA->MODER &= ~(3UL<<(2*PUMP_PIN));  
	GPIOA->MODER |=   1UL<<(2*PUMP_PIN);      // Output(01)

	// 3. Configure GPIO Output Type to 'Push-Pull': Output push-pull (0), Output open drain (1) 
	GPIOA->OTYPER &= ~(1<<PUMP_PIN);      // Push-pull
	
	// 4. Configure GPIO Push-Pull to 'No Pull-up or Pull-down': No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR  &= ~(3<<(2*PUMP_PIN));  // No pull-up, no pull-down
}

void configure_Push_Button_pin(void){
  // 1. Enable the clock to GPIO Port A	
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;   
		
	// 2. Configure GPIO Mode to 'Input': Input(00), Output(01), AlterFunc(10), Analog(11)
	GPIOC->MODER &= ~(3UL<<(2*Button_PIN));  	// Input (00)
	
	// 3. Configure GPIO Push-Pull to 'No Pull-up or Pull-down': No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOC->PUPDR  &= ~(3<<(2*Button_PIN));  // No pull-up, no pull-down
}

void configure_EXTI(void){

	//1. Enable the EXTI15_10 interrupt (including EXTI13) in NVIC using a function from CMSIS's core_cm4.h.
	NVIC_EnableIRQ(EXTI15_10_IRQn); 
	
	//2. Configure the SYSCFG module to link EXTI line 13 to GPIO PC13
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;							// Enable the clock to SYSCFG
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;     	// Clear the EXTI13 bits in SYSCFG's EXTICR4 register.
	SYSCFG->EXTICR[3] |=  SYSCFG_EXTICR4_EXTI13_PC; 	// Set PC13 (0010) as the EXTI13 source in SYSCFG_EXTICR4.

	// 3. Enable (unmask) the EXTI13 interrupt by setting its corresponding bit in the EXTI's IMR.
	EXTI->IMR1 |= (1<<EXTI_PIN);     //Interrupt Mask Register (IMR): 0 = marked, 1 = not masked (i.e., enabled)
	
	//4. Enable interrupt trigger for falling (button press) edge
	EXTI->RTSR1 |= (1<<EXTI_PIN);  //Rising trigger selection register (RTSR):0 = disabled, 1 = enabled
	EXTI->FTSR1 |= (1<<EXTI_PIN);  //Falling trigger selection register (FTSR): 0 = disabled, 1 = enabled
}


// Modular function to turn on the LD2 LED.
void turn_on_PUMP(){
	GPIOA->ODR |= (1 << PUMP_PIN);
}

// Modular function to turn off the LD2 LED.
void turn_off_PUMP(){
	GPIOA->ODR &= ~(1 << PUMP_PIN);
}

// Modular function to toggle the LD2 LED.
void toggle_PUMP(){
	GPIOA->ODR ^= (1 << PUMP_PIN);
}

// ISR (interrupt handler) for EXTI15_10. Interrupt handlers are initially defined in startup_stml476xx.s.
void EXTI15_10_IRQHandler(void) {  
	// PR (Pending Register): Check if the interrupt is triggered by EXTI13, as EXTI 10-15 share this interrupt vector.	
	if ((EXTI->PR1 & EXTI_PR1_PIF13) == EXTI_PR1_PIF13) {
		// cleared by writing a 1 to this bit
		EXTI->PR1 |= EXTI_PR1_PIF13;
		//toggle_LED();
		counter++;
	}
}





int main(void){

turn_on_PUMP();
configure_EXTI();	
configure_Push_Button_pin();
configure_PUMP_pin();

// Modular function to initialize ADC
	ADC_Init();
	
	// After initialization, begin ADC conversion with a software trigger by setting ADSTART bit in ADC1_CR.
	ADC1->CR |= ADC_CR_ADSTART;		
	
		while(1){

		}

}