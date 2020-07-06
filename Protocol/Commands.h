/*!
 * \file      Commands.c
 *
 * \brief     Application functions for working with the device
 *
 * \author    Anosov Anton
 */

#ifndef __COMMANDS_H_
#define __COMMANDS_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "Discrets.h"
#include "Wrap_Flash.h"
#include "Fifo.h"
#include "tim.h"

#define MAX_LENGHT_COMMAND		(256)

/*!
 * \brief Application function command "Help"
 */
void CommandHelp(void);

/*!
 * \brief Application function command "Reset board"
 */
void CommandResetBoard(void);

/*!
 * \brief Application function command "Menu"
 */
void CommandMenu(void);

/*!
 * \brief Application function command "Enable channel"
 */
void CommandEnableChannel(uint8_t channel);

/*!
 * \brief Application function command "Disable channel"
 */
void CommandDisableChannel(uint8_t channel);

/*!
 * \brief Application function command "Start measurement"
 */
void CommandStartMeasurement(void);

/*!
 * \brief Application function command "Stop measurement"
 */
void CommandStopMeasurement(void);

/*!
 * \brief Application function command "Set pulse channel"
 */
void CommandSetPulseChannel(uint8_t channel, uint8_t pulse);

/*!
 * \brief Application function command "Set period channel"
 */
void CommandSetPeriodChannel(uint8_t channel, uint32_t period);

/*!
 * \brief Application function command "Check impulse channel"
 */
void CommandCheckImpulseChannel(uint8_t channel);

/*!
 * \brief Application function command "Reset impulse channel"
 */
void CommandResetImpulseChannel(uint8_t channel);

/*!
 * \brief Application function command "Reset impulse channel"
 */
void CommandSetImpulseChannel(uint8_t channel, uint64_t quantity);

/*!
 * Application function Check params
 */
void CommandCheckParams(uint8_t channel);

#ifdef __cplusplus
}
#endif
#endif /* __COMMANDS_H_ */
