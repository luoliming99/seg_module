#include "proc.h"
#include "string.h"     /* memset, strcmp */
#include "disp.h"
#include "timer.h"
#include "uart.h"

char code _str_reply_ok[] = {"ok\r\n"};
char code _str_reply_cmd_err[] = {"cmd error\r\n"};
char code _str_reply_para_err[] = {"para error\r\n"};
char code _str_reply_cmd_too_long[] = {"cmd too long\r\n"};
char code _str_reply_help[] = {"Kong YiJi V1.0.01.000 command usage:\r\n\
\tDISP [string]\r\n\
\tFLICK [cycle1] [cycle2] [cycle3] [cycle4]\r\n\
\tLIGHT [level1] [level2] [level3] [level4]\r\n\
\tCODE [hex1] [hex2] [hex3] [hex4]\r\n"};

user_cmd_para_t _user_cmd;

de_status_e _rx_decode(void)
{
    uint8_t i0, i1, i2, i3, i4, j;
    
    memset(&_user_cmd, 0, sizeof(user_cmd_para_t));
    
    for (i0 = 0; i0 < CMD_PARAM_LEN && g_rx_buf[i0] != ' ' && g_rx_buf[i0] != '\0'; i0++)
    {
        _user_cmd.cmd[i0] = g_rx_buf[i0];
    }
    if (g_rx_buf[i0] == ' ')
    {
        i0++;
    }
    else if (g_rx_buf[i0] == '\0')
    {
        return DE_SUCCESS;
    }
    else
    {
        return DE_ERR_PARA_LEN;
    }
    
    j = i0;
    for (i1 = 0; i1 < CMD_PARAM_LEN && g_rx_buf[j+i1] != ' ' && g_rx_buf[j+i1] != '\0'; i1++)
    {
        _user_cmd.para1[i1] = g_rx_buf[j+i1];
    }
    if (g_rx_buf[j+i1] == ' ')
    {
        i1++;
    }
    else if (g_rx_buf[j+i1] == '\0')
    {
        return DE_SUCCESS;
    }
    else
    {
        return DE_ERR_PARA_LEN;
    }
    
    j += i1;
    for (i2 = 0; i2 < CMD_PARAM_LEN && g_rx_buf[j+i2] != ' ' && g_rx_buf[j+i2] != '\0'; i2++)
    {
        _user_cmd.para2[i2] = g_rx_buf[j+i2];
    }
    if (g_rx_buf[j+i2] == ' ')
    {
        i2++;
    }
    else if (g_rx_buf[j+i2] == '\0')
    {
        return DE_SUCCESS;
    }
    else
    {
        return DE_ERR_PARA_LEN;
    }
    
    j += i2;
    for (i3 = 0; i3 < CMD_PARAM_LEN && g_rx_buf[j+i3] != ' ' && g_rx_buf[j+i3] != '\0'; i3++)
    {
        _user_cmd.para3[i3] = g_rx_buf[j+i3];
    }
    if (g_rx_buf[j+i3] == ' ')
    {
        i3++;
    }
    else if (g_rx_buf[j+i3] == '\0')
    {
        return DE_SUCCESS;
    }
    else
    {
        return DE_ERR_PARA_LEN;
    }
    
    j += i3;
    for (i4 = 0; i4 < CMD_PARAM_LEN && g_rx_buf[j+i4] != ' ' && g_rx_buf[j+i4] != '\0'; i4++)
    {
        _user_cmd.para4[i4] = g_rx_buf[j+i4];
    }
    if (i4 == CMD_PARAM_LEN)
    {
        return DE_ERR_PARA_LEN;
    }
    
//    print_string(_user_cmd.cmd);
//    print_string("\r\n");
//    print_string(_user_cmd.para1);
//    print_string("\r\n");
//    print_string(_user_cmd.para2);
//    print_string("\r\n");
//    print_string(_user_cmd.para3);
//    print_string("\r\n");
//    print_string(_user_cmd.para4);
//    print_string("\r\n");
    return DE_SUCCESS;
}

void _strupr(char *str)
{
    for (; *str != '\0'; str++)
    {
        if (*str >= 'a' && *str <= 'z')
        {
            *str -= 32;
        }
    }
}

uint8_t _atoi(char *str, uint16_t *num)
{
    uint8_t i = 0;
    
    *num = 0;
    while (*(str + i) != '\0')
    {
        if (*(str + i) >= '0' && *(str + i) <= '9')
        {
            *num *= 10;
            *num += *(str + i) - '0';
            i++; 
            if (i > 4)  /* 只检测4位长度 */
            {
                return 0;
            }
            continue;
        }

        return 0;    /* the string contains char not belongs to 0-9 */
    }
    return 1;
}

uint8_t _atohex(char *str, uint8_t *hex)
{
    uint8_t i = 0;
    
    *hex = 0;
    while (*(str + i) != '\0')
    {
        if (*(str + i) >= '0' && *(str + i) <= '9' ||
            *(str + i) >= 'A' && *(str + i) <= 'F' ||
            *(str + i) >= 'a' && *(str + i) <= 'f')
        {
            *hex <<= 4;
            if (*(str + i) >= '0' && *(str + i) <= '9')
            {
                *hex += *(str + i) - '0';
            }
            else if (*(str + i) >= 'A' && *(str + i) <= 'F')
            {
                *hex += *(str + i) - 'A' + 10;
            }
            else
            {
                *hex += *(str + i) - 'a' + 10;
            }
            i++; 
            if (i > 2)  /* 只检测2位长度 */
            {
                return 0;
            }
            continue;
        }

        return 0;    /* the string contains char not belongs to 0-9 or A-F */
    }
    return 1;
}

cmd_e _cmd_decode(void)
{
    char *cmd = NULL;
    
    _strupr(_user_cmd.cmd);   /* 小写转换为大写 */

    if (0 == strcmp("DISP", _user_cmd.cmd))
    {
        return CMD_DISP;
    }
    else if (0 == strcmp("FLICK", _user_cmd.cmd))
    {
        return CMD_FLICK;
    }
    else if (0 == strcmp("LIGHT", _user_cmd.cmd))
    {
        return CMD_LIGHT;
    }
    else if (0 == strcmp("CODE", _user_cmd.cmd))
    {
        return CMD_CODE;
    }
    else if (0 == strcmp("HELP", _user_cmd.cmd))
    {
        return CMD_HELP;
    }
    
    return CMD_UNKNOWN;
}

para1_e _para1_decode(para_de_type_e type, uint16_t *para1)
{
    if (PARA_TO_INT == type && _atoi(_user_cmd.para1, para1))
    {
        return PARA1_INT;
    }
    else if (PARA_TO_HEX == type && _atohex(_user_cmd.para1, (uint8_t *)para1))
    {
        return PARA1_HEX;
    }

    return PARA1_UNKNOWN;
}

para2_e _para2_decode(para_de_type_e type, uint16_t *para2)
{
    if (PARA_TO_INT == type && _atoi(_user_cmd.para2, para2))
    {
        return PARA2_INT;
    }
    else if (PARA_TO_HEX == type && _atohex(_user_cmd.para2, (uint8_t *)para2))
    {
        return PARA2_HEX;
    }

    return PARA2_UNKNOWN;
}

para3_e _para3_decode(para_de_type_e type, uint16_t *para3)
{
    if (PARA_TO_INT == type && _atoi(_user_cmd.para3, para3))
    {
        return PARA3_INT;
    }
    else if (PARA_TO_HEX == type && _atohex(_user_cmd.para3, (uint8_t *)para3))
    {
        return PARA3_HEX;
    }

    return PARA3_UNKNOWN;
}

para4_e _para4_decode(para_de_type_e type, uint16_t *para4)
{
    if (PARA_TO_INT == type && _atoi(_user_cmd.para4, para4))
    {
        return PARA4_INT;
    }
    else if (PARA_TO_HEX == type && _atohex(_user_cmd.para4, (uint8_t *)para4))
    {
        return PARA4_HEX;
    }

    return PARA4_UNKNOWN;
}

void _cmd_reply(reply_e de_status)
{
    switch (de_status)
    {
        case REPLY_OK:
            print_string(_str_reply_ok);
        break;
        
        case REPLY_CMD_ERR:
            print_string(_str_reply_cmd_err);
        break;
        
        case REPLY_PARA_ERR:
            print_string(_str_reply_para_err);
        break;
        
        case REPLY_CMD_TOO_LONG:
            print_string(_str_reply_cmd_too_long);
        break;
        
        case REPLY_HELP:
            print_string(_str_reply_help);
        break;
        
        default: break;
    }
}

void proc_rx_decode_exe(void) 
{
    de_status_e     status      = DE_SUCCESS;
    cmd_e           cmd         = CMD_UNKNOWN;
    para1_e         para1_type  = PARA1_UNKNOWN;
    uint16_t        para1_val   = 0;
    para2_e         para2_type  = PARA2_UNKNOWN;
    uint16_t        para2_val   = 0;
    para3_e         para3_type  = PARA3_UNKNOWN;
    uint16_t        para3_val   = 0;
    para4_e         para4_type  = PARA4_UNKNOWN;
    uint16_t        para4_val   = 0;
    
    uint16_t flick[] = {0};
    uint8_t  light[] = {0};
    uint8_t  seg[]   = {0};
    
    status = _rx_decode();
    
    switch (status)
    {
        case DE_SUCCESS:
        {
            cmd = _cmd_decode();
        
            switch (cmd)
            {
                case CMD_DISP:
                    disp_set_string(_user_cmd.para1);
                break;
                
                case CMD_FLICK:
                    para1_type = _para1_decode(PARA_TO_INT, &para1_val);
                    para2_type = _para2_decode(PARA_TO_INT, &para2_val);
                    para3_type = _para3_decode(PARA_TO_INT, &para3_val);
                    para4_type = _para4_decode(PARA_TO_INT, &para4_val);
                    if (PARA1_INT == para1_type && PARA2_INT == para2_type && PARA3_INT == para3_type && PARA4_INT == para4_type)
                    {
                        if ((para1_val == 0 || (para1_val >= 100 && para1_val <= 2000)) &&
                            (para2_val == 0 || (para2_val >= 100 && para2_val <= 2000)) &&
                            (para3_val == 0 || (para3_val >= 100 && para3_val <= 2000)) &&
                            (para4_val == 0 || (para4_val >= 100 && para4_val <= 2000)))
                        {
                            flick[0] = para1_val;
                            flick[1] = para2_val;
                            flick[2] = para3_val;
                            flick[3] = para4_val;
                            disp_set_flick_half_cycle(flick);
                        }
                        else
                        {
                            _cmd_reply(REPLY_PARA_ERR);
                            return;
                        }
                    }
                    else
                    {
                        _cmd_reply(REPLY_PARA_ERR);
                        return;
                    }
                break;
                
                case CMD_LIGHT:
                    para1_type = _para1_decode(PARA_TO_INT, &para1_val);
                    para2_type = _para2_decode(PARA_TO_INT, &para2_val);
                    para3_type = _para3_decode(PARA_TO_INT, &para3_val);
                    para4_type = _para4_decode(PARA_TO_INT, &para4_val);
                    if (PARA1_INT == para1_type && PARA2_INT == para2_type && PARA3_INT == para3_type && PARA4_INT == para4_type)
                    {
                        if (para1_val <= 10 && para2_val <= 10 && para3_val <= 10 && para4_val <= 10)
                        {
                            light[0] = (uint8_t)para1_val;
                            light[1] = (uint8_t)para2_val;
                            light[2] = (uint8_t)para3_val;
                            light[3] = (uint8_t)para4_val;
                            disp_set_light_level(light);
                        }
                        else
                        {
                            _cmd_reply(REPLY_PARA_ERR);
                            return;
                        }
                    }
                    else
                    {
                        _cmd_reply(REPLY_PARA_ERR);
                        return;
                    }
                break;
                    
                case CMD_CODE:
                    para1_type = _para1_decode(PARA_TO_HEX, &para1_val);
                    para2_type = _para2_decode(PARA_TO_HEX, &para2_val);
                    para3_type = _para3_decode(PARA_TO_HEX, &para3_val);
                    para4_type = _para4_decode(PARA_TO_HEX, &para4_val);
                    if (PARA1_HEX == para1_type && PARA2_HEX == para2_type && PARA3_HEX == para3_type && PARA4_HEX == para4_type)
                    {
                        seg[0] = (para1_val & 0xFF00) >> 8;
                        seg[1] = (para2_val & 0xFF00) >> 8;
                        seg[2] = (para3_val & 0xFF00) >> 8;
                        seg[3] = (para4_val & 0xFF00) >> 8;
                        disp_set_code(seg);
                    }
                    else
                    {
                        _cmd_reply(REPLY_PARA_ERR);
                        return;
                    }
                break;
                    
                case CMD_HELP:
                    _cmd_reply(REPLY_HELP);
                    return;
                break;
                    
                case CMD_UNKNOWN:
                    _cmd_reply(REPLY_CMD_ERR);
                    return;
                break;
            }
        }
        break;
        
        case DE_ERR_PARA_LEN:
            _cmd_reply(REPLY_CMD_TOO_LONG);
            return;
        break;
        
        default: break;
    }
    
    _cmd_reply(REPLY_OK);
}