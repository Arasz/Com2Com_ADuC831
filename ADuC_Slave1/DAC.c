#include "DAC.h"

/// \brief Inicjalizuje DACx.
///
/// DAC zostaje uruchomiony w trybie 12 bitowym, w zakresie napiec wyjsciowych od 0-VDD i z wylaczona synchronizacja.
/// \params [in] nrDAC Numer DAC ktory zostanie zainicjalizowany.
/// \returns Wartosc pusta.
void DAC_Init(uchar nrDAC)
{
	DACx_POWER_ON(nrDAC); // Uruchom DACx.
	DACx_MODE_12BIT; // Ustaw oba DAC w tryb 12 bitowy
	DACx_RANGE_VDD(nrDAC); // Zakres napiec wyjsciowych dla DACx to 0-VDD
	DACx_OUTPUT_NORM(nrDAC); // Wlacz wyjscie dla DACx;
}

/// \brief Ustawia napiecie wyjsciowe dla okreslonego DAC.
///
/// Funkcja wylacza synchronizacje, ustawia odpowiednia wartosc po czym wlacza synchronizacje.
/// \params [in] nrDAC Numer DAC ktory zostanie zainicjalizowany.
/// \params [in] voltage Napiecie wejsciowe.
/// \returns Wartosc pusta.
void DAC_SetVoltage(uchar nrDAC, float voltage)
{
	DACx_SYNC_OFF; // Wylacz synchronizacje dla obu DAC.
	
	if(nrDAC == DAC0)
		SET_DACx_REG_VAL(0,voltage);
	else
		SET_DACx_REG_VAL(1,voltage);
	
	DACx_SYNC_ON; // Wlacz synchrronizacje dla obu DAC.
}


