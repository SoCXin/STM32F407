#ifndef __BOOTLOADER_H
#define __BOOTLOADER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define USER_APP1_ADDRESS   0x08004000
#define USER_APP2_ADDRESS   0x08008000

#define TX_TIMEOUT          ((uint32_t)100)
#define RX_TIMEOUT          HAL_MAX_DELAY

#define IS_CAP_LETTER(c)    (((c) >= 'A') && ((c) <= 'F'))
#define IS_LC_LETTER(c)     (((c) >= 'a') && ((c) <= 'f'))
#define IS_09(c)            (((c) >= '0') && ((c) <= '9'))
#define ISVALIDHEX(c)       (IS_CAP_LETTER(c) || IS_LC_LETTER(c) || IS_09(c))
#define ISVALIDDEC(c)       IS_09(c)
#define CONVERTDEC(c)       (c - '0')

#define CONVERTHEX_ALPHA(c) (IS_CAP_LETTER(c) ? ((c) - 'A'+10) : ((c) - 'a'+10))
#define CONVERTHEX(c)       (IS_09(c) ? ((c) - '0') : CONVERTHEX_ALPHA(c))


typedef enum
{
    bkp_rtc=0,
    bkp_app1,
    bkp_app2,
    bkp_boot,
    bkp_chsum,
    bkp_max=20
}bkp_type;

uint32_t Mark_Set(bkp_type flag, uint32_t val);
uint32_t Mark_Get(bkp_type flag);
void appjump(uint32_t appxaddr);
void sysUpdate(uint32_t chsum);
void sysReset(void);

#ifdef __cplusplus
}
#endif
#endif
/*------------------------- (C) COPYRIGHT 2021 OS-Q --------------------------*/
