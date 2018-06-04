#include "stm8s.h"

@near bool b100Hz=0,b10Hz=0,b5Hz=0,b1Hz=0;
@near static char t0_cnt0=0,t0_cnt1=0,t0_cnt2=0,t0_cnt3=0;

//signed long temp_adc;
unsigned short temp_adc;

//-----------------------------------------------
void adc1_init(void)
{



//GPIOD->DDR&=~(1<<5);
//GPIOD->CR1&=~(1<<5);
//GPIOD->CR2&=~(1<<5);

GPIOD->DDR&=~(1<<6);
GPIOD->CR1&=~(1<<6);
GPIOD->CR2&=~(1<<6);

//GPIOC->DDR&=~(1<<4);
//GPIOC->CR1&=~(1<<4);
//GPIOC->CR2&=~(1<<4);

//GPIOB->DDR&=~(1<<7);
//GPIOB->CR1&=~(1<<7);
//GPIOB->CR2&=~(1<<7);



ADC1->TDRL=0xff;
	
ADC1->CR2=0x08;
ADC1->CR1=0x40;
//if(adc_ch)
	{
	ADC1->CSR=0x20+6;
	
	ADC1->CR1|=1;
	ADC1->CR1|=1;
	}

//adc_plazma[1]=adc_ch;
}


//-----------------------------------------------
void t4_init(void)
{
TIM4->PSCR = 3;
TIM4->ARR= 158;
TIM4->IER|= TIM4_IER_UIE;					// enable break interrupt

TIM4->CR1=(TIM4_CR1_URS | TIM4_CR1_CEN | TIM4_CR1_ARPE);	
	
}

//***********************************************
//***********************************************
//***********************************************
//***********************************************
@far @interrupt void TIM4_UPD_Interrupt (void) 
{
if(++t0_cnt0>=125)
	{
  t0_cnt0=0;
  b100Hz=1;


	if(++t0_cnt1>=10)
		{
		t0_cnt1=0;
		b10Hz=1;
		//bWFI=1;
		}
		
	if(++t0_cnt2>=20)
		{
		t0_cnt2=0;
		b5Hz=1;
		//bWFI=1;
		}
		
	if(++t0_cnt3>=100)
		{
		t0_cnt3=0;
		b1Hz=1;
		}
	}
TIM4->SR1&=~TIM4_SR1_UIF;			// disable break interrupt
return;
}

//***********************************************
@far @interrupt void ADC1_EOC_Interrupt (void) {




ADC1->CSR&=~(1<<7);

temp_adc=(ADC1->DRH);
temp_adc=(((signed long)(ADC1->DRH))*256)+((signed long)(ADC1->DRL));
}



main()
{
CLK->CKDIVR=0;

t4_init();

adc1_init();
enableInterrupts();
while (1)
	{
	if(b10Hz)
		{
		b10Hz=0;
		GPIOB->DDR|=(1<<5);
		GPIOB->CR1&=~(1<<5);
		GPIOB->CR2&=~(1<<5);	
		//GPIOB->ODR^=(1<<5);
		if(temp_adc>512) GPIOB->ODR|=(1<<5);
		else GPIOB->ODR&=~(1<<5);
		}
      	 
	if(b5Hz)
		{
		b5Hz=0;
		

		adc1_init();	
		}
      	      	
	if(b1Hz)
		{
		b1Hz=0;
		}
	};

}