#include "stm8s.h"

@near bool b100Hz=0,b10Hz=0,b5Hz=0,b1Hz=0;
@near static char t0_cnt0=0,t0_cnt1=0,t0_cnt2=0,t0_cnt3=0;

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


main()
{
CLK->CKDIVR=0;

t4_init();
enableInterrupts();
while (1)
	{
	if(b10Hz)
		{
		b10Hz=0;
/*   	if(ibatton_polling())
			{
			ibatton_send_byte(0x44);
			}*/
		}
      	 
	if(b5Hz)
		{
		b5Hz=0;
		

			
		}
      	      	
	if(b1Hz)
		{
		b1Hz=0;
		}
	};

}