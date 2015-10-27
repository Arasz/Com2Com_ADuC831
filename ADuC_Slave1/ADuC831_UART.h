#ifndef __ADUC831_UART_H__
#define __ADUC831_UART_H__

#include "ADuC831.h"
#include "ADuC831_STD.h"


#define UART_8BIT_VAR_BAUD 0x1
#define SET_UART_MODE(mode) do{ SM1 = 0x1&mode; SM0 = 0x2&mode;}while(0) 
#define BAUD_RATE 9600
#define TIM3_BAUD_ENABLE (T3CON |= (1<<7))
#define DIV 0x5 ///< Wartosc liczona z zaleznosci DIV = ln(fcore/32*baud)/ln(2)), zaokroaglona w dol
#define T3FD_VAL 0x8///< Wartosc liczona z zaleznosci T3FD = (2*fcore)/((2^DIV)*baud)-64, zaokroaglona do najblizeszej wartosci calkowitej
#define SET_REQUIRED_BAUD do{ T3CON |= DIV; T3FD = T3FD_VAL;  }while(0)
#define BUFFER_SIZE 14

/// \brief Set up UART
void UART_Init();


#endif 