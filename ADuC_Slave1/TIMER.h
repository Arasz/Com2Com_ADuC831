/// \file Timer.h
/// \brief Biblioteka obslugujaca TIMx.
#ifndef __TIMER_H__
#define __TIMER_H__

#include "ADuC831_STD.h"

// STALE TIMER
#define TIMER_ON 0x01 ///< Wlacz timer.
#define TIMER_OFF 0x00 ///< Wylacz timer.

#define TIM0_MODE1  0x01 ///< Licznik 16 bitowy 
#define TIM0_MODE2  0x02 ///< Licznik 8 bitowy + auto-reload
#define TIM1_MODE1  0x10 ///< Licznik 16 bitowy 
#define TIM1_MODE2  0x20 ///< Licznik 8 bitowy + auto-reload

#define TIM2_ENABLE_DIGITAL_INPUT 0xFC;

#define TIM_MAX_VAL(REG_BITS_NUMBER) ((1<<REG_BITS_NUMBER)) ///< Oblicza maksymalna wartosc na podstawie ilosci bitow rejestru.

// MAKRA
#define TS2REG(TS) (TIM_MAX_VAL(16) - ((TS*F_OSC*0.001)/12)) ///< Oblicza wartosc rejestru TIM na podstawie podanego okresu
// STALE POMOCNICZE
#define AUTO_RELOAD_VALUE TS2REG(T_SAMPLE)


// FUNKCJE 
void TIM2_Init();
void TIM0_Init(uint initValue);
void TIM0_TimerAutoReload(uint reloadValue);

#endif