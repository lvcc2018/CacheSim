#include "cache.h"
#include <assert.h>

unsigned long long log_two(unsigned long long natural)
{
    unsigned long long count = 0;
    unsigned long long sum = 1;
    while (1)
    {
        sum <<= 1;
        count += 1;
        if (sum >= natural)
        {
            return count;
        }
    }
}

cache::cache() {}

// cache 初始化
void cache::Init(bit_64 _cache_size, bit_64 _cache_line_size, bit_64 _way_num, replace_policy _rp, bit_64 _write_through, bit_64 _write_allocation)
{
    printf("Cache Sim Initializing...");
    // cache 初始设置
    cache_size = _cache_size;
    cache_line_size = _cache_line_size;
    way_num = _way_num;
    rp = _rp;
    cache_line_num = cache_size / cache_line_size;
    set_num = cache_line_num / way_num;
    write_through = _write_through;
    write_allocation = _write_allocation;

    // 计算各个位数
    index_bit_num = log_two(set_num);
    offset_bit_num = log_two(cache_line_size);
    tag_bit_num = 64 - index_bit_num - offset_bit_num;
    order_count = read_order_count = write_order_count = 0;
    read_memory_count = write_memory_count = memory_count = 0;
    hit_count = miss_count = read_hit_count = write_hit_count = read_miss_count = write_miss_count = 0;

    // 分配cache空间
    cache_lines = (cache_line *)malloc(sizeof(cache_line) * cache_line_num);
    memset(cache_lines, 0, sizeof(cache_line) * cache_line_num);

    for (int i = 0; i < set_num; i++)
    {
        for (int j = 0; j < way_num; j++)
        // 初始化cache line LRU顺序
        {
            cache_lines[i * way_num + j].LRU_order = j;
        }
    }

    printf("Success!\n\n");
    show_info();
}
cache::~cache()
{
    free(cache_lines);
}

void cache::show_info()
{
    printf("Cache Sim Config:\n");
    printf("Cache size:         %llu KB\n", (cache_size >> 10));
    printf("Cache line size:    %llu B\n", cache_line_size);
    printf("Cache ways num:     %llu ways\n", way_num);
    printf("Cache line num:     %llu cachelines\n", cache_line_num);
    printf("Cache write policy: ");
    printf(write_through ? "Write through + " : "Write back + ");
    printf(write_allocation ? "Write allocation\n" : "Write not allocation\n");
    printf("Address Analysis:\n");
    printf("Tag (%llu bits) | Index (%llu bits) | offset (%llu bits) \n", tag_bit_num, index_bit_num, offset_bit_num);
}

// 检查在当前set中是否命中
int cache::check_hit(bit_64 _set_base, bit_64 _addr)
{
    assert((_set_base >= 0) && (_set_base <= (set_num - 1) * way_num));
    for (bit_64 i = 0; i < way_num; i++)
    {
        if ((cache_lines[_set_base + i].flag & CACHE_VALID) &&
            (cache_lines[_set_base + i].tag == (_addr >> (offset_bit_num + index_bit_num))))
        {
            // 找到后返回在组中的偏移
            return i;
        }
    }
    // 没有找到
    return -1;
}
// 获取cache line set中空闲的line
bit_64 cache::get_free_cache_line(bit_64 _set_base)
{
    assert((_set_base >= 0) && (_set_base <= (set_num - 1) * way_num));
    for (bit_64 i = 0; i < way_num; i++)
    {
        if (!(cache_lines[_set_base + i].flag & CACHE_VALID))
        {
            // 找到后返回在组中的偏移
            return i;
        }
    }
    // 没有找到
    return -1;
}

bit_64 cache::get_victim(bit_64 _set_base, replace_policy _rp)
{
    printf("%d", _rp);
    assert((_set_base >= 0) && (_set_base <= (set_num - 1) * way_num));
    int victim_index;
    int max_order, min;
    victim_index = get_free_cache_line(_set_base);
    if (victim_index >= 0)
    {
        return victim_index;
    }
    switch (_rp)
    {
    case Random:
    {
        victim_index = rand() % way_num;
        break;
    }
    case LRU:
    {
        max_order = 0;
        for (bit_64 i = 0; i < way_num; i++)
        {
            if (cache_lines[_set_base + i].LRU_order > max_order)
            {
                max_order = cache_lines[_set_base + i].LRU_order;
                victim_index = i;
            }
        }
        break;
    }
    default:
    {
        printf("Wrong replace policy...");
        break;
    }
    }
    if (victim_index >= 0)
    {
        if (cache_lines[_set_base + victim_index].flag & CACHE_DIRTY)
        {
            cache_lines[_set_base + victim_index].flag &= ~CACHE_DIRTY;
            write_memory_count++;
        }
    }
    assert(victim_index >= 0 && victim_index < way_num);
    return victim_index;
}

// 更新cache相关状态
void cache::cache_update(bool hit, bit_64 _set_base, bit_64 _index, replace_policy _rp)
{
    assert((_set_base >= 0) && (_set_base <= (set_num - 1) * way_num));
    assert(_index >= 0 && _index < way_num);
    if (hit) // 命中
    {
        switch (_rp)
        {
        case Random:
            break;
        case LRU:
            for (bit_64 i = 0; i < way_num; i++)
            {
                if (cache_lines[_set_base + i].LRU_order < cache_lines[_set_base + _index].LRU_order)
                    cache_lines[_set_base + i].LRU_order++;
            }
            cache_lines[_set_base + _index].LRU_order = 0;

            break;
        default:
            printf("Wrong replace policy...");
            break;
        }
    }
    else // 未命中
    {
        switch (_rp)
        {
        case Random:
            break;
        case LRU:
        {
            for (bit_64 i = 0; i < way_num; i++)
            {
                if (cache_lines[_set_base + i].LRU_order < cache_lines[_set_base + _index].LRU_order)
                    cache_lines[_set_base + i].LRU_order++;
            }
            cache_lines[_set_base + _index].LRU_order = 0;
            break;
        }
        default:
            printf("Wrong replace policy...");
            break;
        }
    }
}

// 读入指令
void cache::cache_operation(bit_64 _addr, char _operation)
{
    order_count++;
    if (_operation == 'r')
        read_order_count++;
    if (_operation == 'w')
        write_order_count++;

    bit_64 set_base = ((_addr >> (offset_bit_num)) % set_num) * way_num;
    int cache_index = check_hit(set_base, _addr);
    int victim_index = get_victim(set_base, rp);

    if (_operation == 'w') // 写操作
    {
        if (cache_index >= 0) // 写操作命中
        {
            printf("Hit!\n");
            hit_count++;
            write_hit_count++;
            if (write_through) // 写通
            {
                memory_count++;
                write_memory_count++;
            }
            else // 写回
            {
                cache_lines[set_base + cache_index].flag |= CACHE_DIRTY;
            }
            cache_update(true, set_base, cache_index, rp);
        }
        else // 写操作未命中
        {
            printf("Miss! ");
            miss_count++;
            write_miss_count++;
            if (write_allocation) // 写分配
            {
                memory_count++;
                read_memory_count++;
                cache_lines[set_base + victim_index].tag = _addr >> (index_bit_num + offset_bit_num);
                cache_lines[set_base + victim_index].flag = (bit_8)~FLAG_B_MASK;
                cache_lines[set_base + victim_index].flag |= CACHE_VALID;
                if (write_through)
                {
                    write_memory_count++;
                }
                else
                {
                    cache_lines[set_base + victim_index].flag |= CACHE_DIRTY;
                }
                printf("Replace the %d way cacheline\n", victim_index);
                cache_update(false, set_base, victim_index, rp);
            }
            else // 写不分配
            {
                printf("\n");
                memory_count++;
                write_memory_count++;
            }
        }
    }
    else
    {
        if (cache_index >= 0) // 命中
        {
            printf("Hit!\n");
            hit_count++;
            read_hit_count++;
            cache_update(true, set_base, cache_index, rp);
        }
        else // 不命中
        {
            printf("Miss! ");
            miss_count++;
            read_miss_count++;
            memory_count++;
            read_memory_count++;
            cache_lines[set_base + victim_index].tag = _addr >> (index_bit_num + offset_bit_num);
            cache_lines[set_base + victim_index].flag = (bit_8)~FLAG_B_MASK;
            cache_lines[set_base + victim_index].flag |= CACHE_VALID;
            printf("Replace the %d way cacheline\n", victim_index);
            cache_update(false, set_base, victim_index, rp);
        }
    }
}

// 读取文件
void cache::read_file(const char *_filename)
{
    FILE *infile = fopen(_filename, "r");
    char act;
    bit_64 addr;

    while (fscanf(infile, "%c 0x%llx\n", &act, &addr) == 2)
    {
        switch (act)
        {
        case 'r':
            printf("Read from addr: %llx ", addr);
            break;
        case 'w':
            printf("Write to addr:  %llx ", addr);
            break;
        default:
            continue;
        }
        cache_operation(addr, act);
    }

    fclose(infile);
}

// 获取数据
void cache::save_rate()
{
    printf("\n");
    show_info();

    printf("Order count :               %llu\n", order_count);
    printf("Read order count :          %llu\n", read_order_count);
    printf("Write order count :         %llu\n\n", write_order_count);

    printf("Access memory count :       %llu\n", memory_count);
    printf("Read memory count :         %llu\n", read_memory_count);
    printf("Write memory count :        %llu\n\n", write_memory_count);

    printf("Hit count :                 %llu\n", hit_count);
    printf("Miss count :                %llu\n", miss_count);
    printf("Read hit count :            %llu\n", read_hit_count);
    printf("Read miss count :           %llu\n", read_miss_count);
    printf("Write hit count :           %llu\n", write_hit_count);
    printf("Write miss count :          %llu\n\n", write_miss_count);

    printf("Miss rate :                 %f\n", (1.0 * miss_count) / (miss_count + hit_count));
    printf("Read miss rate :            %f\n", (1.0 * read_miss_count) / (read_miss_count + read_hit_count));
    printf("Write miss rate :           %f\n\n", (1.0 * write_miss_count) / (write_miss_count + write_hit_count));
}