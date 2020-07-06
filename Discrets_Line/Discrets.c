/*!
 * \file      Discrets.c
 *
 * \brief     Discrets outputs config
 *
 * \author    Anosov Anton
 */

#include "Discrets.h"
#include "Wrap_Flash.h"

extern volatile FlashMapData_t GlobalStorage;

/*!
 * @brief Init discret lines
 */
void DiscretsInitPins(void)
{
	for(uint8_t NumberLine = 0; NumberLine < DISCRETS_OUTPUTS_PINS; NumberLine++)
	{
		if(GlobalStorage.DiscretLine[NumberLine].IsInitDiscretLines)
			continue;
		else
		{
			GlobalStorage.DiscretLine[NumberLine].pGpioPort = GPIOB;
			GlobalStorage.DiscretLine[NumberLine].GpioPin = 0x01 << NumberLine;
			GlobalStorage.DiscretLine[NumberLine].IsDiscretLineEnable = false;
			GlobalStorage.DiscretLine[NumberLine].IsInitDiscretLines = 1;
		}
	}
}

/*!
 * @brief Write state discret line
 *
 * @param NumberLine		Number discret line
 * @param value				Line state
 */
void DiscretsWriteLines(NumberDiscretLine_t NumberLine, GPIO_PinState value)
{
	if(NumberLine == NUMBER_DISCRETS_LINE_ALL)
		HAL_GPIO_WritePin(GlobalStorage.DiscretLine[NumberLine].pGpioPort, GPIO_PIN_All, value);
	else
		HAL_GPIO_WritePin(GlobalStorage.DiscretLine[NumberLine].pGpioPort, GlobalStorage.DiscretLine[NumberLine].GpioPin, value);
}

/*!
 * @brief Toggle state discret line
 *
 * @param NumberLine		Number discret line
 */
void DiscretsToggleLines(NumberDiscretLine_t NumberLine)
{
	if(NumberLine == NUMBER_DISCRETS_LINE_ALL)
		HAL_GPIO_TogglePin(GlobalStorage.DiscretLine[NumberLine].pGpioPort, GPIO_PIN_All);
	else
		HAL_GPIO_TogglePin(GlobalStorage.DiscretLine[NumberLine].pGpioPort, GlobalStorage.DiscretLine[NumberLine].GpioPin);
}
