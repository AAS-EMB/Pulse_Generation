/*!
 * \file      Parser.c
 *
 * \brief     Parser commands
 *
 * \author    Anosov Anton
 */

#include "Parser.h"

extern Fifo_t UartTx;
char Command[COMMAND_PARSER_LINES][CONNAND_PARSER_COLUMN];
uint8_t NumberParser;
static uint8_t CounterLines = 0, CounterColumns = 0;
extern volatile bool IsDiscretInit, IsStateQuantity;
extern volatile FlashMapData_t GlobalStorage;

/*!
 * \brief Translate str to int
 */
static int StrToInt(char *str)
{
	uint8_t i;
	int dig = 0;

	for(i = 0; i < strlen(str); i++)
		if(str[i] >= '0' && str[i] <= '9')
			dig = dig*10 + str[i] - 0x30;
		else
			return -1;

	return dig;
}

/*!
 * \brief Parse received command
 */
void CommandParser(void)
{
	if(!strcmp("reset", Command[0]))
	{
		CommandResetBoard();
		ResetParser();
	}
	else if(!strcmp("help", Command[0]))
	{
		CommandHelp();
		ResetParser();
	}
	else if(!strcmp("en_channel", Command[0]))
	{
		uint8_t Channel;

		if(NumberParser != 2 || NumberParser > 2)
		{
			uint8_t TransmitBuffer[MAX_LENGHT_COMMAND];

			sprintf(TransmitBuffer,"\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			sprintf(TransmitBuffer,"en_channel: error format of the command!\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			sprintf(TransmitBuffer,"Usage: en_channel [(int)channel]. To set/read parameters for all channels use channel = 16!\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			sprintf(TransmitBuffer,"\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			UartSend();
			ResetParser();
			return;
		}

		Channel = StrToInt(Command[1]);
		CommandEnableChannel(Channel);
		ResetParser();
	}
	else if(!strcmp("dis_channel", Command[0]))
	{
		uint8_t Channel;

		if(NumberParser != 2 || NumberParser > 2)
		{
			uint8_t TransmitBuffer[MAX_LENGHT_COMMAND];

			sprintf(TransmitBuffer,"\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			sprintf(TransmitBuffer,"dis_channel: error format of the command!\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			sprintf(TransmitBuffer,"Usage: dis_channel [(int)channel]. To set/read parameters for all channels use channel = 16!\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			sprintf(TransmitBuffer,"\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			UartSend();
			ResetParser();
			return;
		}

		Channel = StrToInt(Command[1]);
		CommandDisableChannel(Channel);
		ResetParser();
	}
	else if(!strcmp("set_pulse", Command[0]))
	{
		uint8_t Channel, Pulse;

		if(NumberParser != 3 || NumberParser > 3)
		{
			uint8_t TransmitBuffer[MAX_LENGHT_COMMAND];

			sprintf(TransmitBuffer,"\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			sprintf(TransmitBuffer,"set_pulse: error format of the command!\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			sprintf(TransmitBuffer,"Usage: set_pulse [(int)channel] [(int)pulse] %%. To set/read parameters for all channels use channel = 16!\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			sprintf(TransmitBuffer,"\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			UartSend();
			ResetParser();
			return;
		}

		Channel = StrToInt(Command[1]);
		Pulse = StrToInt(Command[2]);
		CommandSetPulseChannel(Channel, Pulse);
		ResetParser();
	}
	else if(!strcmp("set_period", Command[0]))
	{
		uint8_t Channel;
		uint32_t Period;

		if(NumberParser != 3 || NumberParser > 3)
		{
			uint8_t TransmitBuffer[MAX_LENGHT_COMMAND];

			sprintf(TransmitBuffer,"\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			sprintf(TransmitBuffer,"set_period: error format of the command!\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			sprintf(TransmitBuffer,"Usage: set_period [(int)channel] [(int)period] ms. To set/read parameters for all channels use channel = 16!\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			sprintf(TransmitBuffer,"\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			UartSend();
			ResetParser();
			return;
		}

		Channel = StrToInt(Command[1]);
		Period = StrToInt(Command[2]);
		CommandSetPeriodChannel(Channel, Period);
		ResetParser();
	}
	else if(!strcmp("start", Command[0]))
	{
		IsDiscretInit = false;
		for(uint8_t NumberLine = 0; NumberLine < DISCRETS_OUTPUTS_PINS; NumberLine++)
		{
			if(GlobalStorage.DiscretLine[NumberLine].IsDiscretLineEnable)
			{
				IsDiscretInit = true;
				break;
			}
		}
		if(!IsDiscretInit)
		{
			uint8_t TransmitBuffer[MAX_LENGHT_COMMAND];

			sprintf(TransmitBuffer,"\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			sprintf(TransmitBuffer,"Discret channel's not initialization!\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			sprintf(TransmitBuffer,"\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			UartSend();
			ResetParser();
			return;
		}
		else if(!IsStateQuantity)
		{
			uint8_t TransmitBuffer[MAX_LENGHT_COMMAND];

			sprintf(TransmitBuffer,"\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			sprintf(TransmitBuffer,"Quantity impulse not set!\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			sprintf(TransmitBuffer,"\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			UartSend();
			ResetParser();
			return;
		}
		CommandStartMeasurement();
		ResetParser();
	}
	else if(!strcmp("stop", Command[0]))
	{
		CommandStopMeasurement();
		ResetParser();
	}
	else if(!strcmp("check_impulse", Command[0]))
	{
		uint8_t Channel;

		if(NumberParser != 2 || NumberParser > 2)
		{
			uint8_t TransmitBuffer[MAX_LENGHT_COMMAND];

			sprintf(TransmitBuffer,"\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			sprintf(TransmitBuffer,"check_impulse: error format of the command!\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			sprintf(TransmitBuffer,"Usage: check_impulse [(int)channel]. To set/read parameters for all channels use channel = 16!\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			sprintf(TransmitBuffer,"\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			UartSend();
			ResetParser();
			return;
		}

		Channel = StrToInt(Command[1]);
		CommandCheckImpulseChannel(Channel);
		ResetParser();
	}
	else if(!strcmp("reset_impulse", Command[0]))
	{
		uint8_t Channel;

		if(NumberParser != 2 || NumberParser > 2)
		{
			uint8_t TransmitBuffer[MAX_LENGHT_COMMAND];

			sprintf(TransmitBuffer,"\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			sprintf(TransmitBuffer,"reset_impulse: error format of the command!\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			sprintf(TransmitBuffer,"Usage: reset_impulse [(int)channel]. To set/read parameters for all channels use channel = 16!\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			sprintf(TransmitBuffer,"\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			UartSend();
			ResetParser();
			return;
		}

		Channel = StrToInt(Command[1]);
		CommandResetImpulseChannel(Channel);
		ResetParser();
	}
	else if(!strcmp("set_impulse", Command[0]))
	{
		uint8_t Channel;
		uint64_t Quantity = 0;

		if(NumberParser != 3 || NumberParser > 3)
		{
			uint8_t TransmitBuffer[MAX_LENGHT_COMMAND];

			sprintf(TransmitBuffer,"\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			sprintf(TransmitBuffer,"set_impulse: error format of the command!\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			sprintf(TransmitBuffer,"Usage: set_impulse [(int)channel] [(int)quantity_impulse]. To set/read parameters for all channels use channel = 16!\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			sprintf(TransmitBuffer,"\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			UartSend();
			ResetParser();
			return;
		}

		Channel = StrToInt(Command[1]);
		Quantity = StrToInt(Command[2]);
		CommandSetImpulseChannel(Channel, Quantity);
		ResetParser();
	}
	else if(!strcmp("check_param", Command[0]))
	{
		uint8_t Channel;

		if(NumberParser != 2 || NumberParser > 2)
		{
			uint8_t TransmitBuffer[MAX_LENGHT_COMMAND];

			sprintf(TransmitBuffer,"\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			sprintf(TransmitBuffer,"check_param: error format of the command!\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			sprintf(TransmitBuffer,"Usage: check_param [(int)channel]. To set/read parameters for all channels use channel = 16!\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			sprintf(TransmitBuffer,"\n");
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
			UartSend();
			ResetParser();
			return;
		}

		Channel = StrToInt(Command[1]);
		CommandCheckParams(Channel);
		ResetParser();
	}
}

/*!
 * \brief Parse received buffer
 *
 * \param [IN] symbol  Received symbol
 */
void ParseSymbols(uint8_t symbol)
{
	if(symbol == ASCII_COMMAND_LF)
		CommandParser();
	else if(symbol != ASCII_COMMAND_BACKSPACE)
	{
		if(CounterColumns < CONNAND_PARSER_COLUMN)
		{
			Command[CounterLines][CounterColumns] = symbol;
			CounterColumns++;
		}
		else
			CounterColumns = 0;
	}
	else
	{
		CounterLines++;

		if(CounterLines == COMMAND_PARSER_LINES)
			NumberParser = CounterLines;
		else
			CounterColumns = 0;

		NumberParser = CounterLines + 1;
	}
}

/*!
 * \brief Parse reset
 */
void ResetParser(void)
{
	CounterLines = 0;
	CounterColumns = 0;
	NumberParser = 0;
	memset(Command, 0, sizeof(Command));
}
