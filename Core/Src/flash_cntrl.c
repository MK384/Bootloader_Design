
/*******************************************************************************
 * @file    flash_cntrl.c
 * @author  Mohammed Khaled
 * @email   Mohammed.kh384@gmail.com
 * @website EMSTutorials.blogspot.com/
 * @Created on: Feb 20, 2023
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

/* Includes */
#include <stddef.h>
#include "flash_cntrl.h"

/**
 * @addtogroup stm32_examples
 * @{
 */

/**
 * @defgroup flash
 * @brief
 * @{
 */

/**
 * @defgroup flash_private_typedefs
 * @{
 */


/**
 * @}
 */

/**
 * @defgroup flash_private_defines
 * @{
 */

static ERR_StateType Flash_Errors_Check(void);
/**
 * @}
 */

/**
 * @defgroup flash_private_macros
 * @{
 */

/**
 * @}
 */

/**
 * @defgroup flash_private_constants
 * @{
 */

/**
 * @}
 */

/**
 * @defgroup flash_private_variables
 * @{
 */


/**
 * @}
 */

/**
 * @defgroup flash_private_function_prototypes
 * @{
 */


/**
 * @}
 */

/**
 * @defgroup flash_private_functions
 * @{
 */


/**
 * @brief   Flash Initialization function
 * @note
 * @param   None
 * @retval  None
 */
void Flash_Init(void)
{
  /* Write KEY1 */
  FLASH->KEYR = 0x45670123;

  /* Write KEY2 */
  FLASH->KEYR = 0xCDEF89AB;

  /* Select flash parallelism x32 */
  FLASH->CR &= ~FLASH_CR_PSIZE;
  FLASH->CR |= FLASH_CR_PSIZE_1;

  /* Write lock bit */
  FLASH->CR |= FLASH_CR_LOCK;
}

/**
 * @brief   Check flash errors
 * @param   none
 * @retval  error_type
 */
 static ERR_StateType Flash_Errors_Check(void)
{
	ERR_StateType error_type;
  if(FLASH_SR_PGSERR == (FLASH->SR & FLASH_SR_PGSERR))
  {
    /* Programming sequence error */
	  error_type = Programming_sequence_error;

    /* Clear */
    FLASH->SR |= FLASH_SR_PGSERR;
  }
  else
  {
    /* Do nothing */
  }

  if(FLASH_SR_PGPERR == (FLASH->SR & FLASH_SR_PGPERR))
  {
    /* Programming parallelism error */
	  error_type = Programming_parallelism_error;

    /* Clear */
    FLASH->SR |= FLASH_SR_PGPERR;
  }


  if(FLASH_SR_PGAERR == (FLASH->SR & FLASH_SR_PGAERR))
  {
    /* Programming alignment error */
	  error_type = Programming_alignment_error;

    /* Clear */
    FLASH->SR |= FLASH_SR_PGAERR;
  }
  else
  {
    /* Do nothing */
  }

  if(FLASH_SR_WRPERR == (FLASH->SR & FLASH_SR_WRPERR))
  {
    /* Write protection error */
	  error_type = Write_protection_error;

    /* Clear */
    FLASH->SR |= FLASH_SR_WRPERR;
  }
  else
  {
    /* Do nothing */
  }

  return error_type;
}

 /**
  * @brief   Lock the Flash memory Interface registers.
  * @param   none
  * @retval  none
  */

 void Flash_Lock(void){

     /* Write lock bit */
     FLASH->CR |= FLASH_CR_LOCK;

     /*wait until locking takes effect*/

     while ((FLASH->CR & FLASH_CR_LOCK) == 0x0 ){

     }


 }

 /**
  * @brief   Unlock the Flash memory Interface registers.
  * @param   none
  * @retval  none
  */

 void Flash_Unlock(void){

     /* Write KEY1 */
     FLASH->KEYR = 0x45670123;

     /* Write KEY2 */
     FLASH->KEYR = 0xCDEF89AB;

 }

 /**
  * @brief   Write a word of data at specific address
  * @param   address: uint32 , data:uint32
  * @retval  ERR_StateType
  */
 ERR_StateType Flash_Write(uint32_t address , uint32_t data ){

     /* Wait bsy flag */
     while(0 != (FLASH->SR & FLASH_SR_BSY))
     {
       /* Waiting */
     }

     /* Enable flash programming */
     FLASH->CR |= FLASH_CR_PG;

     /* Write data into flash */

     *(volatile uint32_t*)(address) = data;

     /* Wait bsy flag */
     while(0 != (FLASH->SR & FLASH_SR_BSY))
     {
       /* Waiting */
     }

     /* Disable flash programming */
     FLASH->CR &= ~FLASH_CR_PG;

     return Flash_Errors_Check();



 }


 /**
  * @brief   Read a word of data at specific address
  * @param   address: uint32
  * @retval  ERR_StateType , dataOut address holds the data if the process succeed.
  */
 ERR_StateType Flash_Read(uint32_t address , uint32_t* dataOut){


     *dataOut = * (uint32_t *) address;
     return Flash_Errors_Check();
 }


 /**
  * @brief   Erase an given sector in the flash memory.
  * @param   secto_numr: uint32 [must be less than 5 for stm32f401xC/B]
  * @retval  ERR_StateType.
  */
 ERR_StateType Flash_erase(uint32_t sector_num){

     /* Wait bsy flag */
     while(0 != (FLASH->SR & FLASH_SR_BSY))
     {
       /* Waiting */
     }

     /* Select sector erase */
     FLASH->CR |= FLASH_CR_SER;

     /* Write sector number */
     FLASH->CR |= (FLASH_CR_SNB & (sector_num << FLASH_CR_SNB_Pos));

     /* Start sector erase */
     FLASH->CR |= FLASH_CR_STRT;

     /* Wait bsy flag */
     while(0 != (FLASH->SR & FLASH_SR_BSY))
     {
       /* Waiting */
     }

     /* Disable sector erase */
     FLASH->CR &= ~FLASH_CR_SER;

     return Flash_Errors_Check();
 }


 /**
  * @brief   Protect a sector in flash memory from being written.
  * @param   sector_num: uint32 [must be less than 5 for stm32f401xC/B]
  * @retval  ERR_StateType.
  */
 ERR_StateType Flash_WRP_Enable(uint32_t sector_num){

     /* Wait bsy flag */
     while(0 != (FLASH->SR & FLASH_SR_BSY))
     {
       /* Waiting */
     }

       /* write the sector number */
       FLASH->OPTCR &= ~((1 << sector_num) << 16);


     /* Start sector write protection */
     FLASH->OPTCR |= FLASH_OPTCR_OPTSTRT;

     /* Wait bsy flag */
     while(0 != (FLASH->SR & FLASH_SR_BSY))
     {
       /* Waiting */
     }

     return Flash_Errors_Check();
 }


 /**
  * @brief   un-protect a sector in flash memory.
  * @param   sector_num: uint32 [must be less than 5 for stm32f401xC/B]
  * @retval  ERR_StateType.
  */
 ERR_StateType Flash_WRP_Desable(uint32_t sector_num){

     /* Wait bsy flag */
     while(0 != (FLASH->SR & FLASH_SR_BSY))
     {
       /* Waiting */
     }

       /* write the sector number */
       FLASH->OPTCR |= ((1 << sector_num) << 16);


     /* Start sector write protection */
     FLASH->OPTCR |= FLASH_OPTCR_OPTSTRT;

     /* Wait bsy flag */
     while(0 != (FLASH->SR & FLASH_SR_BSY))
     {
       /* Waiting */
     }

     return Flash_Errors_Check();

 }



 /**
  * @brief   Option bytes unlock
  * @param   Non
  * @retval  ERR_StateType.
  */

 ERR_StateType Flash_OB_Unlock(void){

     /* Write OPTKEY1 */
     FLASH->OPTKEYR = 0x08192A3B;

     /* Write OPTKEY2 */
     FLASH->OPTKEYR = 0x4C5D6E7F;

     return Flash_Errors_Check();
 }


 /**
  * @brief   Option bytes unlock
  * @param   Non
  * @retval  ERR_StateType.
  */

 ERR_StateType Flash_OB_Lock(void){

     /* Write lock bit */
     FLASH->OPTCR |= FLASH_OPTCR_OPTLOCK;

     return Flash_Errors_Check();

 }



/**
 * @}
 */
/**
 * @}
 */
/**
 * @}
 */


