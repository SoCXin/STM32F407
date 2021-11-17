#include "iap.h"


#include "stmflash.h"

typedef  void (*iapfun)(void);
iapfun jump_app;
uint32_t iapbuf[512]; 	//2K字节缓存

__asm void MSR_MSP(uint32_t addr)
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

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

//跳转到应用程序段
//appxaddr:用户代码起始地址.
void iap_load_app(uint32_t appxaddr)
{
    // 判断栈顶地址值是否在0x2000 0000 - 0x 2000 2000之间
    //appxaddr是用户APP程序开始的地址，我们会把APP程序的中断向量表放置在appxaddr开始的位置；而中断向量表里第一个放的就是栈顶地址的值
    //这句话通过判断栈顶地址值是否正确（是否在0x2000 0000 - 0x 2000 2000之间）
    //这是由于应用程序的启动文件刚开始就会去初始化栈空间，如果栈顶值对了，说明应用程已经下载了，启动文件的初始化也执行了。
    if(((*(__IO uint32_t*)appxaddr)&0x2FF00000)==0x20000000)	//检查栈顶地址是否合法.
    {
        // (vu32* )(appxaddr+4)里面放的是中断向量表的第二项“复位地址”
			 //iapfun的别名，定义为typedef void (* iapfun)(void); 即 iapfun是类型 void (*)(void)函数指针 的一个别名
			//所以此时，jump2app指向APP复位函数所在的地址
        jump_app=(iapfun)*(__IO uint32_t*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)
        MSR_MSP(*(__IO uint32_t*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
        jump_app();									//跳转到APP.
    }
}

