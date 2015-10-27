#ifndef __DAC_H__
#define __DAC_H__

#include "ADuC831_STD.h"


//STALE 
#define DAC0  0x0 
#define DAC1  0x1

#define T_SAMPLE 2 ///< Okres probkowania w sekundach [ms] - dostepne wartosci: 2,4,6,8,10 


#define VDD  5.0 ///< Napiecie zasilania DAC.
#define MAX_REG_VAL  4095 ///< Maksymalna wartosc która mozemy wpisac do resestru obliczana jako 2^(RESOLUTION)-1.


//MAKRA

#define V2REG(V) (uint)(((V/VDD))*(MAX_REG_VAL)) ///< Makro przelicza wartosc podana w woltach na wartosc rejestru
#define SET_DACx_REG_VAL(x,V) do{DAC##x##H =(V2REG(V)>>8); DAC##x##L = (0x00ff&V2REG(V));}while(0) ///< Ustawia podane napiecie na wyjsciu DACx

#define DACx_MODE_8BIT setBit(DACCON,7) ///<  Wlacz oba DAC w tryb 8 bitowy.
#define DACx_MODE_12BIT clearBit(DACCON,7) ///<  Wlacz oba DAC w tryb 12 bitowy.

#define DACx_RANGE_VDD(x) setBit(DACCON,5+x) ///<  Ustaw zakres dla DACx na 0V-VDD.
#define DACx_RANGE_REF(x) clearBit(DACCON,5+x) ///<  Ustaw zakres dla DACx na 0V-VREF.

#define DACx_OUTPUT_NORM(x) setBit(DACCON,3+x) ///<  Wyjscie DACx wlaczone.
#define DACx_OUTPUT_0V(x) clearBit(DACCON,3+x) ///<  Wymus na wyjsciu DACx 0V.

#define DACx_SYNC_ON  setBit(DACCON,2) ///<  Wlacz synchronizacje dla obu DAC.
#define DACx_SYNC_OFF clearBit(DACCON,2) ///<  Wylacz synchronizacje dla obu DAC.

#define DACx_POWER_ON(x) setBit(DACCON,0+x) ///<  Wlacz DACx.
#define DACx_POWER_OFF(x) clearBit(DACCON,0+x) ///<  Wylacz DACx.



//FUNKCJE

void DAC_Init(uchar dacNr);
void DAC_SetVoltage(uchar nrDAC, float voltage);



#endif 