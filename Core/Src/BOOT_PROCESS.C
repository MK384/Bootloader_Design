
/*******************************************************************************
 * @file    BOOT_PROCESS.c
 * @author  Mohammed Khaled
 * @email   Mohammed.kh384@gmail.com
 * @website EMSTutorials.blogspot.com/
 * @Created on: Feb 24, 2023
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
#include "BOOT_PROCESS.h"


/**
 * @defgroup  private local variables
 * @brief
 * @{
 */

char RxBuffer[RX_BUFFER_SIZE];		// this buffer will contain the proceed data for all process functions.
char TxBuffer[TX_BUFFER_SIZE];		// this buffer will contain the proceed data for all process functions.

extern UART_HandleTypeDef huart1;


/**
  * @}
  */

/**
 * @defgroup  private local defines
 * @brief
 * @{
 */


#define 	TYPEPROGRAM				FLASH_TYPEPROGRAM_WORD

#define 	TRANS_WAIT_TIME			(0x00000064U)
#define		CMD_SIZE				(0x00000001U)
#define 	DATA_OFFSET				(0x00000005U)
#define 	BLOCK_SIZE				(0x00000004U)

#define 	ADDRESS_OFFSET			(0x00000001U)
#define 	SECTOR_OFFSET			(0x00000001U)
#define 	SIZE_OFFSET				(0x00000008U)


/**
  * @}
  */

/**
 * @defgroup  private local types
 * @brief
 * @{
 */

typedef   uint32_t	AddressType;
typedef	  uint32_t	DataType;
typedef	  uint32_t	SizeType;


/**
  * @}
  */

/**
 * @defgroup  private local functions
 * @brief
 * @{
 */

static	void SEND_ACK(void);
static	void SEND_NACK(void);



/**
* @}
*/

/**
 * @brief	Proccess initialization function
 * @note	None
 * @param   None
 * @retval  None
 */
void PROCESS_INIT						(void){

	Process_Handlers[GET_CMD]         =		 PROCESS_GET_CMD;
	Process_Handlers[FLASH_UNLOCK_CMD]     =		 PROCESS_FLASH_UNLOCK_CMD;
	Process_Handlers[FLASH_LOCK_CMD]       = 		 PROCESS_FLASH_LOCK_CMD;
	Process_Handlers[FLASH_PROG_CMD]       =		 PROCESS_FLASH_PROG_CMD;
	Process_Handlers[FLASH_READ_CMD] 	   =		 PROCESS_FLASH_READ_CMD;
	Process_Handlers[FLASH_ERASE_CMD] 	   =		 PROCESS_FLASH_ERASE_CMD;
	Process_Handlers[FLASH_MASS_ERASE_CMD] = 		 PROCESS_FLASH_MASS_ERASE_CMD;
	Process_Handlers[FLASH_CPY_CMD]        = 		 PROCESS_FLASH_CPY_CMD;
	Process_Handlers[TRANSFER_CNTRL_CMD]   = 		 PROCESS_TRANSFER_CNTRL_CMD;
	Process_Handlers[OB_UNLOCK_CMD]        =		 PROCESS_OB_UNLOCK_CMD;
	Process_Handlers[OB_LOCK_CMD]          = 		 PROCESS_OB_LOCK_CMD;
	Process_Handlers[OB_READ_CMD]          = 		 PROCESS_OB_READ_CMD;
	Process_Handlers[WR_PROTECT_CMD]       = 		 PROCESS_WR_PROTECT_CMD;
	Process_Handlers[WR_UNPROTECT_CMD]     = 		 PROCESS_WR_UNPROTECT_CMD;


}

/**
 * @}
 */


/**
 * @brief	Called when Get command is retrieved.
 * @note	None
 * @param   None
 * @retval  None
 */
void PROCESS_GET_CMD (void){

	// Send Bootloader info Header
	HAL_UART_Transmit(&huart1, (uint8_t*) SEPART_LINE, (uint16_t)sizeof(SEPART_LINE), TRANS_WAIT_TIME);
	HAL_UART_Transmit(&huart1, (uint8_t*) INFO_HEAD, (uint16_t)sizeof(INFO_HEAD),     TRANS_WAIT_TIME);
	HAL_UART_Transmit(&huart1, (uint8_t*) SEPART_LINE, (uint16_t)sizeof(SEPART_LINE), TRANS_WAIT_TIME);

	// Send bootloader Info
	HAL_UART_Transmit(&huart1, (uint8_t*) ID_LINE, (uint16_t)sizeof(ID_LINE), TRANS_WAIT_TIME);
	HAL_UART_Transmit(&huart1, (uint8_t*) ID, (uint16_t)sizeof(ID), TRANS_WAIT_TIME);


	HAL_UART_Transmit(&huart1, (uint8_t*) VER_LINE, (uint16_t)sizeof(VER_LINE), TRANS_WAIT_TIME);
	HAL_UART_Transmit(&huart1, (uint8_t*) VERSION, (uint16_t)sizeof(VERSION), TRANS_WAIT_TIME);


	HAL_UART_Transmit(&huart1, (uint8_t*) AUTH_LINE, (uint16_t)sizeof(AUTH_LINE), TRANS_WAIT_TIME);
	HAL_UART_Transmit(&huart1, (uint8_t*) AUTHOR, (uint16_t)sizeof(AUTHOR), TRANS_WAIT_TIME);

	HAL_UART_Transmit(&huart1, (uint8_t*) SEPART_LINE, (uint16_t)sizeof(SEPART_LINE), TRANS_WAIT_TIME);

}

/**
 * @}
 */


/**
 * @brief	Called when unlock command retrieved
 * @note	None
 * @param   None
 * @retval  None
 */

void PROCESS_FLASH_UNLOCK_CMD	(void){

	if(HAL_FLASH_Unlock())
		SEND_NACK();

	else
		SEND_ACK();
}

/**
 * @}
 */

/**
 * @brief	Called when lock command retrieved
 * @note	None
 * @param   None
 * @retval  None
 */
void PROCESS_FLASH_LOCK_CMD		(void){

	if(HAL_FLASH_Lock())
		SEND_NACK();

	else
		SEND_ACK();
}


/**
 * @}
 */

/**
 * @brief	Called when Program command retrieved
 * @note	None
 * @param   None
 * @retval  None
 */
void PROCESS_FLASH_PROG_CMD		(void){

	//  Skip the ADDRESS OFFSET and read the address to program.
	AddressType Address = *( (AddressType*) (&RxBuffer[ADDRESS_OFFSET]));

	for (int idx = 0 ; idx < BLOCK_SIZE; ++idx) {

		if(HAL_FLASH_Program(TYPEPROGRAM, (Address + (idx << TYPEPROGRAM)),  RxBuffer[DATA_OFFSET+(idx << TYPEPROGRAM)]))
			{
			SEND_NACK();
			return;
			}
	}
	SEND_ACK();

}


/**
 * @}
 */

/**
 * @brief	Called when read command retrieved.
 * @note	None
 * @param   None
 * @retval  None
 */
void PROCESS_FLASH_READ_CMD		(void){

	//  Skip the first byte contain the command and read word as the address to program.
	AddressType Address = *( (AddressType*) (&RxBuffer[ADDRESS_OFFSET]));

	for (int idx = 0; idx < BLOCK_SIZE; ++idx)
		*((DataType*)&TxBuffer[idx<<TYPEPROGRAM]) = *((DataType*)(Address+(idx<<TYPEPROGRAM)));

	HAL_UART_Transmit(&huart1, (uint8_t*) TxBuffer, (uint16_t)(BLOCK_SIZE << TYPEPROGRAM), TRANS_WAIT_TIME);

}


/**
 * @}
 */

/**
 * @brief	Called when erase command retrieved.
 * @note	None
 * @param   None
 * @retval  None
 */
void PROCESS_FLASH_ERASE_CMD	(void){

	FLASH_EraseInitTypeDef strInit;
	uint32_t SectorError = 0;


	strInit.Banks = FLASH_BANK_1;
	strInit.Sector = RxBuffer[SECTOR_OFFSET];
	strInit.NbSectors = RxBuffer[SECTOR_OFFSET+1];
	strInit.TypeErase = FLASH_TYPEERASE_SECTORS;
	strInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;

	HAL_FLASHEx_Erase(&strInit, &SectorError);

	if(SectorError != 0xFFFFFFFFU){
		SEND_NACK();
		HAL_UART_Transmit(&huart1, (uint8_t*) &SectorError, 1U , TRANS_WAIT_TIME);
		return;
	}
	SEND_ACK();

}


/**
 * @}
 */

/**
 * @brief	Called when mass erase command retrieved.
 * @note	None
 * @param   None
 * @retval  None
 */

void PROCESS_FLASH_MASS_ERASE_CMD	    (void){

	FLASH_EraseInitTypeDef strInit;
	uint32_t SectorError = 0;


	strInit.Banks = FLASH_BANK_1;
	strInit.TypeErase = FLASH_TYPEERASE_MASSERASE;
	strInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;

	HAL_FLASHEx_Erase(&strInit, &SectorError);

	if(SectorError != 0xFFFFFFFFU){
		SEND_NACK();
		HAL_UART_Transmit(&huart1, (uint8_t*) &SectorError, 1U , TRANS_WAIT_TIME);
		return;
	}
	SEND_ACK();


}

/**
 * @}
 */


/**
 * @brief	Called when copy command retrieved.
 * @note	None
 * @param   None
 * @retval  None
 */
void PROCESS_FLASH_CPY_CMD		(void){

	//  Skip the first byte contain the command and read word as the address to program.
	AddressType srcAddress = *( (AddressType*) (&RxBuffer[ADDRESS_OFFSET]));
	AddressType destAddress = *( (AddressType*) (&RxBuffer[DATA_OFFSET]));
	SizeType size = *( (SizeType*) (&RxBuffer[SIZE_OFFSET]));


	if (BOOT_CPY_IMAGE(srcAddress, destAddress, size)){
		SEND_NACK();
		return;
	}
	SEND_ACK();
}


/**
 * @}
 */


/**
 * @brief	Called when OB Unlock command retrieved.
 * @note	None
 * @param   None
 * @retval  None
 */
void PROCESS_OB_UNLOCK_CMD	(void){


	if(HAL_FLASH_OB_Unlock())
		SEND_NACK();

	else
		SEND_ACK();

}


/**
 * @}
 */

/**
 * @brief	Called when OB Lock command retrieved.
 * @note	None
 * @param   None
 * @retval  None
 */
void PROCESS_OB_UNLOCK_CMD	(void){


	if(HAL_FLASH_OB_Lock())
		SEND_NACK();

	else
		SEND_ACK();

}


/**
 * @}
 */

/**
 * @brief	Called when OB Read command retrieved.
 * @note	None
 * @param   None
 * @retval  None
 */
void PROCESS_OB_READ_CMD	(void){

	*(  (DataType*)  TxBuffer  ) = *( (DataType *) OPTCR_BYTE0_ADDRESS   );
	HAL_UART_Transmit(&huart1, (uint8_t*) TxBuffer, 4U, TRANS_WAIT_TIME);
}


/**
 * @}
 */
/**
 * @brief	Called when Sector write protect command retrieved.
 * @note	None
 * @param   None
 * @retval  None
 */
void PROCESS_WR_PROTECT_CMD	(void){

	FLASH_OBProgramInitTypeDef pOBInit;
	pOBInit.OptionType = OPTIONBYTE_WRP;
	pOBInit.Banks = FLASH_BANK_1;
	pOBInit.WRPState = OB_WRPSTATE_ENABLE;
	pOBInit.WRPSector = RxBuffer[SECTOR_OFFSET] ;

	if (HAL_FLASHEx_OBProgram(&pOBInit)){
		SEND_NACK();
		return;
	}

	SEND_ACK();
}
/**
 * @}
 */
/**
 * @brief	Called when Sector write Unprotect command retrieved.
 * @note	None
 * @param   None
 * @retval  None
 */
void PROCESS_WR_UNPROTECT_CMD	(void){

	FLASH_OBProgramInitTypeDef pOBInit;
	pOBInit.OptionType = OPTIONBYTE_WRP;
	pOBInit.Banks = FLASH_BANK_1;
	pOBInit.WRPState = OB_WRPSTATE_DISABLE;
	pOBInit.WRPSector = RxBuffer[SECTOR_OFFSET] ;

	if (HAL_FLASHEx_OBProgram(&pOBInit)){
		SEND_NACK();
		return;
	}

	SEND_ACK();
}


/**
 * @}
 */
/**
 * @brief	Called when transfer control command retrieved.
 * @note	None
 * @param   None
 * @retval  None
 */
void PROCESS_TRANSFER_CNTRL_CMD	(void){


	AddressType ImageAddress = *( (AddressType*) (&RxBuffer[ADDRESS_OFFSET]));
	BOOT_TRANSFER_CNTRL(ImageAddress);

}

/**
 * @}
 */



/**
 * @brief
 * @note	None
 * @param   None
 * @retval  None
 */


/**
 * @}
 */

/**
 * @brief	Transmit ACK to a command proceed
 * @note	None
 * @param   None
 * @retval  None
 */
static	void SEND_ACK(void){

	TxBuffer[0] = ACK_MSG;
	HAL_UART_Transmit(&huart1, TxBuffer, CMD_SIZE, TRANS_WAIT_TIME);

}


/**
 * @}
 */

/**
 * @brief	Transmit NACK to a command proceed followed by the error code
 * @note	None
 * @param   None
 * @retval  None
 */

static	void SEND_NACK(void){

	RxBuffer[0] = NACK_MSG;
	HAL_UART_Transmit(&huart1, RxBuffer, CMD_SIZE, TRANS_WAIT_TIME);

	uint8_t ERR_CODE;

	switch (HAL_FLASH_GetError()) {
		case HAL_FLASH_ERROR_RD:
			ERR_CODE = RDPR_ERR_MSG;
			break;
		case HAL_FLASH_ERROR_PGS:
			ERR_CODE = PGSERR_ERR_MSG;
			break;
		case HAL_FLASH_ERROR_PGP:
			ERR_CODE = PGPERR_ERR_MSG;
			break;
		case HAL_FLASH_ERROR_PGA:
			ERR_CODE = PGAERR_ERR_MSG;
			break;
		case HAL_FLASH_ERROR_WRP:
			ERR_CODE = WRPERR_ERR_MSG;
			break;
		case HAL_FLASH_ERROR_OPERATION:
			ERR_CODE = OP_ERR_MSG;
			break;
		default:
			ERR_CODE = 0x00;
			break;
	}

	TxBuffer[0] = ERR_CODE;
	HAL_UART_Transmit(&huart1, TxBuffer, CMD_SIZE, TRANS_WAIT_TIME);

}


/**
 * @}
 */
/**
 * @}
 */










