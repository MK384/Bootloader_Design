
/*******************************************************************************
 * @file    boot_cntrl.c
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
#include "boot_cntrl.h"


/**
 * @addtogroup
 * @{
 */

/**
 * @defgroup  private local functions
 * @brief
 * @{
 */

static void clear_all_cnfgs(void);

/**
 * @defgroup Exported_VALUES
 * @{
 */

#define 	SYS_MEM_ADDR	(uint32_t)(0x1FFF0000)
#define 	RAM_ADDR		(uint32_t)(0x20000000)

/**
 * @}
 */




/**
 * @brief 	Jump and transfer control into a program image at a specific location
 * @note	The function shouldn't return, if returned, an error has occurred.
 * @param   None
 * @retval  None
 */
void BOOT_Transfer_Cntrl(uint32_t ImageLocation){

	/**
	 * First value in the image should be a valid stack pointer address,
	 * A valid stack pointer address should be located at RAM,
	 * So we check if it's in the RAM or not.
	 */

	uint32_t _stackPtr = * (uint32_t *) ImageLocation;
    if(RAM_ADDR == (_stackPtr & RAM_ADDR))
    {

    	clear_all_cnfgs();

        /* Check jump address */


        if(SYS_MEM_ADDR == ImageLocation)    // Transfer to the System memory
        {
          /* Enable SYSCFG clock */
          RCC->APB2ENR |= RCC_APB2LPENR_SYSCFGLPEN;

          /* Map address 0x0 to system memory */
          SYSCFG->MEMRMP = SYSCFG_MEMRMP_MEM_MODE_0;
        }
        else	// Transfer to the Flash memory
        {
          /* Vector Table Relocation in Internal FLASH */
          __DMB();
          SCB->VTOR = ImageLocation;
          __DSB();
        }

        /* Set jump to the reset handler */
        void (* J_ImageLocation)(void) = (void *)(*((uint32_t *)(ImageLocation + 4)));

        /* Set stack pointer */
        __set_MSP(_stackPtr);

        /* Jump */
        J_ImageLocation();
     }



	  else
	  {
		/* No valid stack pointer */
		/*Do Nothing , just return*/
	  }
}




/**
 * @brief 	clears all the configuration that the Boot Loader made and made every thing as just a reset happened;
 * @param   none
 * @retval  none
 */
static void clear_all_cnfgs(void){

    /* Disable all interrupts */
    __disable_irq();

    /* Reset GPIOA and DMA2 */
    RCC->AHB1RSTR = (RCC_AHB1RSTR_GPIOARST | RCC_AHB1RSTR_DMA2RST);

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


