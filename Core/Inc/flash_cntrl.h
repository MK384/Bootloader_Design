
/*******************************************************************************
 * @file    flash_cntrl.h
 * @author  Mohammed Khaled
 * @email   Mohammed.kh384@gmail.com
 * @website EMSTutorials.blogspot.com/
 * @Created on: Feb 20, 2023
 *
 * @brief   this header file contains the declarations of the flash control APIs.
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

/* Define to prevent recursive inclusion */
#ifndef __INC_FLASH_H_
#define __INC_FLASH_H_

/* C++ detection */
#ifdef __cplusplus
extern "C"
{
#endif


/*
 * Includes:
 */
#include "stm32f4xx.h"

    /**
     * @addtogroup stm32_examples
     * @{
     */

    /**
     * @addtogroup flash
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
	typedef enum
	{
	  Ok_State,
	  Programming_sequence_error,
	  Programming_parallelism_error,
	  Programming_alignment_error,
	  Write_protection_error


	}ERR_StateType;

    /**
     * @defgroup flash_exported_defines
     * @{
     */

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
     * @defgroup flash_exported_constants
     * @{
     */

    /**
     * @}
     */

    /**
     * @defgroup flash_exported_functions
     * @{
     */
		 void Flash_Init(void);
		 void Flash_Lock(void);
		 void Flash_Unlock(void);

		 ERR_StateType Flash_Write(uint32_t address , uint32_t data);
		 ERR_StateType Flash_Read(uint32_t address , uint32_t* dataOut);
		 ERR_StateType Flash_erase(uint32_t sector_num);
		 ERR_StateType Flash_WRP_Enable(uint32_t sector_num);
		 ERR_StateType Flash_WRP_Desable(uint32_t sector_num);
		 ERR_StateType Flash_OB_Unlock(void);
		 ERR_StateType Flash_OB_Lock(void);


/**
 * @}
 */
/**
 * @}
 */
/**
 * @}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /*__INC_FLASH_H_ */
