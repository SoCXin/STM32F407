#include "ymodem.h"
#include "dev_com.h"
#include "usart.h"
#include <string.h>
Com_paser_BuffTypedef m_com_buf;


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
	USART1->DR = data;
    while((USART1->SR&0X40)==0);
	// LL_USART_TransmitData8(USART1,data);
	// while (LL_USART_IsActiveFlag_TXE(USART1)== RESET);
}
void drv_com2_write(char data)
{
	USART2->DR = data;
    while((USART2->SR&0X40)==0);
	// LL_USART_TransmitData8(USART2,data);
	// while (LL_USART_IsActiveFlag_TXE(USART2)== RESET);
}
void drv_com3_write(char data)
{
	USART3->DR = data;
    while((USART3->SR&0X40)==0);
	// LL_USART_TransmitData8(USART3,data);
	// while (LL_USART_IsActiveFlag_TXE(USART3)== RESET);
}

/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
void (*drv_com1_interrput)(char data);
void (*drv_com2_interrput)(char data);
void (*drv_com3_interrput)(char data);

void com_regist_callback(uint32_t USARTx)
{
    switch(USARTx)
    {
        case 1:
            drv_com1_interrput = drv_rx_interrput;
            drv_tx_byte = drv_com1_write;
            break;
        case 2:
            drv_com2_interrput = drv_rx_interrput;
            drv_tx_byte = drv_com2_write;
            break;
        case 3:
            drv_com2_interrput = drv_rx_interrput;
            drv_tx_byte = drv_com3_write;
            break;
    }
}

/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
void dev_uart_init(void)
{
    memset(&m_com_buf,0,sizeof(Com_paser_BuffTypedef));
    com_regist_callback(2);
}

