#include "Timer.h"

/// \brief Inicjalizuje TIM2.
///
///
/// \returns Wartosc pusta.

void TIM2_Init()
{
  EXEN2 = SET_BIT; // Uruchomienie przechwytywania na negatywnym zboczu
  CNT2 = SET_BIT; // Ustawienie TIM2 w tryb licznika
  TR2 = TIMER_ON;

  EA = INTERRUPT_ENABLE; // Uuchom wszystkie przerwania
  ET2 = INTERRUPT_ENABLE; // Uruchom przerwanie od TIM2

  RCAP2L = 0xff; // Sluzy jako reload register
  RCAP2H = 0xff;

  TL2 =  0xff; // Wpisz wartosc poczatkowa do rejestru tim2
  TH2 =  0xff;
  
  P1 = TIM2_ENABLE_DIGITAL_INPUT; 
}

/// \brief Inicjalizuje TIM0.
///
/// Okres T TIM0 dla fosc = 11.058 MHz wynosi 200 us.
/// \returns Wartosc pusta.
void TIM0_Init(uint initValue)
{
	TMOD = TIM0_MODE1; // Ustaw TIM0 w tryb licznika 16 bitowego
	
	// Wartosc w rejestrze T0 = 63693 = 0xF8CD i odpowiada T = 200 us dla fosc = 11.058 MHz
	TL0 = (0x00ff&initValue);//0xcd ; 
	TH0 = (initValue>>8);//0xf8 ;
	
	EA = INTERRUPT_ENABLE; // Uuchom wszystkie przerwania
	ET0 = INTERRUPT_ENABLE; // Wlacz przerwanie od TIM0
	PT0 = HIGH_PRIORITY; // Wysoki pryiorytet dla przerwan od TIM0
	TR0 = TIMER_ON;
}

/// \brief Realizuje auto przeladowanie TIM0
///
/// \param [in] reloadValue Wartosc od ktorej zacznie odliczac TIM0.
/// \returns Wartosc pusta.
void TIM0_TimerAutoReload(uint reloadValue)
{
	TR0 = TIMER_OFF;
	TL0 = (0x00ff&reloadValue);
	TH0 = (reloadValue>>8);
	TR0 = TIMER_ON;
}