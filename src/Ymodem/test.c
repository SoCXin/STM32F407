#include "main.h"
#include "ymodem.h"
#include "dev_com.h"
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>


#define USE_RX_MODE
#define KAPP_ADDR  (uint32_t)0x08004000

int fputc(int ch, FILE *f)
{
    while((USART1->SR&0X40)==0);
    USART1->DR = ch; //(uint8_t)
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
**功能描述 ：串口错误回调
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
uint32_t FLASH_Write(uint32_t FlashAddress, uint32_t* Data ,uint32_t DataLength)
{
    uint32_t i = 0;
    for (i = 0; (i < DataLength) && (FlashAddress <= (USER_FLASH_END_ADDRESS-4)); i++)
    {
        if (HAL_FLASH_Program(TYPEPROGRAM_WORD, FlashAddress, *(uint32_t*)(Data+i)) == HAL_OK)
        {
            if (*(uint32_t*)FlashAddress != *(uint32_t*)(Data+i))
            {
                return(FLASHIF_WRITINGCTRL_ERROR);
            }
            FlashAddress += 4;
        }
        else
        {
            return (FLASHIF_WRITING_ERROR);
        }
    }
    return (FLASHIF_OK);
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
    if (FLASH_Write(app_addr,(uint32_t*) data,len/4) == FLASHIF_OK)
    {
        app_addr += len;
        printf("data write ok\r\n");
    }
    else
    {
        printf("data write error\r\n");
    }
}
/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
static uint32_t GetSector(uint32_t Address)
{
    uint32_t sector = 0;
    if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
    {
        sector = FLASH_SECTOR_0;
    }
    else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
    {
        sector = FLASH_SECTOR_1;
    }
    else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
    {
        sector = FLASH_SECTOR_2;
    }
    else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
    {
        sector = FLASH_SECTOR_3;
    }
    else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
    {
        sector = FLASH_SECTOR_4;
    }
    else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
    {
        sector = FLASH_SECTOR_5;
    }
    else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
    {
        sector = FLASH_SECTOR_6;
    }
    else
    {
        sector = FLASH_SECTOR_7;
    }
    return sector;
}
/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
uint32_t FLASH_Erase(uint32_t StartSector)
{
    uint32_t SectorError;
    FLASH_EraseInitTypeDef pEraseInit;

    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                        FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

    pEraseInit.TypeErase = TYPEERASE_SECTORS;
    pEraseInit.Sector = GetSector(StartSector);
    pEraseInit.NbSectors = 1;
    pEraseInit.VoltageRange = VOLTAGE_RANGE_3;

    if (HAL_FLASHEx_Erase(&pEraseInit, &SectorError) != HAL_OK)return (1);
    return (0);
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
    printf("\r\nfile:%s %d\r\n",file_name,file_len);
    //FLASH_If_Erase(KAPP_ADDR);
    FLASH_Erase(KAPP_ADDR);
    return 0;
}

/******************************************************************************
**函数信息 ：
**功能描述 ：接受完成回调
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
void g_ymodem_iap_done_handle(int state)
{
    typedef void (*pFunction)(void);
    if(state == 0)
    {
        app_addr = KAPP_ADDR;
        // printf("\r\n--file end--\r\n");
        if (((*(__IO uint32_t*)app_addr) & 0x2FFE0000 ) == 0x20000000)
        {
            pFunction jump_app=(pFunction)*(__IO uint32_t*)(app_addr+4);
            __set_PSP(*(__IO uint32_t*) app_addr);
            __set_MSP(*(__IO uint32_t*) app_addr);
            jump_app();
        }
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
	// LL_USART_EnableIT_RXNE(USART1);
	// LL_USART_EnableIT_PE(USART1);
	// LL_USART_EnableIT_RXNE(USART2);
	// LL_USART_EnableIT_PE(USART2);

	Ymodem_TypeDef ymodem;
// //   ymodem.rx_interrupt=
// 	ymodem.ymodem_tx_byte = drv_com3_write;
    ymodem.ymodem_rx_callback = ymodem_rx_handle;
	ymodem.ymodem_rx_head_handle = g_ymodem_rx_head_handle;
	ymodem.ymodem_rx_data_handle = g_ymodem_rx_data_handle;
    ymodem.ymodem_rx_error_handle = g_ymodem_rx_error_handle;
	ymodem.rx_done_handle = g_ymodem_iap_done_handle;
	ymodem.ymodem_tx_data_handle = g_ymodem_tx_data_handle;
	ymodem_init(&ymodem);
	printf("test ymodem\r\n");
    while (1)
    {
        ymodem_loop();
    }
}
