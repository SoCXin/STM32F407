
#include "drv_com.h"
#include "usart.h"

/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
void driver_com_init()
{

}
/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
void driver_com_regist_reccallback(uint32_t USARTx,void (*drv_com_m_handle)(unsigned char data))
{
	sys_com_regist_reccallback(USARTx,drv_com_m_handle);
}

/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
void drv_com1_write(uint8_t data)
{
	LL_USART_TransmitData8(USART1,data);
	while (LL_USART_IsActiveFlag_TXE(USART1)== RESET);
}
/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
void drv_com2_write(uint8_t data)
{
	LL_USART_TransmitData8(USART2,data);
}
/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
void drv_com3_write(uint8_t data)
{
	LL_USART_TransmitData8(USART3,data);
}
/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
void drv_com4_write(uint8_t data)
{
	LL_USART_TransmitData8(UART4,data);
}
/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
void drv_com1_printf(char *fmt, ...)
{
	char buffer[50];
	char *p = buffer;

	va_list arg_ptr;
	va_start(arg_ptr, fmt);
	vsnprintf(buffer, 50, fmt, arg_ptr);
	while(*p != '\0'){
	LL_USART_TransmitData8(USART1,(uint8_t)*p++);
	while (LL_USART_IsActiveFlag_TXE(USART1)== RESET);
	}
	va_end(arg_ptr);
}
/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
void drv_com2_printf(char *fmt, ...)
{
	char buffer[50];
	char *p = buffer;

	va_list arg_ptr;
	va_start(arg_ptr, fmt);
	vsnprintf(buffer, 50, fmt, arg_ptr);
	while(*p != '\0'){
	LL_USART_TransmitData8(USART2,(uint8_t)*p++);
	while (LL_USART_IsActiveFlag_TXE(USART2)== RESET);
	}
	va_end(arg_ptr);
}
/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
void drv_com3_printf(char *fmt, ...)
{
	char buffer[50];
	char *p = buffer;

	va_list arg_ptr;
	va_start(arg_ptr, fmt);
	vsnprintf(buffer, 50, fmt, arg_ptr);
	while(*p != '\0'){
	LL_USART_TransmitData8(USART3,(uint8_t)*p++);
	while (LL_USART_IsActiveFlag_TXE(USART3)== RESET);
	}
	va_end(arg_ptr);
}
/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
void drv_com4_printf(char *fmt, ...)
{
	char buffer[50];
	char *p = buffer;
	va_list arg_ptr;
	va_start(arg_ptr, fmt);
	vsnprintf(buffer, 50, fmt, arg_ptr);
	while(*p != '\0'){
	LL_USART_TransmitData8(UART4,(uint8_t)*p++);
	while (LL_USART_IsActiveFlag_TXE(UART4)== RESET);
	}
	va_end(arg_ptr);
}






