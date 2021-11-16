#ifndef __DEV_COMCTRL_H_
#define __DEV_COMCTRL_H_

#include "common.h"


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


// �˿ڿ�������ʼ��
void dev_comctrl_init(void);
void dev_comctrl_handle(void);
void dev_comctrl_regist_rx_callback(void (*arg_callBack)(unsigned char* data,uint32_t size));
#endif
