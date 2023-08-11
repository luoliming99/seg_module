#ifndef __TMF8801_H
#define __TMF8801_H
#include "type.h"

#define NO_DATA_PROVIDE                 0         /**< ���ṩ���� */
#define FACTORY_CALIBRATION_PROVIDE     1         /**< �ṩ�˹���У׼���� */

#define GPIO0_INPUT                     0         /**< GPIO0 �������� */
/** GPIO0 Ϊ��ʱ���ܲ�����Ϊ��ʱ�������� */
#define GPIO0_LOW_STOP_HIGH_START       1
/** GPIO0 Ϊ��ʱ���ܲ�����Ϊ��ʱ�������� */
#define GPIO0_HIGH_STOP_LOW_START       2
#define GPIO0_OUTPUT                    3         /**< GPIO0 ������� */
#define GPIO0_OUTPUT_LOW                4         /**< GPIO0 ����� */
#define GPIO0_OUTPUT_HIGH               5         /**< GPIO0 ����� */

#define GPIO1_INPUT                    (0 << 4)   /**< GPIO1 �������� */
/** GPIO1 Ϊ��ʱ���ܲ�����Ϊ��ʱ�������� */
#define GPIO1_LOW_STOP_HIGH_START      (1 << 4)
/** GPIO1 Ϊ��ʱ���ܲ�����Ϊ��ʱ�������� */
#define GPIO1_HIGH_STOP_LOW_START      (2 << 4)
#define GPIO1_OUTPUT                   (3 << 4)   /**< GPIO1 ������� */
#define GPIO1_OUTPUT_LOW               (4 << 4)   /**< GPIO1 ����� */
#define GPIO1_OUTPUT_HIGH              (5 << 4)   /**< GPIO1 ����� */

/** \brief �����㷨�������ýṹ�嶨�� */
typedef struct app0_para_config
{
    /* ����У׼���㷨״̬���� */
    uint8_t calibration_state_mask;   
    
    /* GPIOx ���� */
    uint8_t gpio_control;
  
    /* GPIOx ������� */
    uint8_t gpio_output_control;
    
    /* ���������������� */
    uint8_t repetition_period_ms;
    
    /* �������� */
    uint16_t iterations;
} app0_para_config_t;  

/** \brief ����������ݽṹ�嶨�� */
typedef struct result_info
{
    uint8_t  result_num;             /* 0x20,      ��� ID */
    uint8_t  reliability;            /* 0x21[5:0], ���Ŷ� */
    uint16_t distance;               /* 0x22~0x23, ����(mm) */
    uint16_t distance_cal;           /* ƵƫУ����ľ���ֵ */
    uint32_t time_stamp;             /* 0x25~0x28, ʱ���(0.2us) */
    uint16_t xtalk;                  /* 0x30~0x31,���� */
    uint32_t reference_hits;         /* 0x33~0x36, ����ģʽ�£��ο� SPADs �����ӻ��������� */
    uint32_t object_hits;            /* 0x37~0x40, ����ģʽ�£�Ŀ�� SPADs �����ӻ��������� */
} result_info_t;

typedef int (*pfn_tmf8x01_i2c_read) (uint8_t   slave_addr,
                                     uint8_t   reg_addr,
                                     uint8_t  *p_data,
                                     uint32_t  nbytes) reentrant;
                                        
typedef int (*pfn_tmf8x01_i2c_write) (uint8_t   slave_addr,
                                      uint8_t   reg_addr,
                                      uint8_t  *p_data,
                                      uint32_t  nbytes) reentrant;
                                     
/**
 * \brief TMF8x01 ��ʼ��
 */
int tmf8x01_init (pfn_tmf8x01_i2c_read read, pfn_tmf8x01_i2c_write write);
                                      
/**
 * \brief ���������㷨
 *
 * \param[in]  p_config : �����㷨����
 */
int tmf8x01_app0_start (app0_para_config_t *p_config);

/**
 * \brief ��ȡ�������
 *
 * \param[out]  p_res : ��Ż�ȡ���Ĳ�����Ϣ
 */
int tmf8x01_result_get (result_info_t *p_res);

/**
 * \brief ֹͣ�����㷨
 */
int tmf8x01_app0_stop (void);

/**
 * \brief ִ�й�������ѧ��У׼
 */
int tmf8x01_factory_calibration (void);

/**
 * \brief ��ȡ�����㷨����Ĭ��ֵ
 *
 * \param[out]  p_config : �����㷨����
 */
void tmf8x01_app0_get_default_config (app0_para_config_t *p_config);

/**
 * \brief ʹ��TMF8x01�ж�
 */
void tmf8x01_intx_enable (void);

/**
 * \brief ��ȡTMF8x01�ж�״̬�Ĵ�����ֵ
 *
 * \return INT_STATUS�Ĵ�����ֵ
 */
uint8_t tmf8x01_int_status_get (void);

/**
 * \brief �����������жϱ�־λ
 *
 * \param[in]  intx           �ж�����
 *            -----------------------------
 *              1           ��������ж�          
 *              2            ֱ��ͼ�ж�      
 */
void tmf8x01_int_status_clr (uint8_t intx);

/**
 * \brief �жϷ�ʽ�£�TMF8x01�жϴ�����
 *
 * \param[out]  p_res : ��Ż�ȡ���Ĳ�����Ϣ
 */
void tmf8x01_handle_irq (result_info_t *p_res);

/**
 * \brief ����ӻ���ַ
 *
 * \param[in] old_addr : ��ǰ�ӻ���ַ 
 * \param[in] new_addr : �����õ��´ӻ���ַ       
 */
int tmf8801_addr_set (uint8_t old_addr, uint8_t new_addr);
    
#endif
