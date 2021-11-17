#ifndef __DEV_COMCTRL_H_
#define __DEV_COMCTRL_H_

#include "main.h"


// ���ͻ�����
#define TX_BUFF_SIZE (uint32_t)4096
// ���ջ�����
#define RX_BUFF_SIZE (uint32_t)4096
// ÿ�δ��������ֽ���
#define RX_DEAL_MAX_SIZE 2048

typedef struct
{
    unsigned char Tx_part[TX_BUFF_SIZE];
    unsigned int  Tx_read;
    unsigned int  Tx_write;
    unsigned char Rx_part[RX_BUFF_SIZE];
    unsigned int  Rx_write;
    unsigned int  Rx_read;
}Com_paser_BuffTypedef;

extern void driver_com_regist_reccallback(uint32_t USARTx,void (*drv_com_m_handle)(unsigned char data));
extern void dev_comctrl_regist_rx_callback(void (*arg_callBack)(unsigned char* data,uint32_t size));

extern void (*drv_com1_handle)(unsigned char data);
extern void (*drv_com2_handle)(unsigned char data);
extern void (*drv_com3_handle)(unsigned char data);
extern void sys_com_regist_reccallback(uint32_t USARTx,void (*drv_com_m_handle)(unsigned char data));

void driver_com_handle(void);
void drv_com1_write(uint8_t data);
void drv_com2_write(uint8_t data);
void drv_com3_write(uint8_t data);

// �˿ڿ�������ʼ��
void dev_comctrl_init(void);
void dev_comctrl_handle(void);

#endif
