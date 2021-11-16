
#include "dev_com.h"
#include "drv_com.h"
#include <string.h>
Com_paser_BuffTypedef m_com_buf;
// 接收回调函数
void (*m_com_rev_callBack)(unsigned char* data,uint32_t size) = 0;


/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/

static void dev_comctrl_buff_init(void)
{
	// 清空缓冲区
	memset(&m_com_buf,0,sizeof(Com_paser_BuffTypedef));
}

/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/

static void dev_comctrl_interrput_rx_handle(uint8_t data){
	// 接收缓冲区添加数据
	m_com_buf.Rx_part[m_com_buf.Rx_write++] = data;
	// 如果达到末尾,写指针归零
	if(m_com_buf.Rx_write >= RX_BUFF_SIZE)
		m_com_buf.Rx_write = 0;
}

/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
static void dev_comctrl_rx_handle(void)
{
    int offset_dir;
    uint32_t temp;
    // 获取偏差量
    offset_dir = m_com_buf.Rx_write - m_com_buf.Rx_read;

    if(offset_dir > 0)
    {
        temp = (offset_dir>=RX_DEAL_MAX_SIZE)?RX_DEAL_MAX_SIZE:offset_dir;

    }
    else if(offset_dir < 0)
    {
        temp = ((RX_BUFF_SIZE-m_com_buf.Rx_read)>=RX_DEAL_MAX_SIZE)?
                RX_DEAL_MAX_SIZE:RX_BUFF_SIZE-m_com_buf.Rx_read;
    }
    else
    {
        return;
    }
    if(m_com_rev_callBack !=0)
    {
        m_com_rev_callBack(&m_com_buf.Rx_part[m_com_buf.Rx_read],temp);
        m_com_buf.Rx_read+=temp;
        m_com_buf.Rx_read = (m_com_buf.Rx_read >= RX_BUFF_SIZE) ? 0 : m_com_buf.Rx_read;
    }
}
/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/

static void dev_comctrl_tx_handle(void)
{

}

/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
void dev_comctrl_init(void)
{
	dev_comctrl_buff_init();
	driver_com_regist_reccallback(1,dev_comctrl_interrput_rx_handle);
	//drv_com_printf(com1,"this is from com1");
}
/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
void dev_comctrl_handle(void)
{
    dev_comctrl_tx_handle();
    dev_comctrl_rx_handle();
}
/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
void dev_comctrl_regist_rx_callback(void (*arg_callBack)(unsigned char* data,uint32_t size))
{
	m_com_rev_callBack = arg_callBack;
}
