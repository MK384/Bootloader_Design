
/*******************************************************************************
 * @file    BOOT_CNTRL.h
 * @author  Mohammed Khaled
 * @email   Mohammed.kh384@gmail.com
 * @website EMSTutorials.blogspot.com/
 * @Created on: Feb 21, 2023
 *
 * @brief   this header file contains the declarations of the boot control APIs.
 * @note
 *
@verbatim
Copyright (C) EMSTutorials, 2019

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
@endverbatim
*******************************************************************************/


#ifndef INC_BOOT_CNTRL_H_
#define INC_BOOT_CNTRL_H_


/*
 * Includes:
 */
#include "stm32f4xx.h"
#include "stdint.h"



/**
 * @addtogroup
 * @{
 */

/**
 * @addtogroup
 * @{
 */

/**
 * @defgroup flash_exported_typedefs
 * @{
 */

/**
 * @}
 */

/**
 * @brief   FLASH states definition
 */


/**
 * @defgroup FLASHEx_Sectors FLASH Sectors
 * @{
 */

#define FLASH_SECTOR_0     0U  /*!< Sector Number 0   */
#define FLASH_SECTOR_1     1U  /*!< Sector Number 1   */
#define FLASH_SECTOR_2     2U  /*!< Sector Number 2   */
#define FLASH_SECTOR_3     3U  /*!< Sector Number 3   */
#define FLASH_SECTOR_4     4U  /*!< Sector Number 4   */
#define FLASH_SECTOR_5     5U  /*!< Sector Number 5   */
#define FLASH_SECTOR_6     6U  /*!< Sector Number 6   */
#define FLASH_SECTOR_7     7U  /*!< Sector Number 7   */
#define FLASH_SECTOR_8     8U  /*!< Sector Number 8   */
#define FLASH_SECTOR_9     9U  /*!< Sector Number 9   */
#define FLASH_SECTOR_10    10U /*!< Sector Number 10  */
#define FLASH_SECTOR_11    11U /*!< Sector Number 11  */
#define FLASH_SECTOR_12    12U /*!< Sector Number 12  */
#define FLASH_SECTOR_13    13U /*!< Sector Number 13  */
#define FLASH_SECTOR_14    14U /*!< Sector Number 14  */
#define FLASH_SECTOR_15    15U /*!< Sector Number 15  */
#define FLASH_SECTOR_16    16U /*!< Sector Number 16  */
#define FLASH_SECTOR_17    17U /*!< Sector Number 17  */
#define FLASH_SECTOR_18    18U /*!< Sector Number 18  */
#define FLASH_SECTOR_19    19U /*!< Sector Number 19  */
#define FLASH_SECTOR_20    20U /*!< Sector Number 20  */
#define FLASH_SECTOR_21    21U /*!< Sector Number 21  */
#define FLASH_SECTOR_22    22U /*!< Sector Number 22  */
#define FLASH_SECTOR_23    23U /*!< Sector Number 23  */


/**
 * @}
 */

/**
 * @defgroup flash_exported_macros
 * @{
 */

/**
 * @}
 */
/**
 * @defgroup Exported_Types
 * @{
 */

typedef uint32_t FLASH_ErrorCode;

/**
 * @}
 */

/**
 * @defgroup flash_exported_constants
 * @{
 */

/**
 * @}
 */

/**
 * @defgroup boot_exported_functions
 * @{
 */

 	/*This function shouldn't return, if returned the transfer control has failed*/
	void BOOT_TRANSFER_CNTRL(uint32_t ImageAddress);

	/*Copy Image from Source location to destination location.*/
FLASH_ErrorCode BOOT_CPY_IMAGE(uint32_t srcAddress ,uint32_t destAddress, uint32_t size);






/**
 * @}
 */
/**
 * @}
 */
/**
 * @}
 */

#endif /* INC_BOOT_CNTRL_H_ */
