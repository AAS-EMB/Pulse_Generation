/*
	@file    Wrap_Flash.h
	@author  Anosov A.S.
	@date    06.06.2019
	@version 1.0.0
	@brief   Файл модуля-обертки для модуля работы с памятью
*/

#ifndef WRAP_FLASH_H_
#define WRAP_FLASH_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "../Internal_Flash/Internal_Flash.h"
#include "Wrap_Flash_Cfg.h"
#include "stm32f1xx.h"
#include "Discrets.h"

/*!
 * Map macros state code
 */
#define FLASH_MODULE_OK							(0x00)
#define FLASH_MODULE_EXT_STRG_CORRUPTED			(0x01)
#define FLASH_MODULE_EXT_STRG_WAS_EMPTY			(0x02)
#define FLASH_MODULE_INT_STRG_WAS_EMPTY			(0x04)
#define FLASH_MODULE_EXT_STRG_WRITE_ERR			(0x08)
#define FLASH_MODULE_INT_STRG_WRITE_ERR			(0x10)
#define FLASH_MODULE_UNEXP_BHVR_ERR				(0x20)

/*!
 * Error flag
 */
typedef uint8_t FlashErrCode;

/*!
 * The repository structure
 */
typedef struct FlashStorage_s
{
	/*!
	 * Control structure
	 */
	FlashMapData_t *pGlobalStorage;

	/*!
	 * Bit flag field of the external structure of the firmware
	 */
	uint8_t IsExternalStorageOk:1;

	/*!
	 * Bit flag field of the internal structure of the firmware
	 */
	uint8_t IsInternalStorageOk:1;

}FlashStorage_t;

/*!
 * Initializing the storage
 *
 * \retval           		Status of the operation
 */
FlashErrCode FlashStorageInit(void);

/*!
 * Writing a global structure to storage
 *
 * \retval           		Status of the operation
 */
FlashErrCode FlashWriteStorage(void);

/*!
 * Reading a structure from storage
 *
 * \retval           		Structure pointer
 */
FlashMapData_t* FlashReadStorage(void);

#ifdef __cplusplus
}
#endif
#endif /* WRAP_FLASH_H_ */
