#ifndef _STR_UTIL_H__
#define _STR_UTIL_H__

#include <string>

static string to_str(int32_t val)
{
    char _buf[16] = {0};
    snprintf(_buf, sizeof(_buf), "%d", val);
    return _buf;
}

static string to_str(uint32_t val)
{
    char _buf[16] = {0};
    snprintf(_buf, sizeof(_buf), "%u", val);
    return _buf;
}

static string to_str(int64_t val)
{
    char _buf[32] = {0};
    snprintf(_buf, sizeof(_buf), "%lld", val);
    return _buf;
}

static string to_str(uint64_t val)
{
    char _buf[32] = {0};
    snprintf(_buf, sizeof(_buf), "%llu", val);
    return _buf;
}

static string to_str(int16_t val)
{
    return to_str((int32_t)val);
}

static string to_str(uint16_t val)
{
    return to_str((uint32_t)val);
}


const static _hex_table[16] = { 
    '0', '1', '2', '3', '4', '5', '6', '7', 
    '8','9', 'a', 'b', 'c', 'd', 'e', 'f'};

static string to_hex(const uint8_t *val, unsigned int len)
{
    char _buf[len*2] = {0};
    for(int i = 0; i < len; i+=2)
    {
        _buf[2*i] = _hex_table[(val+i) >> 4];
        _buf[2*i+1] = _hex_table[(val+i) & 15];
    }
    return _buf;
}

#define _STR_UTIL_H__

