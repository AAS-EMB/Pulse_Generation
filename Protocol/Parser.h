/*!
 * \file      Parser.h
 *
 * \brief     Parser commands
 *
 * \author    Anosov Anton
 */

#ifndef __PARSER_H_
#define __PARSER_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include "string.h"
#include "stdint.h"
#include "Wrap_Flash.h"
#include "Commands.h"

// Количество строк массива
#define COMMAND_PARSER_LINES		(5)

// Количество столбцов
#define CONNAND_PARSER_COLUMN		(20)

#define ASCII_COMMAND_BACKSPACE		(32)

#define ASCII_COMMAND_LF			(10)

/*!
 * \brief Parse received buffer
 *
 * \param [IN] symbol  Received symbol
 */
void ParseSymbols(uint8_t symbol);

/*!
 * \brief Parse reset
 */
void ResetParser(void);

/*!
 * \brief Parse received command
 */
void CommandParser(void);

#ifdef __cplusplus
}
#endif
#endif /* __PARSER_H_ */
