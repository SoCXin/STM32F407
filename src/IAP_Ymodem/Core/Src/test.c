
#include "stmflash.h"
#include "kymodem.h"
#include "dev_com.h"
#include "drv_com.h"
#include "iap.h"
#include "flash_if.h"
#include "common.h"

#define USE_RX_MODE
#define KAPP_ADDR  (uint32_t)0x08004000
// __IO uint32_t g_JumpInit __attribute__((at(0x20001000), zero_init));

#include <stdio.h>
int fputc(int ch, FILE *f)
{
    LL_USART_TransmitData8(USART1,(uint8_t)ch);
    while (LL_USART_IsActiveFlag_TXE(USART1)== RESET);
    return ch;
}




/******************************************************************************
**函数信息 ：
**功能描述 ：串口错误回调
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
void g_ymodem_rx_error_handle(int error_code)
{
	printf("--error code :%d--\r\n",error_code);
}

/******************************************************************************
**函数信息 ：
**功能描述 ：接受数据回调
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
static uint32_t app_addr = KAPP_ADDR;
void g_ymodem_rx_data_handle(char *data, uint16_t len,uint32_t download_byte,uint8_t percent)
{
	printf("data:%d %d [%d]\r\n",len,download_byte,percent);
	if (FLASH_If_Write(app_addr,(uint32_t*) data,len/4) == FLASHIF_OK)
	{
		app_addr += len;
        // printf("data write ok\r\n");
	}
    else
    {
		printf("data write error\r\n");
	}
}
/******************************************************************************
**函数信息 ：
**功能描述 ：接受head回调
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
char g_ymodem_rx_head_handle(char *file_name,uint16_t file_name_len, uint32_t file_len)
{
    app_addr = KAPP_ADDR;
    printf("\r\nfile[%d]:%s\r\n",file_len,file_name);
    // FLASH_If_Erase(KAPP_ADDR);
    return 0;
}

/******************************************************************************
**函数信息 ：
**功能描述 ：接受完成回调
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
void g_ymodem_rx_finish_handle(int state)
{
    if(state ==0)
    {
        app_addr = KAPP_ADDR;
        printf("\r\n--file end--\r\n");
        iap_load_app(KAPP_ADDR);
    }
    else
    {
        printf("\r\n--file end error :%d--\r\n",state);
    }
}
/******************************************************************************
**函数信息 ：
**功能描述 ：发送数据处理
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
char name[] = "testupload.txt";
char file[] = "asdjlfaj129384719823749817239847198273498sdflajsldfjalsdjflasa134917239419823749817298347918237haksjdhfkahsdfkjhaskdjfhkahsd123456789";
//
void g_ymodem_tx_data_handle(uint8_t **file_read_addr, uint32_t  file_read_size, uint32_t file_has_read_size,  uint32_t file_remain_size,uint8_t percent)
{
    printf("read size:%d  has_read:%d  remain:%d  per:%d\r\n",file_read_size,file_has_read_size,  file_remain_size,percent);
    // 指针指向的地址 重新指向
    *file_read_addr = &file[file_has_read_size];
}

/******************************************************************************
**函数信息 ：
**功能描述 ：发送数据处理
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
void test(void)
{
	LL_USART_EnableIT_RXNE(USART1);
	LL_USART_EnableIT_PE(USART1);
	LL_USART_EnableIT_RXNE(USART2);
	LL_USART_EnableIT_PE(USART2);
	LL_USART_EnableIT_RXNE(USART3);
	LL_USART_EnableIT_PE(USART3);


	Ymodem_TypeDef ymodem;
	ymodem.ymodem_write_byte = drv_com2_write;
	ymodem.ymodem_rx_error_handle = g_ymodem_rx_error_handle;
	ymodem.ymodem_rx_head_handle = g_ymodem_rx_head_handle;
	ymodem.ymodem_rx_data_handle = g_ymodem_rx_data_handle;
	ymodem.ymodem_rx_finish_handle = g_ymodem_rx_finish_handle;
	ymodem.ymodem_tx_data_handle = g_ymodem_tx_data_handle;

	ymodem_init(&ymodem);
	printf("qitas IAP\r\n");
	FLASH_If_Init();
	ymodem_tx_init(name,sizeof(name),sizeof(file)-1);
    while (1)
    {
        dev_comctrl_handle();
        #ifdef USE_RX_MODE
        ymodem_rx_time_handle();
        #endif
    }
}
