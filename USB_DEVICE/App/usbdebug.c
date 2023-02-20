
#include "usbdebug.h"




void print(char *message) {
	USBD_StatusTypeDef USBD_Status;
	do{
		USBD_Status = CDC_Transmit_FS((uint8_t*) message, strlen(message));
	   } while(USBD_Status != USBD_OK);
}


void println(char *message) {

	char* tmpBuffer = strndup(message,strlen(message)+2);

	strcat(tmpBuffer, "\n\r");
	uint16_t Len =  (uint16_t) strlen(tmpBuffer);

	USBD_StatusTypeDef USBD_Status;
		do{
			USBD_Status = CDC_Transmit_FS((uint8_t*)tmpBuffer,Len);
		   } while(USBD_Status != USBD_OK);
		free(tmpBuffer);
}

void printfp(float value) {

	static uint8_t digits[20];
	memset(digits,'\0',20);
	sprintf((char*) digits, "%.8f\n\r", value);
	USBD_StatusTypeDef USBD_Status;
		do{
			USBD_Status = CDC_Transmit_FS(digits, 20);
		   } while(USBD_Status != USBD_OK);
}


void printu(uint32_t value) {
	static uint8_t digits[20];
	memset(digits,'\0',20);
	sprintf((char*) digits, "%"PRIu32"\n\r", value);

	USBD_StatusTypeDef USBD_Status;
		do{
			USBD_Status = CDC_Transmit_FS(digits, 20);
		   } while(USBD_Status != USBD_OK);
}

/*
 * Any Invocation to this function renew the string in the buffer pointed with the returned pointer ,
 * if the string returned is to be kept copy it into another buffer before invoking this function again. !!
 *
 * */
char* scan(char *inputMessage){

	print(inputMessage);

	memset(UserRxBufferFS,'\0',strlen(( char *)UserRxBufferFS));
	while(strlen(( char *)UserRxBufferFS) == 0);

	size_t currentSize;
	do{
		currentSize = strlen(( char *)UserRxBufferFS);
		HAL_Delay(1);
	}while(currentSize != strlen(( char *)UserRxBufferFS) );

	print(( char *)UserRxBufferFS);

	return (char*)UserRxBufferFS;

}


uint32_t scanu(char *inputMessage){

	print(inputMessage);

	memset(UserRxBufferFS,'\0',strlen(( char *)UserRxBufferFS));
	while(strlen(( char *)UserRxBufferFS) == 0);

	size_t currentSize;
	do{
		currentSize = strlen(( char *)UserRxBufferFS);
		HAL_Delay(1);
	}while(currentSize != strlen(( char *)UserRxBufferFS) );

	print(( char *)UserRxBufferFS);


	 return (uint32_t)strtoul(( char *)UserRxBufferFS, NULL, 10);
}


float scanfp(char *inputMessage){

	print(inputMessage);

	memset(UserRxBufferFS,'\0',strlen(( char *)UserRxBufferFS));
	while(strlen(( char *)UserRxBufferFS) == 0);

	size_t currentSize;
	do{
		currentSize = strlen(( char *)UserRxBufferFS);
		HAL_Delay(1);
	}while(currentSize != strlen(( char *)UserRxBufferFS) );

	print(( char *)UserRxBufferFS);

	return  strtof(( char *)UserRxBufferFS, NULL);

}

