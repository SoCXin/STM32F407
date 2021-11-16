/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : dev_comctrl.c
 * Author    : ygl
 * Brief     : �˿ڹ���
 * Date      : 2018.11.18
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/
 
#include "dev_com.h"
#include "drv_com.h"


// �Զ���Ļ���
Com_paser_BuffTypedef m_com_buf;
// ���ջص�����
void (*m_com_rev_callBack)(unsigned char* data,uint32_t size) = 0;

static void dev_comctrl_buff_init(void);
static void dev_comctrl_interrput_rx_handle(uint8_t data);
static void dev_comctrl_tx_handle(void);
static void dev_comctrl_rx_handle(void);

/**
* @ Function Name : dev_comctrl_init
* @ Author        : ygl
* @ Brief         : �˿ڿ�������ʼ��
* @ Date          : 2018.11.18
* @ Modify        : ...
**/
void dev_comctrl_init(void)
{	
	dev_comctrl_buff_init();
	driver_com_regist_reccallback(1,dev_comctrl_interrput_rx_handle);
	//drv_com_printf(com1,"this is from com1");
}
/**
* @ Function Name : dev_comctrl_handle
* @ Author        : ygl
* @ Brief         : �˿ڿ�����������
* @ Date          : 2018.11.18
* @ Modify        : ...
**/
void dev_comctrl_handle(void){
	 dev_comctrl_tx_handle();
	 dev_comctrl_rx_handle();
}
/**
* @ Function Name : dev_comctrl_handle
* @ Author        : ygl
* @ Brief         : �˿ڿ�����ע����ջص�����
* @ Date          : 2018.11.18
* @ Modify        : ...
**/
void dev_comctrl_regist_rx_callback(void (*arg_callBack)(unsigned char* data,uint32_t size)){
	m_com_rev_callBack = arg_callBack;
}
/**
* @ Function Name : dev_comctrl_buff_init
* @ Author        : ygl
* @ Brief         : ���ݳ�ʼ��
* @ Date          : 2018.11.18
* @ Modify        : ...
**/
static void dev_comctrl_buff_init(void){
	// ��ջ�����
	memset(&m_com_buf,0,sizeof(Com_paser_BuffTypedef));
}


/**
* @ Function Name : dev_comctrl_rx_handle(u8 data)
* @ Author        : ygl
* @ Brief         : �����жϽ��պ���
* @ Date          : 2018.11.18
* @ Modify        : ...
**/
static void dev_comctrl_interrput_rx_handle(uint8_t data){
	// ���ջ������������
	m_com_buf.Rx_part[m_com_buf.Rx_write++] = data;
	// ����ﵽĩβ,дָ�����
	if(m_com_buf.Rx_write >= RX_BUFF_SIZE)
		m_com_buf.Rx_write = 0;
}


/**
* @ Function Name : dev_comctrl_handle
* @ Author        : ygl
* @ Brief         : �˿ڿ��������մ���
* @ Date          : 2018.11.18
* @ Modify        : ...
**/
static void dev_comctrl_rx_handle(void){
	  int offset_dir;
    uint32_t temp;
    // ��ȡƫ����
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

/**
* @ Function Name : dev_comctrl_handle
* @ Author        : ygl
* @ Brief         : �˿ڿ��������ʹ�����
* @ Date          : 2018.11.18
* @ Modify        : ...
**/
static void dev_comctrl_tx_handle(void){
	
}

