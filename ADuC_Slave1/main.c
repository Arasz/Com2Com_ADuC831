#include "ADuC831.h"
#include "ADuC831_STD.h"
#include "ADuC831_UART.h"
#include "DAC.h"
	
#define SLAVE_ID 0x0C
#define BROADCAST_ID 0x3F
#define PORT P2



uchar READ_READY = 0; ///< Flaga gotowosci do odczytu
uchar MESSAGE_IN_BUFFER;

uchar txBuffer[BUFFER_SIZE];
uchar rxBuffer[BUFFER_SIZE];

uchar flush = 0;

void UART_Init()
{
	rxBuffer[0] = 1;
	txBuffer[0] = 0;
	
	SET_UART_MODE(UART_8BIT_VAR_BAUD);
	REN = SET_BIT; // Uruchom odbieranie danych
	EA = INTERRUPT_ENABLE;  // Zgoda globalna na przerwanie
	ES = INTERRUPT_ENABLE; // Zgoda na przerwanie od portu szeregowego
	// Generowanie BAUDRATE - TIM3
	TIM3_BAUD_ENABLE;
	SET_REQUIRED_BAUD;
}

// Struct definition //

typedef struct ProtocolFrames
{
	uchar SlaveId;
	uchar DataLength;
	char Command[2];
	uchar Data[10];
	
} ProtocolFrame;

const char constProtocolFrameLength = 4;

// Enum definition // 
enum Command
{
	SA = 733,
	SD = 763,
	AS = 895,
	ID = 753,
};

int CalculateEnumValue(char* cmd)
{
	return (int)cmd[0]+10*cmd[1];
}
//--**--//

void UART_Interrupt_Handler(void) interrupt 4
{
	int ProtocolFrameLength = constProtocolFrameLength;
	
	if(RI == SET_FLAG)
	{
		if(MESSAGE_IN_BUFFER)
		{
			if(rxBuffer[0]< BUFFER_SIZE) // Sprawdz czy jest miejsce w buforze
			{
				rxBuffer[rxBuffer[0]] = SBUF; // Przypisz odebrany bajt do wolnego bajtu w buforze
				
				if(rxBuffer[0]>=2&&ProtocolFrameLength<=4) // Odczytano ilosc danych 
					ProtocolFrameLength+=rxBuffer[2];
				
				rxBuffer[0]++;
			}
			if(rxBuffer[0]> ProtocolFrameLength)
			{
				READ_READY = SET_FLAG; // Dane gotowe do odczytu
				MESSAGE_IN_BUFFER = CLEAR_FLAG;
			}
		}
		else 
		{
				if(SBUF==SLAVE_ID||SBUF==BROADCAST_ID)
				{
								MESSAGE_IN_BUFFER = SET_FLAG;
								rxBuffer[rxBuffer[0]] = SBUF; // Przypisz odebrany bajt do wolnego bajtu w buforze
								rxBuffer[0]++; // Zwieksz licznik buffora
				}	
				else
				{
					rxBuffer[0] = 1;
				}
			
		}
		RI = CLEAR_FLAG; // Wyczysc flage przerwania
	}
	if(TI == SET_FLAG)
	{
		if(txBuffer[0]>1)
		{
			SBUF = txBuffer[txBuffer[0]--];
			TI = CLEAR_FLAG;
		}
		else
		{
			TI = CLEAR_FLAG;
		}
	}
}



uchar SLAVE_ReceiveData(ProtocolFrame *frame)
{
	int i;
	int index = 1;
	uchar flag = CLEAR_FLAG;
	
	if(rxBuffer[index]==frame->SlaveId||rxBuffer[index]==BROADCAST_ID)
	{
		index++;
		// Read data length
		frame->DataLength = rxBuffer[index++];
		// Read command
		for(i = 0; i<2; i++, index++)
		{
			frame->Command[i]= rxBuffer[index];
		}
		// Read data
		for(i = 0;i<frame->DataLength;i++,index++)
		{
			frame->Data[i] = rxBuffer[index];
		}
		flag = SET_FLAG;
	}
	rxBuffer[0] = 1; // Reset licznika
	READ_READY = CLEAR_FLAG;
	return flag;
}

void SLAVE_SendData(ProtocolFrame *frame)
{
	int i;
	int index = 5+frame->DataLength;
	txBuffer[0] = index-1;
	
	txBuffer[index--] = frame->SlaveId; 
	txBuffer[index--] = frame->DataLength;
	// Write command
	for(i = 0; i<2; i++, index--)
	{
			txBuffer[index] = frame->Command[i];
	}
  // Read data
	for(i = 0; i<frame->DataLength; i++, index--)
	{
			txBuffer[index] = frame->Data[i];
	}
	SBUF = txBuffer[txBuffer[0]+1];
}

void ReadDigitalIo(ProtocolFrame *frame)
{
	if(frame->DataLength <=2)
	{
		frame->Data[0] = 0x0;
		frame->Data[1] = 0x0;
		
		frame->Data[0] = PORT&0xFF;
	}
	else
	{
		frame->Data[2] = 0x0;
		frame->Data[3] = 0x0;
		
		frame->Data[2] = PORT&0xFF;
	}
}

void WriteDigitalIo(ProtocolFrame *frame)
{
	if(frame->DataLength <=2)
	{
		PORT = frame->Data[0];
	}
	else
	{
		PORT = frame->Data[2];
	}
}

void WriteAnalogIo(ProtocolFrame *frame)
{
	int value = frame->Data[0];
	DAC_SetVoltage(DAC0, (value/63.0)*5);
}

void ReadAnalogIo(ProtocolFrame *frame)
{
	int value;
	value = DAC0L; // First 8 bits
	value = (value&0x00FF)|(DAC0H<<8);
	value = (int)(((1.0*value)/MAX_REG_VAL)*63);
	frame->Data[0] = value&0xFF;
	frame->Data[1] = (value>>8)&0xFF;
}



void main(void)
{
	uchar READY_TO_WORK = 0;
	
	ProtocolFrame inputFrame;
	
	PORT = 0x00; //wyjscia
	UART_Init();
	DAC_Init(DAC0);

	// Change id before start
	inputFrame.SlaveId = SLAVE_ID;
	
	while(1)
	{
		if(READ_READY)
		{
			EA = INTERRUPT_DISABLE;
			READY_TO_WORK = SLAVE_ReceiveData(&inputFrame);
		}
		if(READY_TO_WORK)
		{
			READY_TO_WORK  = 0;
			switch(CalculateEnumValue(inputFrame.Command))
			{
				case SA:
					WriteAnalogIo(&inputFrame);
					ReadAnalogIo(&inputFrame);
					SLAVE_SendData(&inputFrame);
					break;
				case SD:
					WriteDigitalIo(&inputFrame);
					ReadDigitalIo(&inputFrame);
					SLAVE_SendData(&inputFrame);
					break;
				case AS:
					WriteAnalogIo(&inputFrame);
					ReadAnalogIo(&inputFrame);
					WriteDigitalIo(&inputFrame);
					ReadDigitalIo(&inputFrame);
					SLAVE_SendData(&inputFrame);
					break;
				case ID:
					inputFrame.DataLength = 4;
					inputFrame.SlaveId = SLAVE_ID;
					ReadAnalogIo(&inputFrame);
					ReadDigitalIo(&inputFrame);
					SLAVE_SendData(&inputFrame);
					break;
			}
		}
		if(EA == 0)
			EA = INTERRUPT_ENABLE;
	}
}
