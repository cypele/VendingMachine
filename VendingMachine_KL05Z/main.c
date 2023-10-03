// Vending Machine Project
// Adam Cyplinski


#include "MKL05Z4.h"
#include "frdm_bsp.h"
#include "lcd1602.h"
#include "leds.h"
#include "uart0.h"
#include "i2c.h"
#include "tsi.h"
#include "klaw.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define S5_LIMIT	9
#define S2_LIMIT	9
#define S3_LIMIT	9	// Limit liczby wcisniec klawisza S3
#define S4_LIMIT	9	// Limit liczby wcisniec klawisza S4

volatile uint8_t S5_press=0;
volatile uint8_t S2_press=0;	// "1" - klawisz zostal wcisniety "0" - klawisz "skonsumowany"
volatile uint8_t S3_press=0;
volatile uint8_t S4_press=0;

uint8_t temp=0;
uint8_t wynik=0;

uint8_t wynik_ok=0;
uint8_t S2_nr=0;
uint8_t S3_nr=0;			// Liczba wcisniec klawisza S3
uint8_t S4_nr=0;						// Liczba wcisniec klawisza S4
uint8_t S5_nr=0;

void PORTA_IRQHandler(void)	// Podprogram obslugi przerwania od klawiszy S2, S3 i S4
{
	uint32_t buf;
	buf=PORTA->ISFR & ( S5_MASK | S2_MASK | S3_MASK | S4_MASK);

	switch(buf)
	{	
		case S5_MASK:	DELAY(10)
								if(!(PTA->PDIR&S5_MASK))		// Minimalizacja drgan zestyków
								{
									if(!(PTA->PDIR&S5_MASK))	// Minimalizacja drgan zestyków (c.d.)
									{
										if(!S5_press)
										{
											S5_nr+=1;
											if(S5_nr>S2_LIMIT)
												S5_nr=0;
												S5_press=1;
										}
									}
								}
		case S2_MASK:	DELAY(10)
									if(!(PTA->PDIR&S2_MASK))		// Minimalizacja drgan zestyków
									{
										if(!(PTA->PDIR&S2_MASK))	// Minimalizacja drgan zestyków (c.d.)
										{
											if(!S2_press)
											{
												S2_nr+=1;
												if(S2_nr>S2_LIMIT)
													S2_nr=0;
												S2_press=1;
											}
										}
									}
									break;
		case S3_MASK:	DELAY(10)
									if(!(PTA->PDIR&S3_MASK))		// Minimalizacja drgan zestyków
									{
										if(!(PTA->PDIR&S3_MASK))	// Minimalizacja drgan zestyków (c.d.)
										{
											if(!S3_press)
											{
												S3_nr+=1;
												if(S3_nr>S3_LIMIT)
													S3_nr=0;
												S3_press=1;
											}
										}
									}
									break;
		case S4_MASK:	DELAY(10)
									if(!(PTA->PDIR&S4_MASK))		// Minimalizacja drgan zestyków
									{
										if(!(PTA->PDIR&S4_MASK))	// Minimalizacja drgan zestyków (c.d.)
										{
											if(!S4_press)
											{
												S4_nr+=1;
												if(S4_nr==2)
												if(S4_nr>S3_LIMIT)
													S4_nr=0;
												S4_press=1;
											}
										}
									}
									break;
		default:			break;
	}	

	
	
		temp=S2_nr*10 + S3_nr;
	if(!wynik_ok){

		wynik=temp;
		wynik_ok=1;

		
	}
	
  PORTA->ISFR |=   S5_MASK  | S2_MASK | S3_MASK | S4_MASK;	// Kasowanie wszystkich bitów ISF
	NVIC_ClearPendingIRQ(PORTA_IRQn);
}








int main (void) 
{
	uint32_t baud_rate, i=0;
	uint8_t w=0;
	
	char display[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
	Klaw_Init();								// Inicjalizacja klawiatury
	Klaw_S2_4_Int();						// Klawisze S2, S3 i S4 zglaszaja przerwanie
	UART0_Init();		// Inicjalizacja portu szeregowego UART0
	TSI_Init();
	LCD1602_Init();		 					// Inicjalizacja LCD
	LCD1602_Backlight(TRUE);  	// Wlaczenie podswietlenia
	LCD1602_Print("Wybierz BR");
	
	
	
	while(w==0)
		w=TSI_ReadSlider();		// Ustaw BR dla UART0, dotykaac pole: 0<1/3 - 9600, 1/3<2/3 - 28800,2/3< - 230400
	
	UART0->C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK );		//Blokada nadajnika i o dbiornika
	
	if(w<33)
	{
		UART0->BDH = 1;			//Dla CLOCK_SETUP=0 BR=9600
		UART0->BDL =17;			//Dla CLOCK_SETUP=0 BR=9600
		baud_rate=9600;
	}
	if((w>33)&&(w<66))
	{
		UART0->BDH = 0;			//Dla CLOCK_SETUP=0 BR=28800
		UART0->BDL =91;			//Dla CLOCK_SETUP=0 BR=28800
		baud_rate=28800;
	}
	if(w>66)
	{
		UART0->BDH = 0;			//Dla CLOCK_SETUP=0 BR=230400
		UART0->BDL =11;			//Dla CLOCK_SETUP=0 BR=230400
		baud_rate=230400;
	}

	sprintf(display,"BR=%d b/s",baud_rate);	// Wyswietlenie aktualnejj wartosci BR
	LCD1602_SetCursor(0,0);
	LCD1602_Print(display);
	LCD1602_SetCursor(0,1);
	LCD1602_Print("Ustaw BR w PC...");
	DELAY(500)
	w=0;
	while(w==0)
		w=TSI_ReadSlider();		// Ustaw BR w komputerze i dotknij pole dotykowe
	
	LCD1602_SetCursor(0,1);
	LCD1602_Print("Transmisja...   ");
	UART0->C2 |= UART0_C2_TE_MASK;		//Wlacz nadajnik
	
	
	LCD1602_ClearAll();					// Wyczysc ekran
	LCD1602_Print("Wybierz produkt");
	
	
  
	while(1)
	{
		if(S5_press)							// Wyswietl ilosc wzisniec klawisza S3
		{
			S5_press=0;
		}
		if(S2_press)							// Wyswietl ilosc wzisniec klawisza S3
		{
			S2_press=0;
		}
		if(S3_press)							// Wyswietl ilosc wzisniec klawisza S3
		{
			S3_press=0;
		}
		if(S4_press)							// Wyswietl ilosc wzisniec klawisza S3
		{
			if(S4_nr%2==0)
			{
			LCD1602_ClearAll();
			LCD1602_SetCursor(0,0);
			LCD1602_Print("Zatwierdzono");
			}
			
			S5_nr=0;
			S2_nr=0;
			S3_nr=0;
			S4_press=0;

		}

		if (wynik_ok)
		{
			if(S4_nr==0 || S4_nr%3==0){
			LCD1602_ClearAll();
			LCD1602_SetCursor(0,1);
			sprintf(display, "soda=%02d%c", wynik, 0xd);
			LCD1602_Print(display);
			}

			for (i = 0; display[i] != 0; i++)
			{
				while (!(UART0->S1 & UART0_S1_TDRE_MASK));	// Czy nadajnik jest pusty?
				UART0->D = display[i];		// Wyslij aktualna wartosc licznika
			}
			
			DELAY(100)
			wynik_ok = 0;
		}
		
	}
}