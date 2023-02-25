
/*******************************************************************************
 * @file    BOOT_PROCESS.h
 * @author  Mohammed Khaled
 * @email   Mohammed.kh384@gmail.com
 * @website EMSTutorials.blogspot.com/
 * @Created on: Feb 24, 2023
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


#ifndef INC_BOOT_PROCESS_H_
#define INC_BOOT_PROCESS_H_


/*
 * Includes:
 */
#include "BOOT_CNTRL.h"
#include "BOOT_Info.h"



/**
 * @addtogroup PROCESS_Examples
 * @{
 */

/**
 * @addtogroup PROCESS_Global_Vars
 * @{
 */

/**
 * @defgroup PROCESS_Exported_Typedefs
 * @{
 */

typedef uint32_t AddressType;

/**
 * @}
 */


/**
 * @}
 */

/**
 * @defgroup PROCESS_Exported_Macros
 * @{
 */

/**
 * @}
 */

/**
 * @defgroup PROCESS_Exported_Constants
 * @{
 */

/**
 * @}
 */

/**
 * @defgroup PROCESS_Exported_Functions  @Group1
 * @{
 */

void PROCESS_GET_CMD			(void);

void PROCESS_FLASH_UNLOCK_CMD	(void);
void PROCESS_FLASH_LOCK_CMD		(void);
void PROCESS_FLASH_PROG_CMD		(void);
void PROCESS_FLASH_READ_CMD		(void);
void PROCESS_FLASH_ERASE_CMD	(void);
void PROCESS_FLASH_CPY_CMD		(void);

// FOR FUTURE VERSION
void PROCESS_FLASH_CHECK_CMD	(void);
void PROCESS_MASS_ERASE_CMD		(void);

/**
 * @}
 */


/**
 * @defgroup PROCESS_Exported_Functions  @Group2
 * @{
 */
void PROCESS_OB_UNLOCK_CMD	(void);
void PROCESS_OB_LOCK_CMD	(void);
void PROCESS_OB_READ_CMD	(void);

// WRITE PROTECTION PROCESS
void PROCESS_PROTECT_CMD	(void);
void PROCESS_UNPROTECT_CMD	(void);


/**
 * @}
 */


/**
 * @defgroup PROCESS_Exported_Functions  @Group3
 * @{
 */

void PROCESS_ERROR		(void);
void PROCESS_COMPLETE	(void);

/**
 * @}
 */









#endif /* INC_BOOT_PROCESS_H_ */
