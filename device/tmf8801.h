#ifndef __TMF8801_H
#define __TMF8801_H
#include "type.h"

#define NO_DATA_PROVIDE                 0         /**< 不提供数据 */
#define FACTORY_CALIBRATION_PROVIDE     1         /**< 提供了工厂校准数据 */

#define GPIO0_INPUT                     0         /**< GPIO0 用作输入 */
/** GPIO0 为低时禁能测量，为高时启动测量 */
#define GPIO0_LOW_STOP_HIGH_START       1
/** GPIO0 为高时禁能测量，为低时启动测量 */
#define GPIO0_HIGH_STOP_LOW_START       2
#define GPIO0_OUTPUT                    3         /**< GPIO0 用作输出 */
#define GPIO0_OUTPUT_LOW                4         /**< GPIO0 输出低 */
#define GPIO0_OUTPUT_HIGH               5         /**< GPIO0 输出高 */

#define GPIO1_INPUT                    (0 << 4)   /**< GPIO1 用作输入 */
/** GPIO1 为低时禁能测量，为高时启动测量 */
#define GPIO1_LOW_STOP_HIGH_START      (1 << 4)
/** GPIO1 为高时禁能测量，为低时启动测量 */
#define GPIO1_HIGH_STOP_LOW_START      (2 << 4)
#define GPIO1_OUTPUT                   (3 << 4)   /**< GPIO1 用作输出 */
#define GPIO1_OUTPUT_LOW               (4 << 4)   /**< GPIO1 输出低 */
#define GPIO1_OUTPUT_HIGH              (5 << 4)   /**< GPIO1 输出高 */

/** \brief 测量算法参数配置结构体定义 */
typedef struct app0_para_config
{
    /* 工厂校准和算法状态掩码 */
    uint8_t calibration_state_mask;   
    
    /* GPIOx 控制 */
    uint8_t gpio_control;
  
    /* GPIOx 输出控制 */
    uint8_t gpio_output_control;
    
    /* 连续测量周期设置 */
    uint8_t repetition_period_ms;
    
    /* 迭代次数 */
    uint16_t iterations;
} app0_para_config_t;  

/** \brief 测量结果数据结构体定义 */
typedef struct result_info
{
    uint8_t  result_num;             /* 0x20,      结果 ID */
    uint8_t  reliability;            /* 0x21[5:0], 置信度 */
    uint16_t distance;               /* 0x22~0x23, 距离(mm) */
    uint16_t distance_cal;           /* 频偏校正后的距离值 */
    uint32_t time_stamp;             /* 0x25~0x28, 时间戳(0.2us) */
    uint16_t xtalk;                  /* 0x30~0x31,串扰 */
    uint32_t reference_hits;         /* 0x33~0x36, 距离模式下，参考 SPADs 被光子击中总数量 */
    uint32_t object_hits;            /* 0x37~0x40, 距离模式下，目标 SPADs 被光子击中总数量 */
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
 * \brief TMF8x01 初始化
 */
int tmf8x01_init (pfn_tmf8x01_i2c_read read, pfn_tmf8x01_i2c_write write);
                                      
/**
 * \brief 启动测量算法
 *
 * \param[in]  p_config : 测量算法参数
 */
int tmf8x01_app0_start (app0_para_config_t *p_config);

/**
 * \brief 获取测量结果
 *
 * \param[out]  p_res : 存放获取到的测量信息
 */
int tmf8x01_result_get (result_info_t *p_res);

/**
 * \brief 停止测量算法
 */
int tmf8x01_app0_stop (void);

/**
 * \brief 执行工厂（光学）校准
 */
int tmf8x01_factory_calibration (void);

/**
 * \brief 获取测量算法参数默认值
 *
 * \param[out]  p_config : 测量算法参数
 */
void tmf8x01_app0_get_default_config (app0_para_config_t *p_config);

/**
 * \brief 使能TMF8x01中断
 */
void tmf8x01_intx_enable (void);

/**
 * \brief 获取TMF8x01中断状态寄存器的值
 *
 * \return INT_STATUS寄存器的值
 */
uint8_t tmf8x01_int_status_get (void);

/**
 * \brief 清除测量结果中断标志位
 *
 * \param[in]  intx           中断类型
 *            -----------------------------
 *              1           测量结果中断          
 *              2            直方图中断      
 */
void tmf8x01_int_status_clr (uint8_t intx);

/**
 * \brief 中断方式下，TMF8x01中断处理函数
 *
 * \param[out]  p_res : 存放获取到的测量信息
 */
void tmf8x01_handle_irq (result_info_t *p_res);

/**
 * \brief 重设从机地址
 *
 * \param[in] old_addr : 当前从机地址 
 * \param[in] new_addr : 待设置的新从机地址       
 */
int tmf8801_addr_set (uint8_t old_addr, uint8_t new_addr);
    
#endif
