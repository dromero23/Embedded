// BranchingFunctionsDelays.c Lab 6
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to 
// toggle an LED while a button is pressed and 
// turn the LED on when the button is released.  
// This lab will use the hardware already built into the LaunchPad.
// Daniel Valvano, Jonathan Valvano
// January 15, 2016

// built-in connection: PF0 connected to negative logic momentary switch, SW2
// built-in connection: PF1 connected to red LED
// built-in connection: PF2 connected to blue LED
// built-in connection: PF3 connected to green LED
// built-in connection: PF4 connected to negative logic momentary switch, SW1

#include "TExaS.h"

#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOF      0x00000020  // port F Clock Gating Control
#define PORTFPIN4_BIT						0x10
#define PORTFPIN2_BIT						0x04
#define PORTFPIN24							0x14
unsigned long In; //input PF4
unsigned long Out; //output PF2

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void PortFInit(void);
void Delay100ms(unsigned long time);

int main(void){ unsigned long volatile delay;
  TExaS_Init(SW_PIN_PF4, LED_PIN_PF2);  // activate grader and set system clock to 80 MHz
  PortFInit();
	Out = PORTFPIN2_BIT;
	GPIO_PORTF_DATA_R = Out;// set the output default to Blue
  EnableInterrupts();           // enable interrupts for the grader
  while(1){
		Delay100ms(1);
		In = GPIO_PORTF_DATA_R&0x10;
		if(!In){
			Out = ~Out & PORTFPIN2_BIT;
			GPIO_PORTF_DATA_R = Out;
		}
		else {
			Out = PORTFPIN2_BIT;
			GPIO_PORTF_DATA_R = Out;
		}
		
    // body goes here
  }
}

void PortFInit(void){
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF; //1. Active the clock port 
	delay = SYSCTL_RCGC2_R; //allow time for the clock to start
	//2. Unlock the port (only needed for pins PC3-0, PD7, PF0)
	//No need to unlock
	GPIO_PORTF_AMSEL_R &= ~PORTFPIN24;	//3. Disable the analog function of the pin 
	GPIO_PORTF_PCTL_R &= ~PORTFPIN24; //4. Clear bits in the PCTL for regular function 
	//5. Set the direction register 
	GPIO_PORTF_DIR_R |= PORTFPIN2_BIT;  // Port F pin 2 as an output (BLUE LED) 
	GPIO_PORTF_DIR_R &= ~PORTFPIN4_BIT; // Port F pin 4 as an input (SW1)
	GPIO_PORTF_AFSEL_R &= ~PORTFPIN24; //6. Clear bits in alternate function register 
	GPIO_PORTF_DEN_R |= PORTFPIN24; //7. Enable Digital Port 
	//extras
	GPIO_PORTF_PUR_R |= PORTFPIN4_BIT; //enable internal pull up resistors
}


void Delay100ms(unsigned long time){
  unsigned long i;
  while(time > 0){
    i = 1333333;  // this number means 100ms
    while(i > 0){
      i = i - 1;
    }
    time = time - 1; // decrements every 100 ms
  }
}
