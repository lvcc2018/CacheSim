#ifndef COFIG
#define COFIG

typedef unsigned char bit_8;
typedef unsigned int bit_32;
typedef unsigned long long bit_64;

const bit_8 CACHE_VALID = 0x01;
const bit_8 CACHE_DIRTY = 0x02;
const bit_8 FLAG_B_MASK = 0xff;

const char READ_OPERATION = 'r';
const char WRITE_OPERATION = 'w';

// 记录支持的替换策略
enum replace_policy
{
    Random,
    LRU,
    FRU
};

// 记录替换结果
enum replace_result
{
    hit = 0,
    miss = 1,
    replace = 2,
    error = 3
};


#endif