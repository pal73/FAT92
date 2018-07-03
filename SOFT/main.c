#include "stm8s.h"

@near bool b100Hz=0,b10Hz=0,b5Hz=0,b1Hz=0;
@near static char t0_cnt0=0,t0_cnt1=0,t0_cnt2=0,t0_cnt3=0;

//signed long temp_adc;
unsigned short temp_adc;
char start_cnt;

short adc_buff[32][3];
short adc_buff_[3];
short adc_buff_instant[3];
short adc_ch;
short adc_cnt;
short adc_sign=0;

signed short temper;
signed short temper_ust_water;
signed short temper_ust_air;

@near short led_drv_cnt;
@near short led_drv_dig1,led_drv_pause1, dig1;
@near short led_drv_dig2,led_drv_pause2, dig2;
@near short led_drv_dig3,led_drv_pause3, dig3;
@near short led_drv_dig4,led_drv_pause4, dig4;
@near short led_drv_dig5,led_drv_pause5, dig5;
@near short led_drv_dig6,led_drv_pause6, dig6;
//-----------------------------------------------
void led_drv(void)
{
GPIOB->DDR|=(1<<4);			//Светодиод
GPIOB->CR1&=~(1<<4);
GPIOB->CR2&=~(1<<4);

led_drv_cnt++;

if(led_drv_cnt<=5)			GPIOB->ODR&=~(1<<4);
else if(led_drv_cnt<=10)	GPIOB->ODR|=(1<<4);
else if(led_drv_cnt%2)		GPIOB->ODR|=(1<<4);
else
	{
	if(led_drv_dig1)
		{
		led_drv_dig1--;
		GPIOB->ODR&=~(1<<4);
		}		
	else if(led_drv_pause1)
		{
		led_drv_pause1--;
		GPIOB->ODR|=(1<<4);
		}
	else if(led_drv_dig2)
		{
		led_drv_dig2--;
		GPIOB->ODR&=~(1<<4);
		}		
	else if(led_drv_pause2)
		{
		led_drv_pause2--;
		GPIOB->ODR|=(1<<4);
		}	
	else if(led_drv_dig3)
		{
		led_drv_dig3--;
		GPIOB->ODR&=~(1<<4);
		}		
	else if(led_drv_pause3)
		{
		led_drv_pause3--;
		GPIOB->ODR|=(1<<4);
		}
	else if(led_drv_dig4)
		{
		led_drv_dig4--;
		GPIOB->ODR&=~(1<<4);
		}		
	else if(led_drv_pause4)
		{
		led_drv_pause4--;
		GPIOB->ODR|=(1<<4);
		}	
	else if(led_drv_dig5)
		{
		led_drv_dig5--;
		GPIOB->ODR&=~(1<<4);
		}		
	else if(led_drv_pause5)
		{
		led_drv_pause5--;
		GPIOB->ODR|=(1<<4);
		}	
	else if(led_drv_dig6)
		{
		led_drv_dig6--;
		GPIOB->ODR&=~(1<<4);
		}		
	else if(led_drv_pause6)
		{
		led_drv_pause6--;
		GPIOB->ODR|=(1<<4);
		}
	else if(!led_drv_pause6)
		{
		led_drv_cnt=0;
		if(dig1)
			{
			led_drv_dig1=dig1;	
			led_drv_pause1=3;
			}
		if(dig2)
			{
			led_drv_dig2=dig2;	
			led_drv_pause2=3;
			}			
		if(dig3)
			{
			led_drv_dig3=dig3;	
			led_drv_pause3=3;
			}
		if(dig4)
			{
			led_drv_dig4=dig4;	
			led_drv_pause4=3;
			}	
		if(dig5)
			{
			led_drv_dig5=dig5;	
			led_drv_pause5=3;
			}
		if(dig6)
			{
			led_drv_dig6=dig6;	
			led_drv_pause6=3;
			}			
		}
	}

}

//-----------------------------------------------
void matemath(void)
{
long tempL;
temper_ust_water=5+((adc_buff_[0]*10)/128);
temper_ust_air=5+((adc_buff_[0])/34);
tempL=0;
tempL=(long)adc_buff_[2];
tempL*=500L;
tempL/=1023L;
tempL-=273L;
temper=(short)tempL;
}

//-----------------------------------------------
void out_hndl(void)
{


GPIOB->DDR|=(1<<5);			//Оптрон
GPIOB->CR1&=~(1<<5);
GPIOB->CR2&=~(1<<5);

if(adc_buff_[1]>900)		//Переключатель в среднем положении, выход выключен
	{
	//GPIOB->ODR^=(1<<4);	
	GPIOB->ODR|=(1<<5);	
	}
else if(adc_buff_[1]<200)		//Переключатель в положении "Вода"
	{
	//GPIOB->ODR&=~(1<<4);	
	if(temper>=temper_ust_water)
		{
		GPIOB->ODR|=(1<<5);	
		//GPIOB->ODR|=(1<<4);
		}
	else if(temper<temper_ust_water)
		{
		GPIOB->ODR&=~(1<<5);
		//GPIOB->ODR&=~(1<<4);
		}
	}	
	
else if((adc_buff_[1]<800) && (adc_buff_[1]>300))		//Переключатель в положении "Воздух"
	{
	//GPIOB->ODR|=(1<<4);	
	if(temper>=temper_ust_air)
		{
		GPIOB->ODR|=(1<<5);	
		//GPIOB->ODR|=(1<<4);
		}
	else if(temper<temper_ust_air)
		{
		GPIOB->ODR&=~(1<<5);
		//GPIOB->ODR&=~(1<<4);
		}
	}	
//GPIOB->ODR^=(1<<4);	
}

//-----------------------------------------------
void adc1_hndl(void)
{
short temp_adc;


temp_adc=(((short)(ADC1->DRH))*256)+((short)(ADC1->DRL));

adc_buff[adc_cnt][adc_ch]=temp_adc;
adc_buff_instant[adc_ch]=temp_adc;

if(!adc_sign)adc_buff_[adc_ch]=adc_buff_instant[adc_ch];
else
	{
	if((adc_cnt&0x0003)==0)	
		{
		unsigned short tempUS=0;
		char i;
		for(i=0;i<32;i++)
			{
			tempUS+=adc_buff[i][adc_ch];
			}
		adc_buff_[adc_ch]=tempUS>>5;
		}
	}
	
if(++adc_ch>=3)
	{
	adc_ch=0;
	if(++adc_cnt>=32)
		{
		adc_cnt=0;
		adc_sign=1;
		//GPIOB->ODR^=(1<<4);
		}
	}


GPIOD->DDR&=~(1<<6);
GPIOD->CR1&=~(1<<6);
GPIOD->CR2&=~(1<<6);

if(start_cnt>10)
	{
	GPIOD->DDR&=~(1<<3);
	GPIOD->CR1|=(1<<3);
	GPIOD->CR2&=~(1<<3);
	}
	
GPIOC->DDR&=~(1<<4);
GPIOC->CR1&=~(1<<4);
GPIOC->CR2&=~(1<<4);

ADC1->TDRL=0xff;
	
ADC1->CR2=0x08;
ADC1->CR1=0x40;

if(adc_ch==0)		ADC1->CSR=0x06;
else if(adc_ch==1)	ADC1->CSR=0x04;
else if(adc_ch==2)	ADC1->CSR=0x02;

ADC1->CR1|=1;
ADC1->CR1|=1;

}

//-----------------------------------------------
void adc1_init(void)
{



//GPIOD->DDR&=~(1<<5);
//GPIOD->CR1&=~(1<<5);
//GPIOD->CR2&=~(1<<5);

GPIOD->DDR&=~(1<<6);
GPIOD->CR1&=~(1<<6);
GPIOD->CR2&=~(1<<6);

if(start_cnt>10)
	{
	GPIOD->DDR&=~(1<<3);
	GPIOD->CR1|=(1<<3);
	GPIOD->CR2&=~(1<<3);
	}

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
CLK->CKDIVR=4;

t4_init();

adc1_init();
enableInterrupts();
while (1)
	{
	if(b10Hz)
		{
		b10Hz=0;
	
		//GPIOB->ODR^=(1<<4);
		//if(temp_adc>512) GPIOB->ODR|=(1<<4);
		//else GPIOB->ODR&=~(1<<4);
		//if(start_cnt>10) GPIOB->ODR|=(1<<4);
		//else GPIOB->ODR&=~(1<<4);
		/*if(adc_buff_[2]>620)GPIOB->ODR|=(1<<4);
		else GPIOB->ODR&=~(1<<4);*/
		}
      	 
	if(b5Hz)
		{
		b5Hz=0;
		
		led_drv();
		//adc1_init();
		adc1_hndl();		
		}
      	      	
	if(b1Hz)
		{
		b1Hz=0;
		if(start_cnt<200)start_cnt++;
		
		matemath();
		out_hndl();
		}
	};

}