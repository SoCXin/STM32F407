
#include "common.h"
#include "flash_if.h"
#include "bootloader.h"
#include "kymodem.h"
#include "stmflash.h"
#include "rtc.h"


uint32_t iapbuf[512]; 	//2K字节缓存


//appxaddr:应用程序的起始地址
//appbuf:应用程序CODE.
//appsize:应用程序大小(字节).
void iap_write_appbin(uint32_t appxaddr,uint8_t *appbuf,uint32_t appsize)
{
    uint32_t t;
    uint16_t i=0;
    uint32_t temp;
    uint32_t fwaddr=appxaddr;//当前写入的地址
    uint8_t *dfu=appbuf;
    for(t=0; t<appsize; t+=4)
    {
        temp=(uint32_t)dfu[3]<<24;
        temp|=(uint32_t)dfu[2]<<16;
        temp|=(uint32_t)dfu[1]<<8;
        temp|=(uint32_t)dfu[0];
        dfu+=4;//偏移4个字节
        iapbuf[i++]=temp;
        if(i==512)
        {
            i=0;
            STMFLASH_Write(fwaddr,iapbuf,512);
            fwaddr+=2048;//偏移2048  512*4=2048
        }
    }
    if(i)STMFLASH_Write(fwaddr,iapbuf,i);//将最后的一些内容字节写进去.
}
/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
void iap_load_app(uint32_t addr)
{
    typedef  void (*pFunction)(void);
    if (((*(__IO uint32_t*)addr) & 0x2FFE0000 ) == 0x20000000)
    {
        // __disable_irq();     	//关总中断
        pFunction jump_app=(pFunction)*(__IO uint32_t*)(addr+4);
        __set_PSP(*(__IO uint32_t*) addr);
        __set_MSP(*(__IO uint32_t*) addr);
        __set_CONTROL(0);
        SCB->VTOR = addr;
        jump_app();
    }
}

/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
void sysReset(void)
{
    __disable_irq();
    NVIC_SystemReset();
}

/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
void sysUpdate(uint32_t chsum)
{
    Mark_Set(bkp_app1,USER_APP1_ADDRESS);
    Mark_Set(bkp_chsum,chsum);
    sysReset();
    // __disable_irq();
    // NVIC_SystemReset();
}
/******************************************************************************
**函数信息 ：
**功能描述 ：计算 CRC-16
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
uint16_t Modem_CRC16(uint8_t * buf, uint16_t len)
{
    uint16_t chsum = 0;
    uint8_t * in_ptr;
    in_ptr = buf;
    for (uint16_t stat = len ; stat > 0; stat--)     //len是所要计算的长度
    {
        chsum = chsum^(uint16_t)(*in_ptr++) << 8;
        for (uint8_t i=8; i!=0; i--)
        {
            if (chsum & 0x8000)
            {
                chsum = chsum << 1 ^ 0x1021;
            }
            else
            {
                chsum = chsum << 1;
            }
        }
    }
    return chsum;
}


/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
uint32_t Mark_Get(bkp_type flag)
{
    return HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0+flag);
}

/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
uint32_t Mark_Set(bkp_type flag, uint32_t val)
{
    HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0+flag,val);
    return HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0+flag);
}
