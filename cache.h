#ifndef CACHE
#define CACHE

#include <cstdlib>
#include <cstdio>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include "config.h"

// cacheline单位
struct cache_line
{
    bit_64 tag;
    bit_8 flag;
    bit_32 LRU_order;
};

struct cache_line_set
{
    struct cache_line* cache_lines;
    bit_8 binary_tree;
};

class cache
{
private:
    // 用来描述cache的特征
    // cache总大小，共有多少byte
    bit_64 cache_size;
    // cacheline大小
    bit_64 cache_line_size;
    // cacheline个数
    bit_64 cache_line_num;
    // cache路数，共有多少way
    bit_64 way_num;
    // cache组数，共有多少set
    bit_64 set_num;

    // 各index占用位数
    bit_64 tag_bit_num, index_bit_num, offset_bit_num;

    // cacheline指针数组
    cache_line_set *cache_line_sets;

    // 记录访问历史的计数器
    // 指令计数
    bit_64 order_count, read_order_count, write_order_count;
    // 访存计数
    bit_64 read_memory_count, write_memory_count, memory_count;
    // 命中与缺失计数
    bit_64 hit_count, miss_count, read_hit_count, write_hit_count, read_miss_count, write_miss_count;

    // 替换策略
    replace_policy rp;
    // 写策略
    bit_64 write_through;
    bit_64 write_allocation;

public:
    // 构造函数
    cache();
    // 析构函数
    ~cache();
    // cache 初始化
    void Init(bit_64 _cache_size, bit_64 _cache_line_size, bit_64 _way_num, replace_policy _rp, bit_64 _write_through, bit_64 _write_allocation);

    // 输出当前状态
    void show_info();

    // 检查命中
    int check_hit(bit_64 _set_base, bit_64 _addr);
    // 获取cache line set中空闲的line, 获取需要替换的cache line
    int get_free_cache_line(bit_64 _set_base);
    int get_victim(bit_64 _set_base, replace_policy _rp);
    // 更新cache
    void cache_update(bool hit, bit_64 _set_base, bit_64 _index, replace_policy _rp);

    // 读入指令
    bool cache_operation(bit_64 _addr, char _operation);
    // 读取文件
    void read_file(const char *_trace_filename, const char *_log_filename);

    // 获取数据
    void save_result(const char* _filename);

};

#endif
