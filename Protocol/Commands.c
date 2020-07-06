/*!
 * \file      Commands.c
 *
 * \brief     Application functions for working with the device
 *
 * \author    Anosov Anton
 */

#include "Commands.h"

extern Fifo_t UartTx;
volatile bool IsDiscretInit, IsStateQuantity, IsStartMeasurement;
extern volatile FlashMapData_t GlobalStorage;
extern volatile uint32_t CounterSemaphore[];
extern volatile uint32_t CounterStateSemaphore[];
//extern volatile uint64_t UserQuantityStateLine[];

/*!
 * \brief Application function command "Help"
 */
void CommandHelp(void)
{
	uint8_t TransmitBuffer[MAX_LENGHT_COMMAND];

	sprintf(TransmitBuffer,"\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	sprintf(TransmitBuffer,"reset 		- command reset board\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	sprintf(TransmitBuffer,"en_channel 	- command enable discret channel's\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	sprintf(TransmitBuffer,"dis_channel 	- command disable discret channel's\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	sprintf(TransmitBuffer,"set_pulse 	- command set pulse (0 to 100 %%) discret channel's\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	sprintf(TransmitBuffer,"set_period 	- command set period (in ms) discret channel's\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	sprintf(TransmitBuffer,"start 		- command start testing discret channel's\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	sprintf(TransmitBuffer,"stop 		- command stop testing discret channel's\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	sprintf(TransmitBuffer,"check_impulse 	- command check quantity impulse discret channel's\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	sprintf(TransmitBuffer,"reset_impulse 	- command reset quantity impulse discret channel's\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	sprintf(TransmitBuffer,"set_impulse 	- command set quantity impulse discret channel's\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	sprintf(TransmitBuffer,"check_param 	- command check param's discret channel's\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	sprintf(TransmitBuffer,"\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	UartSend();
}

/*!
 * \brief Application function command "Reset board"
 */
void CommandResetBoard(void)
{
	uint8_t TransmitBuffer[MAX_LENGHT_COMMAND];

	sprintf(TransmitBuffer,"\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	sprintf(TransmitBuffer,"Board was reset!\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	sprintf(TransmitBuffer,"\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	FlashWriteStorage();
	UartSend();
	HAL_Delay(1000);
	NVIC_SystemReset();
}

/*!
 * \brief Application function command "Enable channel"
 */
void CommandEnableChannel(uint8_t channel)
{
	uint8_t TransmitBuffer[MAX_LENGHT_COMMAND];

	if(channel > DISCRETS_OUTPUTS_PINS + 1)
	{
		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"Error channel! Range 0 to 16. All channel - 16!\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		UartSend();
		return;
	}

	if(channel != NUMBER_DISCRETS_LINE_ALL)
	{
		GlobalStorage.DiscretLine[channel].IsDiscretLineEnable = 1;
		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"Channel %d enable!\n", channel);
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	}
	else
	{
		for(uint8_t NumberLine = 0; NumberLine < DISCRETS_OUTPUTS_PINS; NumberLine++)
			GlobalStorage.DiscretLine[NumberLine].IsDiscretLineEnable = 1;

		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"All channel's enable!\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	}

	FlashWriteStorage();
	sprintf(TransmitBuffer,"\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	UartSend();
}

/*!
 * \brief Application function command "Disable channel"
 */
void CommandDisableChannel(uint8_t channel)
{
	uint8_t TransmitBuffer[MAX_LENGHT_COMMAND];

	if(channel > DISCRETS_OUTPUTS_PINS + 1)
	{
		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"Error channel! Range 0 to 16. All channel - 16!\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		UartSend();
		return;
	}

	if(channel != NUMBER_DISCRETS_LINE_ALL)
	{
		GlobalStorage.DiscretLine[channel].IsDiscretLineEnable = 0;
		DiscretsWriteLines(channel, GPIO_PIN_SET);
		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"Channel %d disable!\n", channel);
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	}
	else
	{
		for(uint8_t NumberLine = 0; NumberLine < DISCRETS_OUTPUTS_PINS; NumberLine++)
		{
			GlobalStorage.DiscretLine[NumberLine].IsDiscretLineEnable = 0;
			DiscretsWriteLines(NumberLine, GPIO_PIN_SET);
		}

		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"All channel's disable!\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	}

	FlashWriteStorage();
	sprintf(TransmitBuffer,"\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	UartSend();
}

/*!
 * \brief Application function command "Start measurement"
 */
void CommandStartMeasurement(void)
{
	uint8_t TransmitBuffer[MAX_LENGHT_COMMAND];

	for(uint8_t NumberLine = 0; NumberLine < DISCRETS_OUTPUTS_PINS; NumberLine++)
	{
		CounterSemaphore[NumberLine] = 0;
		CounterStateSemaphore[NumberLine] = 0;
	}

	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim4);

	IsStartMeasurement = true;

	sprintf(TransmitBuffer,"\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	sprintf(TransmitBuffer,"Start measurement!\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	sprintf(TransmitBuffer,"\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	UartSend();
}

/*!
 * \brief Application function command "Stop measurement"
 */
void CommandStopMeasurement(void)
{
	uint8_t TransmitBuffer[MAX_LENGHT_COMMAND];

	HAL_TIM_Base_Stop_IT(&htim2);
	HAL_TIM_Base_Stop_IT(&htim4);

	for(uint8_t NumberLine = 0; NumberLine < DISCRETS_OUTPUTS_PINS; NumberLine++)
	{
		DiscretsWriteLines(NumberLine, GPIO_PIN_SET);
		CounterSemaphore[NumberLine] = 0;
		CounterStateSemaphore[NumberLine] = 0;
	}

	IsStartMeasurement = false;

	sprintf(TransmitBuffer,"\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	sprintf(TransmitBuffer,"Measurement stop!\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	sprintf(TransmitBuffer,"\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	UartSend();
}

/*!
 * \brief Application function command "Set pulse channel"
 */
void CommandSetPulseChannel(uint8_t channel, uint8_t pulse)
{
	uint8_t TransmitBuffer[MAX_LENGHT_COMMAND];

	if(pulse > 100)
	{
		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"Error set pulse! Range 0 to 100 %%\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		UartSend();
		return;
	}

	if(channel > DISCRETS_OUTPUTS_PINS + 1)
	{
		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"Error channel! Range 0 to 16. All channel - 16!\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		UartSend();
		return;
	}

	if(channel != NUMBER_DISCRETS_LINE_ALL)
	{
		CounterSemaphore[channel] = 0;
		CounterStateSemaphore[channel] = 0;
		GlobalStorage.DiscretLine[channel].OutPulseDiscret = pulse;
		GlobalStorage.DiscretLine[channel].DelayStep1 = GlobalStorage.DiscretLine[channel].OutPeriodDiscret*
														GlobalStorage.DiscretLine[channel].OutPulseDiscret/100;
		GlobalStorage.DiscretLine[channel].DelayStep0 = GlobalStorage.DiscretLine[channel].OutPeriodDiscret -
														GlobalStorage.DiscretLine[channel].DelayStep1;
		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"Channel %d set pulse %d%%\n", channel, pulse);
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	}
	else
	{
		for(uint8_t NumberLine = 0; NumberLine < DISCRETS_OUTPUTS_PINS; NumberLine++)
		{
			CounterSemaphore[NumberLine] = 0;
			CounterStateSemaphore[NumberLine] = 0;
			GlobalStorage.DiscretLine[NumberLine].OutPulseDiscret = pulse;
			GlobalStorage.DiscretLine[NumberLine].DelayStep1 = GlobalStorage.DiscretLine[NumberLine].OutPeriodDiscret*
															   GlobalStorage.DiscretLine[NumberLine].OutPulseDiscret/100;
			GlobalStorage.DiscretLine[NumberLine].DelayStep0 = GlobalStorage.DiscretLine[NumberLine].OutPeriodDiscret -
															   GlobalStorage.DiscretLine[NumberLine].DelayStep1;
		}

		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"All channel's set pulse %d%%\n", pulse);
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	}

	FlashWriteStorage();
	sprintf(TransmitBuffer,"\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	UartSend();
}

/*!
 * \brief Application function command "Set period channel"
 */
void CommandSetPeriodChannel(uint8_t channel, uint32_t period)
{
	uint8_t TransmitBuffer[MAX_LENGHT_COMMAND];

	if(channel > DISCRETS_OUTPUTS_PINS + 1)
	{
		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"Error channel! Range 0 to 16. All channel - 16!\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		UartSend();
		return;
	}

	if(channel != NUMBER_DISCRETS_LINE_ALL)
	{
		CounterSemaphore[channel] = 0;
		CounterStateSemaphore[channel] = 0;
		GlobalStorage.DiscretLine[channel].OutPeriodDiscret = period;
		GlobalStorage.DiscretLine[channel].DelayStep1 = period*GlobalStorage.DiscretLine[channel].OutPulseDiscret/100;
		GlobalStorage.DiscretLine[channel].DelayStep0 = period - GlobalStorage.DiscretLine[channel].DelayStep1;
		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"Channel %d set period %d ms\n", channel, period);
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	}
	else
	{
		for(uint8_t NumberLine = 0; NumberLine < DISCRETS_OUTPUTS_PINS; NumberLine++)
		{
			CounterSemaphore[NumberLine] = 0;
			CounterStateSemaphore[NumberLine] = 0;
			GlobalStorage.DiscretLine[NumberLine].OutPeriodDiscret = period;
			GlobalStorage.DiscretLine[NumberLine].DelayStep1 = period*GlobalStorage.DiscretLine[NumberLine].OutPulseDiscret/100;
			GlobalStorage.DiscretLine[NumberLine].DelayStep0 = period - GlobalStorage.DiscretLine[NumberLine].DelayStep1;
		}

		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"All channel's set period %d ms\n", period);
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	}

	FlashWriteStorage();
	sprintf(TransmitBuffer,"\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	UartSend();
}

/*!
 * \brief Application function command "Check impulse channel"
 */
void CommandCheckImpulseChannel(uint8_t channel)
{
	uint8_t TransmitBuffer[MAX_LENGHT_COMMAND];

	if(channel > DISCRETS_OUTPUTS_PINS + 1)
	{
		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"Error channel! Range 0 to 16. All channel - 16!\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		UartSend();
		return;
	}

	if(channel != NUMBER_DISCRETS_LINE_ALL)
	{
		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"Channel %d impulse %d\n", channel, CounterStateSemaphore[channel]);
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	}
	else
	{
		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));

		for(uint8_t NumberLine = 0; NumberLine < DISCRETS_OUTPUTS_PINS; NumberLine++)
		{
			sprintf(TransmitBuffer,"Channel %d impulse %d\n", NumberLine, CounterStateSemaphore[NumberLine]);
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		}
	}

	sprintf(TransmitBuffer,"\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	UartSend();
}

/*!
 * \brief Application function command "Reset impulse channel"
 */
void CommandResetImpulseChannel(uint8_t channel)
{
	uint8_t TransmitBuffer[MAX_LENGHT_COMMAND];

	if(channel > DISCRETS_OUTPUTS_PINS + 1)
	{
		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"Error channel! Range 0 to 16. All channel - 16!\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		UartSend();
		return;
	}

	if(channel != NUMBER_DISCRETS_LINE_ALL)
	{
		CounterStateSemaphore[channel] = 0;
		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"Channel %d reset impulse!\n", channel);
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	}
	else
	{
		for(uint8_t NumberLine = 0; NumberLine < DISCRETS_OUTPUTS_PINS; NumberLine++)
			CounterStateSemaphore[NumberLine] = 0;

		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"All channel's reset impulse!\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	}

	sprintf(TransmitBuffer,"\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	UartSend();
}

/*!
 * \brief Application function command "Reset impulse channel"
 */
void CommandSetImpulseChannel(uint8_t channel, uint64_t quantity)
{
	uint8_t TransmitBuffer[MAX_LENGHT_COMMAND];

	if(channel > DISCRETS_OUTPUTS_PINS + 1)
	{
		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"Error channel! Range 0 to 16. All channel - 16!\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		UartSend();
		return;
	}

	if(channel != NUMBER_DISCRETS_LINE_ALL)
	{
		CounterStateSemaphore[channel] = 0;
		GlobalStorage.DiscretLine[channel].QuantityStateLine = quantity;
		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"Channel %d set quantity impulse %d\n", channel, quantity);
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	}
	else
	{
		for(uint8_t NumberLine = 0; NumberLine < DISCRETS_OUTPUTS_PINS; NumberLine++)
		{
			CounterStateSemaphore[NumberLine] = 0;
			GlobalStorage.DiscretLine[NumberLine].QuantityStateLine = quantity;
		}

		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"All channel set quantity impulse %d\n", quantity);
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	}

	FlashWriteStorage();
	IsStateQuantity = true;
	sprintf(TransmitBuffer,"\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	UartSend();
}

/*!
 * Application function Check params
 */
void CommandCheckParams(uint8_t channel)
{
	uint8_t TransmitBuffer[MAX_LENGHT_COMMAND];

	if(channel > DISCRETS_OUTPUTS_PINS + 1)
	{
		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"Error channel! Range 0 to 16. All channel - 16!\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		UartSend();
		return;
	}

	if(channel != NUMBER_DISCRETS_LINE_ALL)
	{
		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		sprintf(TransmitBuffer,"Channel %d enable(1)/disable(0) %d period %d pulse %d%% impulse %d\n", channel, GlobalStorage.DiscretLine[channel].IsDiscretLineEnable, GlobalStorage.DiscretLine[channel].OutPeriodDiscret, GlobalStorage.DiscretLine[channel].OutPulseDiscret, GlobalStorage.DiscretLine[channel].QuantityStateLine);
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	}
	else
	{
		sprintf(TransmitBuffer,"\n");
		FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));

		for(uint8_t NumberLine = 0; NumberLine < DISCRETS_OUTPUTS_PINS; NumberLine++)
		{
			sprintf(TransmitBuffer,"Channel %d enable(1)/disable(0) %d period %d pulse %d%% impulse %d\n", NumberLine, GlobalStorage.DiscretLine[NumberLine].IsDiscretLineEnable, GlobalStorage.DiscretLine[NumberLine].OutPeriodDiscret, GlobalStorage.DiscretLine[NumberLine].OutPulseDiscret, GlobalStorage.DiscretLine[NumberLine].QuantityStateLine);
			FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
		}
	}

	sprintf(TransmitBuffer,"\n");
	FifoPutBuf(&UartTx, TransmitBuffer, strlen(TransmitBuffer));
	UartSend();
}

/*!
 * \brief Application function command "Start bounce"
 */
void CommandStartBounce(uint8_t channel)
{

}
