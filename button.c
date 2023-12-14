#include "Systick_timer.h"


volatile counter; // Generate a counter 
	
void configure_button(){
		// Enable clock to GPIO C
		RCC->AHB2ENR |= (1U << 2);
	
		// Configure GPIO mode to input (00)
		GPIOC->MODER &= ~(3U << 2*13);
	
		// Configure GPIO Output to push-pull (0)
		GPIOC->OTYPER &= ~(1U << 13);
		
		// Configure GPIO push-pull to no pull-up and pull-down (00)
		GPIOC->PUPDR &= ~(3U << 2*13);
	}
	
void configure_EXTI13(void){
	
  NVIC_EnableIRQ(EXTI15_10_IRQn); //1. Enable the EXTI15_10 interrupt (including EXTI13)
 
  //Configure the SYSCFG module to link EXTI line 13 to GPIO PC13
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;            // Enable the clock to SYSCFG
  SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;     // Clear the EXTI13 bits in SYSCFG's EXTICR4 register (4th register containing EXT13).
  SYSCFG->EXTICR[3] |=  SYSCFG_EXTICR4_EXTI13_PC;  // Set PC13 (0010) as the EXTI13 source in SYSCFG_EXTICR3.


  //Enable / Unmask the EXTI13 interrupt by setting its corresponding bit in the EXTI IMR.
  EXTI->IMR1 |= (1U << 13);   //Interrupt Mask Register 0 = marked, 1 = not masked (i.e., enabled)
 
  //Enable interrupt trigger for both rising (button release) and falling (button press) edges.
 // EXTI->RTSR1 |= (1 << 13);  //Rising trigger selection register 0 = disabled, 1 = enabled
  EXTI->FTSR1 |= (1 << 13);  //Falling trigger selection register 0 = disabled, 1 = enabled
}

// counter and frequency note function

void EXTI15_10_IRQHandler(void) {  
	
  // PR (Pending Register): Check if the interrupt is triggered by EXTI13, EXTI13 shares this interrupt vector.
  if ((EXTI->PR1 & EXTI_PR1_PIF13) == EXTI_PR1_PIF13) {
		
    // cleared by writing a 1 to the bit
    EXTI->PR1 |= EXTI_PR1_PIF13;
    
		// increases counter value with each button press
		counter++;
		
		// counter determines intensity of water pouring, maybe have 3 modes
		if(counter % 3 == 1){ 
			SysTick_Init(2000000/262); // C4 262 Hz
    } else if(counter % 3 == 2){
      SysTick_Init(2000000/294); // D4 294 Hz
		} else {
			SysTick_Init(2000000/494); // B4 494 Hz
		}
	}
}
	

