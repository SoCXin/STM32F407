
#include "common.h"
#include "ymodem.h"
#include "rtc.h"


/******************************************************************************
**函数信息 ：
**功能描述 ：接受完成回调
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
void appjump(uint32_t addr)
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
