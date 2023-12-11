include "stm32l476xx.h"

// contains declarations of functions across multiple files 
// so that functions can be compactly utilized in main.c

void configure_button();
void cconfigure_EXTI13(void);
void EXTI15_10_IRQHandler(void);