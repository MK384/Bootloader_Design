/*
 * usbdebug.h
 *
 *  Created on: Aug 5, 2022
 *      Author: mohammed khaled
 */

#ifndef SRC_USBDEBUG_H_
#define SRC_USBDEBUG_H_

/**
 * @}
 */

/** @defgroup USBD_CDC_IF_Exported_Types USBD_CDC_IF_Exported_Types
 * @brief Types.
 * @{
 */

/* USER CODE BEGIN EXPORTED_TYPES */

/* USER CODE END EXPORTED_TYPES */

/**
 * @}
 */

/** @defgroup USBD_CDC_IF_Exported_Macros USBD_CDC_IF_Exported_Macros
 * @brief Aliases.
 * @{
 */

/*
 * Include Used libraries
 *
 * */
#include "stdint.h"
#include "string.h"
#include "inttypes.h"
#include "stdio.h"
#include "usbd_cdc_if.h"

/*
 * Important defines to easy access
 * */
//#define RxBuffer UserRxBufferFS



/* USER CODE BEGIN EXPORTED_FUNCTIONS */

void print(char *message);
void println(char *message);
void printfp(float value);
void printu(uint32_t value);


char* scan(char *inputMessage);
uint32_t scanu(char *inputMessage);
float scanfp(char *inputMessage);

/* USER CODE END EXPORTED_FUNCTIONS */

#endif /* SRC_USBDEBUG_H_ */
