
#ifndef __ADUC831_STD__H__
#define __ADUC831_STD__H__

// BIBLIOTEKI

#include "aduc831.h"
// TYPY
typedef unsigned char uchar; ///< Unsigned char type.
typedef unsigned int uint; ///< Unsigned int type. 

// MAKRA
#define isSet(Px,Bit) !(Px&(1<<Bit)) // Dotyczy wejscia uC. Negacja na koncu spowodowane specyficzna budowa ukladu 

#define setBit(Byte,Bit) (Byte |=(1<<Bit)) ///< Ustawia bit 'Bit' bajtu 'Byte'

#define clearBit(Byte,Bit) (Byte &=(~(1<<Bit))) ///< Czysci bit 'Bit' bajtu 'Byte'

#define checkBit(Byte,Bit) ((Byte&(1<<Bit)))  ///< Sprawdza stan bitu 'Bit' bajtu 'Byte'.

// STALE LED
#define LED_ON 0x1 // Wlacz diode.
#define LED_OFF 0x0 // Wylacz diode.

#define ACTIVE 'A'
#define NOT_ACTIVE 'N'

// STALE PRZERWANIA
#define INTERRUPT_ENABLE 0x1 ///< Wlaczenie przerwania. 
#define INTERRUPT_DISABLE 0x0 ///< Wylaczenie przerwania.
#define HIGH_PRIORITY 0x1 ///< Przerwanie, wysoki priorytet.
#define LOW_PRIORITY 0x0 ///< Przerwanie, niski priorytet. 
#define EDGE_SENSITIVE_DETECTION 0x1 ///< Przerwanie, wyzwalnie na zboczu.
#define LEVEL_SENSITIVE_DETECTION 0x0 ///< Przerwanie, wyzwalanie poziomem. 

// STALE FLAGI
#define SET_FLAG 0x01 ///< Ustaw flage.
#define CLEAR_FLAG 0x0 ///< Wyczysc flage.

// STALE BITY
#define SET_BIT 0x1			///< Ustawia bit w stan wysoki 
#define CLEAR_BIT 0x0		///< Ustawia bit w stan niski


#endif