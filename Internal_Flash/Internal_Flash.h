/*!
 * \file      Internal_Flash.h
 *
 * \brief     Internal flash module
 *
 * \author    Anosov Anton
 */

#ifndef INTERNAL_FLASH_H_
#define INTERNAL_FLASH_H_
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx.h"
#include "Discrets.h"

// Maximum number of digital outputs
#define DISCRETS_OUTPUTS_PINS		16

/*!
 * Default crc ini val
 */
#define CRC_INI_VAL 						0xFFFFFFFFL

/*!
 * Getting the table value of the checksum
 */
#define CRC32_NEXT(CRC, c) 					(CRC32_Table[(((uint8_t)(CRC)) ^ (c))&0xFF] ^ ((CRC) >> 8))

/*!
 * Flash timeout
 */
#define MAX_FLASH_TIMEOUT					(50000U)

 /*!
  * Flash map parametrs
  */
 #define FLASH_PAGE_SIZES					1024
 #define FLASH_START_ADDRESS				0x08000000
 #define FLASH_MAX_SIZE						0x00010000
 #define FLASH_DATA_USER_ADDRESS            0x0800C800
 #define FLASH_END_ADDRESS					(FLASH_START_ADDRESS + FLASH_MAX_SIZE)

 /*!
  * Flash structe user size
  */
#define FLASH_STRUCT_USER_SIZE				(sizeof(FlashMapData_t))

/*!
 * Flash map data struct with CRC32
 */
typedef struct FlashMapData_s
{
	/*!
	 * Desription discret lines
	 */
	DiscretLineDescription_t DiscretLine[DISCRETS_OUTPUTS_PINS];

    /*!
     * Checksum to check the integrity of the structure
     */
	uint32_t Checksum;

}FlashMapData_t;

/*!
 * Flash status enum
 */
typedef enum FlashStatus_e
{
    /*!
     * No error occurred
     */
	FLASH_STATUS_OK = 0,

    /*!
     * Error unlock flash
     */
	FLASH_STATUS_ERROR_UNLOCK,

    /*!
     * Error lock flash
     */
	FLASH_STATUS_ERROR_LOCK,

    /*!
     * Error write flash
     */
	FLASH_STATUS_ERROR_WRITE,

    /*!
     * Error erase flash
     */
	FLASH_STATUS_ERROR_ERASE,

    /*!
     * Checksum of the data block does not match.
     */
	FLASH_STATUS_ERROR_CRC

}FlashStatus_t;

/*!
 * \brief Initialization flash
 *
 * \retval           		Status of the operation
 */
FlashStatus_t FlashInit(void);

/*!
 * \brief Reads data at the specified address
 *
 * \param[IN] Address  		Base address
 * \retval           		Data
 */
uint32_t FlashReadData(uint32_t Address);

/*!
 * \brief Reads structe data at the specified address
 *
 * \param[IN] Address  		Base address
 * \retval           		Data structe pointer
 */
FlashMapData_t* FlashReadStructe(uint32_t Address);

/*!
 * \brief Erase page flash
 *
 * \param[IN] Address  		Base address
 * \retval           		Status of the operation
 */
FlashStatus_t FlashErasePage(uint32_t Address);

/*!
 * \brief Write data to flash
 *
 * \param[IN] Address  		Base address
 * \param[IN] pData  		Data pointer
 * \param[IN] Size  		Data size
 * \retval           		Status of the operation
 */
FlashStatus_t FlashWriteData(uint32_t Address, uint32_t *pData, uint32_t Size);

/*!
 * \brief Writes structe data at the specified address
 *
 * \param[IN] Address  		Base address
 * \param[IN] pStruct  		Data structe pointer
 * \retval           		Status of the operation
 */
FlashStatus_t FlashWriteStructe(uint32_t Address, FlashMapData_t *pStruct);

/*!
 * \brief Calculating the checksum (CRC32)
 * using the polynomial x^32 + x^26 + x^23 + x^22 + x^16 + x^12 + x^11 + x^10 + x^8 + x^7 + x^5 + x^4 + x^2 + x + 1
 *
 * \param[IN] Crc  			Default crc ini val
 * \param[IN] pData  		Data pointer
 * \param[IN] Size  		Data size
 * \retval           		The value of the checksum
 */
uint32_t ComputeChecksum(uint32_t Crc, void* pData, uint32_t Size);

/*!
 * \brief Check integrities structe with use checksum
 *
 * \param[IN] pStruct  		Data structe pointer
 * \retval           		Status of the operation
 */
FlashStatus_t CheckIntegritiesStructe(FlashMapData_t *pStruct);

#ifdef __cplusplus
}
#endif
#endif /* INTERNAL_FLASH_H_ */
