// ***** 0. Documentation Section *****
// SwitchLEDInterface.c for Lab 8
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to toggle an LED
// while a button is pressed and turn the LED on when the
// button is released.  This lab requires external hardware
// to be wired to the LaunchPad using the prototyping board.
// January 15, 2016
//      Jon Valvano and Ramesh Yerraballi

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"
#define INPUT (1u)
#define LED (1u <<1) // shift bit 0x01 for Bit 1 

// ***** 2. Global Declarations Section *****

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void InitPortE(void); // initialize port f 
void LED_On(void); //turn LED On
void LED_Toggle(void); //toggle LED
void Delay1ms(unsigned long ms); // delay by ms [ms]

// ***** 3. Subroutines Section *****

// PE0, PB0, or PA2 connected to positive logic momentary switch using 10k ohm pull down resistor
// PE1, PB1, or PA3 connected to positive logic LED through 470 ohm current limiting resistor
// To avoid damaging your hardware, ensure that your circuits match the schematic
// shown in Lab8_artist.sch (PCB Artist schematic file) or 
// Lab8_artist.pdf (compatible with many various readers like Adobe Acrobat).
int main(void){ 
//**********************************************************************
// The following version tests input on PE0 and output on PE1
//**********************************************************************
  TExaS_Init(SW_PIN_PE0, LED_PIN_PE1, ScopeOn);  // activate grader and set system clock to 80 MHz
  InitPortE(); //1. Initialize Port 
  LED_On(); // 2. Turn LED On
  EnableInterrupts();           // enable interrupts for the grader
  while(1){
		unsigned long pushed;
		pushed = GPIO_PORTE_DATA_BITS_R [INPUT];
		if (pushed){ // 3.Check if button is push (if high)
			Delay1ms(100); // 3a_1. if Button is push delay 100 ms
			LED_Toggle(); // 3a_2. Toggle LED 	
		}
		else{
			LED_On();// if it's not push (low) turn LED ON
		}
  }
}

void InitPortE(void){
	unsigned long delay;
   //1. Active Clockport
	SYSCTL_RCGC2_R |= SYSCTL_RCGCGPIO_R4; // 1. enable clock port for Port E
	delay |= SYSCTL_RCGCGPIO_R4; //2. delay the clock
	// no need to unlock pins
	GPIO_PORTE_AMSEL_R &= ~0x03; //2. Disable analog function of the pins
	GPIO_PORTE_PCTL_R  &= ~0x03; //3. Clear bits in the PCTl for regular function
	GPIO_PORTE_DIR_R |= 0x02; //4a. set Port E Pin 1 as output 
	GPIO_PORTE_DIR_R &= ~0x01; //4b. set Port E Pin 0 as input
	GPIO_PORTE_AFSEL_R &= ~0x03;  //5. Clear bits in alternate function register 
	GPIO_PORTE_DEN_R  |= 0x03; //6. Enable Digital port 
}
void LED_On(void){
    GPIO_PORTE_DATA_BITS_R  [LED] = LED;
}
void LED_Toggle(void){
    GPIO_PORTE_DATA_BITS_R [LED] ^= LED;
}

void Delay1ms(unsigned long msec){
	for (;msec >0;msec--){
		unsigned long count;
		for( count = 13333;count>0;count--){}
	}
}

