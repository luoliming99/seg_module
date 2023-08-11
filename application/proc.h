#ifndef __PROC_H
#define __PROC_H

#include "type.h"

#define CMD_PARAM_LEN   (5 + 1)

typedef enum
{
    DE_SUCCESS = 0,
    DE_ERR_PARA_LEN,
} de_status_e;

typedef struct
{
    char cmd[CMD_PARAM_LEN];
    char para1[CMD_PARAM_LEN];
    char para2[CMD_PARAM_LEN];
    char para3[CMD_PARAM_LEN];
    char para4[CMD_PARAM_LEN];
} user_cmd_para_t;

typedef enum
{
    CMD_UNKNOWN = 0,
    CMD_DISP,
    CMD_FLICK,
    CMD_LIGHT,
    CMD_CODE,
    CMD_HELP,
} cmd_e;

typedef enum
{
    PARA_TO_INT = 0,
    PARA_TO_HEX,
} para_de_type_e;

typedef enum
{
    PARA1_UNKNOWN = 0,
    PARA1_INT,
    PARA1_HEX,
} para1_e;

typedef enum
{
    PARA2_UNKNOWN = 0,
    PARA2_INT,
    PARA2_HEX,
} para2_e;

typedef enum
{
    PARA3_UNKNOWN = 0,
    PARA3_INT,
    PARA3_HEX,
} para3_e;

typedef enum
{
    PARA4_UNKNOWN = 0,
    PARA4_INT,
    PARA4_HEX,
} para4_e;

typedef enum
{
    REPLY_OK = 0,
    REPLY_CMD_ERR,
    REPLY_PARA_ERR,
    REPLY_CMD_TOO_LONG,
    REPLY_HELP,
} reply_e;


void proc_rx_decode_exe(void);

#endif