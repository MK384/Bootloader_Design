
/*******************************************************************************
 * @file    bootloader_info.c
 * @author  Mohammed Khaled
 * @email   Mohammed.kh384@gmail.com
 * @website EMSTutorials.blogspot.com/
 * @Created on: Feb 21, 2023
 *
 * @brief   this source file contains the implementation of the flash control APIs.
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

/**************** Includes ********************/
#include <stdlib.h>
#include <stdint.h>



/**
 * @addtogroup
 * @{
 */

/**
 * @defgroup BOOT_MAIN_INFO
 * @brief    Contains information about the boot loader id, version and the author
 * @{
 */
const char *ID = "0xFC10EA01  \n";
const char *VERSION = "v1.0   \n";
const char *AUTHOR = "Mohammed Khaled \n";

/**
 * @}
 */


/**
 * @defgroup BOOT_CMDS
 * @brief    Contains all the commands codes.
 * @{
 */

// To get the information of the bottloader
#define 		GET_CMD					(uint8_t)(0x00)
// Flash control operations
#define 		FLASH_UNLOCK_CMD		(uint8_t)(0x01)
#define			FLASH_LOCK_CMD			(uint8_t)(0x02)
#define			FLASH_PROG_CMD			(uint8_t)(0x03)
#define			FLASH_READ_CMD			(uint8_t)(0x04)
#define			FLASH_ERASE_CMD			(uint8_t)(0x05)
#define			FLASH_MASS_ERASE_CMD	(uint8_t)(0x06)
#define			FLASH_CPY_CMD			(uint8_t)(0x07)
// Transfer control operations
#define			TRANSFER_CNTRL_CMD		(uint8_t)(0x08)
// option bytes control
#define			OB_UNLOCK_CMD			(uint8_t)(0x09)
#define			OB_LOCK_CMD				(uint8_t)(0x0A)
#define			OB_READ_CMD				(uint8_t)(0x0B)
// Protection control
#define			WR_PROTECT_CMD			(uint8_t)(0x0C)
#define			WR_UNPROTECT_CMD		(uint8_t)(0x0D)

// For future versions
#define			RD_PROTECT_CMD			(uint8_t)(0x0E)
#define			RD_UNPROTECT_CMD		(uint8_t)(0x0F)
#define			CRC_CHECK_CMD			(uint8_t)(0x10)


/**
 * @}
 */

/**
 * @defgroup BOOT_MSGS_ERR
 * @brief    Contains all the messages and errors codes.
 * @{
 */

#define 		ACK_MSG					(uint8_t)(0x41)
#define 		NACK_MSG				(uint8_t)(0x4E)



#define 		PGSERR_ERR_MSG			(uint8_t)(0xE1)
#define 		PGPERR_ERR_MSG			(uint8_t)(0xE2)
#define 		PGAERR_ERR_MSG			(uint8_t)(0xE3)
#define 		WRPERR_ERR_MSG			(uint8_t)(0xE4)

#define 		RDPR_ERR_MSG			(uint8_t)(0xE5)
#define 		OP_ERR_MSG				(uint8_t)(0xE6)
/**
 * @}
 */

/**
 * @defgroup BOOT_DESCRIPTION
 * @brief    Contains all the descriptions and notes.
 * @{
 */

const char* SEPART_LINE =          "------------------------------------------------\n";
const char* CMD_HEAD =    		    "|*********          COMMANDS      *************|\n";
const char* MSG_HEAD =              "|*********         MSGs & ERR     *************|\n";
const char* INFO_HEAD =             "|*********     BootLoader Info    *************|\n";

const char* ID_LINE   =             "| BL ID           :    ";
const char* VER_LINE  =  			"| BL Version      :    ";
const char* AUTH_LINE =			    "| BL Author       :    ";

const char* GET_INFO =				"| Get BL Info     :    ";

const char* FLASH_UNLOCK =			"| FLASH Unlock    :    ";
const char* FLASH_LOCK =			"| FLASH Lock      :    ";

const char* FLASH_PROG =			"| FLASH Write     :    ";
const char* FLASH_READ =			"| FLASH Read      :    ";

const char* SEC_ERASE =			    "| Sectors erase   :    ";
const char* SEC_ERASE =			    "| Sectors erase   :    ";


const char* OB_UNLOCK =			    "| OB Unlock       :    ";
const char* OB_LOCK =			    "| OB Lock         :    ";
const char* OB_GET =			    "| OB Read         :    ";


const char* SEC_PROTECT =	        "| Sectors Lock    :    ";
const char* SEC_UNPROTECT =	        "| Sectors Unlock  :    ";



const char* ACK =			        "| ACK CODE        :    ";
const char* NACK =			        "| NACK CODE       :    ";

const char* PGSERR_ERR =		    "| Programming Sequence Error     :    ";
const char* PGPERR_ERR =	        "| Programming Parallelism Error  :    ";
const char* PGAERR_ERR =	        "| Programming Alignment Error    :    ";
const char* WRPERR_ERR =	        "| Write Protection Error         :    ";

/**
 * @}
 */



/**
 * @}
 */
/**
 * @}
 */
/**
 * @}
 */


