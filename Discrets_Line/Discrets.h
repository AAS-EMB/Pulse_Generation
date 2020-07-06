/*!
 * \file      Discrets.h
 *
 * \brief     Discrets outputs config
 *
 * \author    Anosov Anton
 */
#ifndef __DISCRETS_H__
#define __DISCRETS_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>
#include "stm32f1xx.h"

typedef struct DiscretLineDescription_s
{
	GPIO_TypeDef 	*pGpioPort;
	uint16_t		GpioPin;
	uint32_t 		OutPeriodDiscret;
	uint8_t 		OutPulseDiscret;
	uint32_t 		DelayStep0;
	uint32_t 		DelayStep1;
	uint32_t 		QuantityStateLine;
	uint8_t 		IsDiscretLineEnable:1;
	uint8_t			IsInitDiscretLines:1;
}DiscretLineDescription_t;

/*!
 * \brief Discrets line number
 */
typedef enum NumberDiscretLine_e
{
	NUMBER_DISCRETS_LINE_0 = 0,
	NUMBER_DISCRETS_LINE_1,
	NUMBER_DISCRETS_LINE_2,
	NUMBER_DISCRETS_LINE_3,
	NUMBER_DISCRETS_LINE_4,
	NUMBER_DISCRETS_LINE_5,
	NUMBER_DISCRETS_LINE_6,
	NUMBER_DISCRETS_LINE_7,
	NUMBER_DISCRETS_LINE_8,
	NUMBER_DISCRETS_LINE_9,
	NUMBER_DISCRETS_LINE_10,
	NUMBER_DISCRETS_LINE_11,
	NUMBER_DISCRETS_LINE_12,
	NUMBER_DISCRETS_LINE_13,
	NUMBER_DISCRETS_LINE_14,
	NUMBER_DISCRETS_LINE_15,
	NUMBER_DISCRETS_LINE_ALL
}NumberDiscretLine_t;

/*!
 * @brief Init discret lines
 */
void DiscretsInitPins(void);

/*!
 * @brief Write state discret line
 *
 * @param NumberLine		Number discret line
 * @param value				Line state
 */
void DiscretsWriteLines(NumberDiscretLine_t NumberLine, GPIO_PinState value);

/*!
 * @brief Toggle state discret line
 *
 * @param NumberLine		Number discret line
 */
void DiscretsToggleLines(NumberDiscretLine_t NumberLine);

#ifdef __cplusplus
}
#endif
#endif /* __DISCRETS_H__ */
