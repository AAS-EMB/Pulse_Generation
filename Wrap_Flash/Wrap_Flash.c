/*
	@file    Wrap_Flash.c
	@author  Anosov A.S.
	@date    06.06.2019
	@version 1.0.0
	@brief   Файл модуля-обертки для модуля работы с памятью
*/

#include "Wrap_Flash.h"

volatile FlashMapData_t GlobalStorage;
extern volatile uint8_t DiscretLineState[DISCRETS_OUTPUTS_PINS];
static FlashStorage_t GlobalStorageDescr = {&GlobalStorage, 1, 1};

/*!
 * Writing the structure to internal Flash with CRC verification
 *
 * \param[IN] pStorage  	Structure pointer
 * \retval           		true - successful entry
 * 							false - error write
 */
static uint8_t WriteInternalStorage(FlashMapData_t *pStorage)
{
	FlashMapData_t* TmpStorage;

	FlashWriteStructe(FLASH_DATA_USER_ADDRESS, pStorage);

	TmpStorage = (FlashMapData_t*) FlashReadStructe(FLASH_DATA_USER_ADDRESS);

	if (CheckIntegritiesStructe(TmpStorage) != FLASH_STATUS_ERROR_CRC)
		return true;
	else
		return false;
}

/*!
 * Filling the global storage with default values
 *
 * \retval           		true - storage filling default values
 *
 */
static uint8_t UseDefaultGlobalStorage(void)
{
	uint32_t crc = CRC_INI_VAL;

	DiscretsInitPins();

	memset(DiscretLineState, 1, DISCRETS_OUTPUTS_PINS);
	for(uint8_t NumberLine = 0; NumberLine < DISCRETS_OUTPUTS_PINS; NumberLine++)
	{
		GlobalStorage.DiscretLine[NumberLine].OutPeriodDiscret = DISCRETS_OUT_PERIOD;
		GlobalStorage.DiscretLine[NumberLine].OutPulseDiscret = DISCRETS_OUT_PULSE;
		GlobalStorage.DiscretLine[NumberLine].DelayStep1 = DISCRETS_OUT_PERIOD*DISCRETS_OUT_PULSE/100;
		GlobalStorage.DiscretLine[NumberLine].DelayStep0 = DISCRETS_OUT_PERIOD - DISCRETS_OUT_PERIOD*DISCRETS_OUT_PULSE/100;
		GlobalStorage.DiscretLine[NumberLine].QuantityStateLine = 0;
	}
	GlobalStorage.Checksum = ComputeChecksum(crc, (void*) (&GlobalStorage), FLASH_STRUCT_USER_SIZE - 4);

	return true;
}

/*!
 * The storage is checked to see if it is empty
 *
 * \param[IN] pStorage  	Structure pointer
 * \retval           		true - storage not empty
 * 							false - storage empty
 */
static uint8_t IsStorageNotEmpty(const FlashMapData_t *pStorage)
{
	if(pStorage->Checksum != 0xFFFFFFFF)
		return true;
	else
		return false;
}

/*!
 * Initializing the storage
 *
 * \retval           		Status of the operation
 */
FlashErrCode FlashStorageInit(void)
{
	FlashMapData_t* int_storage = NULL;
	FlashErrCode error_code = FLASH_MODULE_OK;

	int_storage = (FlashMapData_t*) FlashReadStructe(FLASH_DATA_USER_ADDRESS);

	if (IsStorageNotEmpty(int_storage))
	{
		if(CheckIntegritiesStructe(int_storage) != FLASH_STATUS_ERROR_CRC)
			GlobalStorage = *int_storage;
		else
		{
			UseDefaultGlobalStorage();

			if(WriteInternalStorage(&GlobalStorage) != 1)
			{
				error_code |= FLASH_MODULE_INT_STRG_WRITE_ERR;
			}
		}
	}
	else
	{
		error_code |= FLASH_MODULE_INT_STRG_WAS_EMPTY;

		UseDefaultGlobalStorage();

		if(WriteInternalStorage(&GlobalStorage) != 1)
		{
			error_code |= FLASH_MODULE_INT_STRG_WRITE_ERR;
		}

	}

	return error_code;
}

/*!
 * Writing a global structure to storage
 *
 * \retval           		Status of the operation
 */
FlashErrCode FlashWriteStorage(void)
{
	FlashErrCode error_code = FLASH_MODULE_OK;
	uint32_t crc = CRC_INI_VAL;

	GlobalStorageDescr.pGlobalStorage->Checksum = ComputeChecksum(crc, (void*) (&GlobalStorage), FLASH_STRUCT_USER_SIZE - 4);

	if (WriteInternalStorage(GlobalStorageDescr.pGlobalStorage) != 1)
	{
		error_code |= FLASH_MODULE_INT_STRG_WRITE_ERR;
	}

	return error_code;
}

/*!
 * Reading a structure from storage
 *
 * \retval           		Structure pointer
 */
FlashMapData_t* FlashReadStorage(void)
{
	return GlobalStorageDescr.pGlobalStorage;
}
