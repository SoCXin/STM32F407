#include "ymodem.h"
#include "dev_com.h"
#include "usart.h"
#include <string.h>
Com_paser_BuffTypedef m_com_buf;

//#define UART3_W     HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_SET)
//#define UART3_R     HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_RESET)
void (*drv_com1_interrput)(char data);
void (*drv_com2_interrput)(char data);
void (*drv_com3_interrput)(char data);

/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
void drv_rx_interrput(char data)
{
    // 接收缓冲区添加数据
    m_com_buf.Rx_part[m_com_buf.Rx_write++] = data;
    // 如果达到末尾,写指针归零
    if(m_com_buf.Rx_write >= RX_BUFF_SIZE) m_com_buf.Rx_write = 0;
    // if(LL_USART_IsActiveFlag_RXNE(USART1))
	// {
    //     if(ReceiveCounter>=MODBUS_RECEIVE_BUFFER_SIZE) ReceiveCounter=0;
    //     ReceiveBuffer[ReceiveCounter++] = USART1->RDR;
    //     ModbusTimerValue=0;
	// }
}

/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
void (*drv_tx_byte)(char data);

void drv_com1_write(char data)
{
    #ifdef UART1_W
    UART1_W;
    #endif
	USART1->DR = data;
    while((USART1->SR&0X40)==0);
    #ifdef UART1_R
    UART1_R;
    #endif
}
void drv_com2_write(char data)
{
	USART2->DR = data;
    while((USART2->SR&0X40)==0);
}
void drv_com3_write(char data)
{
    #ifdef UART3_W
    UART3_W;
    #endif
	USART3->DR = data;
    while((USART3->SR&0X40)==0);
    #ifdef UART3_R
    UART3_R;
    #endif
}


/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
void dev_uart_init(uint32_t USARTx)
{
    memset(&m_com_buf,0,sizeof(Com_paser_BuffTypedef));
    switch(USARTx)
    {
        case 1:
            LL_USART_EnableIT_RXNE(USART1);
            LL_USART_EnableIT_PE(USART1);
            drv_com1_interrput = drv_rx_interrput;
            drv_tx_byte = drv_com1_write;
            break;
        case 2:
            LL_USART_EnableIT_RXNE(USART2);
            LL_USART_EnableIT_PE(USART2);
            drv_com2_interrput = drv_rx_interrput;
            drv_tx_byte = drv_com2_write;
            break;
        case 3:
            LL_USART_EnableIT_RXNE(USART3);
            LL_USART_EnableIT_PE(USART3);
            drv_com2_interrput = drv_rx_interrput;
            drv_tx_byte = drv_com3_write;
            #ifdef UART3_R
            UART3_R;
            #endif
            break;
    }
}

