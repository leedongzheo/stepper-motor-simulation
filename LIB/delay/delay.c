#include "delay.h"
#include "main.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//

volatile uint32_t counter_ms = 0;
volatile uint32_t counter_us = 0;

__IO uint32_t TimingMillis;
static void __empty() {
	// Empty
}
void abc(){
	counter_ms++;//1ms
	TimingMillis++;
}
void yield(void) __attribute__ ((weak, alias("__empty")));
/*
void SysTick_Handler2(void) {
	//neu ngat 1ms su dung 2 ham millis() v?micros()
  counter_ms++;//1ms
	TimingMillis++;
	//neu ngat 1us
	//counter_us++; counter_ms %= counter_us;//1us
}
*/
uint32_t millis(void) {
  return counter_ms;
}

uint32_t micros(void)//thong qua ms
{

int Micros = millis()*1000 + (SystemCoreClock/1000-SysTick->VAL)/72;
return Micros;

}
/*
void delay_init(void){
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	SysTick_Config(SystemCoreClock / 1000);	// ngat 1ms
	//SysTick_Config (SystemCoreClock / 1000000); //1us per interrupt
}
*/


void delayMs( uint32_t ms )
{
    uint32_t end = millis() + ms;
    while (millis() < end)
    	yield();
}			 


void delayUs(uint32_t us)
{
	uint32_t end = micros() + us;
    while (micros() < end);
	
}






































