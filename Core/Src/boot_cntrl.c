
/*******************************************************************************
 * @file    BOOT_CNTRL.c
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
#include <BOOT_CNTRL.h>



/**
  * @}
  */

/**
 * @defgroup  private local functions
 * @brief
 * @{
 */

static void BOOT_SYS_RESET(void);

/**
 * @defgroup Exported_VALUES (MEM_MAP_ADDRESSES)
 * @{
 */
#define 	FLASH_MEM_ADDR		(uint32_t)(0x08000000)
#define 	SYS_MEM_ADDR	(uint32_t)(0x1FFF0000)
#define 	RAM_ADDR		(uint32_t)(0x20000000)


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
 * @brief 	Jump and transfer control into a program image at a specific location
 * @note	The function shouldn't return, if returned, an error has occurred.
 * @param   None
 * @retval  None
 */
void BOOT_TRANSFER_CNTRL(uint32_t ImageAddress){

	/**
	 * First value in the image should be a valid stack pointer address,
	 * A valid stack pointer address should be located at RAM,
	 * So we check if it's in the RAM or not.
	 */

	uint32_t _stackPtr = * (uint32_t *) ImageAddress;
    if(RAM_ADDR == (_stackPtr & RAM_ADDR))
    {

    	/* perform system reset */
    	BOOT_SYS_RESET();

        /* Check jump address */

        /* Enable SYSCFG clock */
        __HAL_RCC_SYSCFG_CLK_ENABLE();

        /* Set jump to the reset handler */
        void (* Tranfer_ImageLocation)(void) = (void *)(*((uint32_t *)(ImageAddress + 4)));


        if(SYS_MEM_ADDR == ImageAddress)    // Transfer to the System memory
        {
          /* Map address 0x0 to system memory */
        	SET_BIT(SYSCFG->MEMRMP, SYSCFG_MEMRMP_MEM_MODE_0);
        	CLEAR_BIT(SYSCFG->MEMRMP, SYSCFG_MEMRMP_MEM_MODE_1);
        }

        else if (FLASH_MEM_ADDR <= ImageAddress )	// Transfer to the Flash memory
        {

            /* Map address 0x0 to Flash memory */
        	CLEAR_BIT(SYSCFG->MEMRMP, SYSCFG_MEMRMP_MEM_MODE_0 | SYSCFG_MEMRMP_MEM_MODE_1);

          /* Vector Table Relocation in Internal FLASH */
          __DMB();
          WRITE_REG(SCB->VTOR, ImageAddress);
          __DSB();
        }


        /* Set stack pointer */
        __set_MSP(_stackPtr);

        /* Jump */
        Tranfer_ImageLocation();
     }

	  else
	  {
		/* No valid stack pointer */
		/*Do Nothing , just return*/
	  }
}


/**
 * @brief 	Copy Image from Source location to destination location
 * @note	The function shouldn't return, if returned, an error has occurred.
 * @param   src image address , dest image address , size of the image by bytes
*  @retval FLASH_ErrorCode: The returned value can be a combination of:
*            @arg HAL_FLASH_ERROR_RD: FLASH Read Protection error flag (PCROP)
*            @arg HAL_FLASH_ERROR_PGS: FLASH Programming Sequence error flag
*            @arg HAL_FLASH_ERROR_PGP: FLASH Programming Parallelism error flag
*            @arg HAL_FLASH_ERROR_PGA: FLASH Programming Alignment error flag
*            @arg HAL_FLASH_ERROR_WRP: FLASH Write protected error flag
*            @arg HAL_FLASH_ERROR_OPERATION: FLASH operation Error flag
 */
FLASH_ErrorCode BOOT_CPY_IMAGE(uint32_t srcAddress ,uint32_t destAddress, uint32_t size){


	uint32_t operation_state_error;
	uint32_t  Data;
	uint8_t resend_count = 0;
	uint32_t idx = 0;


	/* Unlock the flash */
	if ( HAL_FLASH_Unlock()){

		operation_state_error = HAL_FLASH_GetError();
		goto END_OP;
	}


	while (idx < size)
	{

		if (!resend_count){

			/*Read the current memory location*/
			Data = * ((uint32_t *) (srcAddress+idx));

		}

		// try 3 times as maximum to write the word
		// If exceeded 3, abort writing.
		if (resend_count >= 3){

			goto END_OP;
		}

		/*Try writing the data*/
		if ( HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, destAddress+idx, Data)){

			resend_count++;
			operation_state_error = HAL_FLASH_GetError();
		}


		else   // the operation has succeeded
		{
			resend_count = 0;
			idx += 4;
		}

	}

	END_OP:
	return operation_state_error;


}



/**
 * @brief 	clears all the configuration that the Boot Loader made and made every thing as just a reset happened;
 * @param   none
 * @retval  none
 */
static void BOOT_SYS_RESET(void){


    /* Disable all interrupts */
    __disable_irq();

    /* Reset GPIOA and DMA2 */
    RCC->AHB1RSTR = RCC_AHB1RSTR_GPIOARST;

    /* Release reset */
    RCC->AHB1RSTR = 0;

    /* Reset USART1 */
    RCC->APB2RSTR = RCC_APB2RSTR_USART1RST;

    /* Release reset */
    RCC->APB2RSTR = 0;

    /* Reset RCC */
    /* Set HSION bit to the reset value */
    RCC->CR |= RCC_CR_HSION;

    /* Wait till HSI is ready */
    while(RCC_CR_HSIRDY != (RCC_CR_HSIRDY & RCC->CR))
    {
      /* Waiting */
    }

    /* Set HSITRIM[4:0] bits to the reset value */
    RCC->CR |= RCC_CR_HSITRIM_4;

    /* Reset CFGR register */
    RCC->CFGR = 0;

    /* Wait till clock switch is ready and
     * HSI oscillator selected as system clock */
    while(0 != (RCC_CFGR_SWS & RCC->CFGR))
    {
      /* Waiting */
    }

    /* Clear HSEON, HSEBYP and CSSON bits */
    RCC->CR &= ~(RCC_CR_HSEON | RCC_CR_HSEBYP | RCC_CR_CSSON);

    /* Wait till HSE is disabled */
    while(0 != (RCC_CR_HSERDY & RCC->CR))
    {
      /* Waiting */
    }

    /* Clear PLLON bit */
    RCC->CR &= ~RCC_CR_PLLON;

    /* Wait till PLL is disabled */
    while(0 != (RCC_CR_PLLRDY & RCC->CR))
    {
      /* Waiting */
    }

    /* Reset PLLCFGR register to default value */
    RCC->PLLCFGR = RCC_PLLCFGR_PLLM_4 | RCC_PLLCFGR_PLLN_6
        | RCC_PLLCFGR_PLLN_7 | RCC_PLLCFGR_PLLQ_2;

    /* Reset SysTick */
    SysTick->CTRL = 0x00000000;
    SysTick->LOAD = 0x00000000;
    SysTick->VAL  = 0x00000000;


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


