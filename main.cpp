#include "cache.h"
#include <fstream>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

using namespace std;
int main(int argc, char **argv)
{
    bit_64 _cache_size = (1 << 17);
    bit_64 _cacheline_size = 8;
    bit_64 _way_num = 8;
    replace_policy _rp = LRU;
    bit_64 _write_through = 0;
    bit_64 _write_allocation = 0;
    string _trace_file = "trace/test.trace";
    int c;
    int optIndex = 0;
    int lopt;

    static struct option longOpts[] = {
        {"cache_size", required_argument, &lopt, 1},
        {"cacheline_size", required_argument, &lopt, 2},
        {"way_num", required_argument, &lopt, 3},
        {"replace_policy", required_argument, NULL, 'p'},
        {"write_through", no_argument, NULL, 't'},
        {"write_allocation", no_argument, NULL, 'a'},
        {"trace_file", required_argument, &lopt, 4},
        {0, 0, 0, 0}};
    while (1)
    {
        c = getopt_long(argc, argv, "p:ta", longOpts, &optIndex);
        if (c == -1)
        {
            break;
        }
        switch (c)
        {
        case 0:
        {
            switch (lopt)
            {
            case 1:
                _cache_size = atoi(optarg);
                break;
            case 2:
                _cacheline_size = atoi(optarg);
                break;
            case 3:
                _way_num = atoi(optarg);
                break;
            case 4:
                _trace_file = optarg;
                break;
            }
            break;
        }
        case 'p':
        {
            if (strncmp(optarg, "LRU", sizeof("LRU")) == 0)
                _rp = LRU;
            else if (strncmp(optarg, "Random", sizeof("Random")) == 0)
                _rp = Random;

            break;
        }

        case 't':
            _write_through = 1;
            break;
        case 'a':
            _write_allocation = 1;
            break;
        }
    }
    printf("Cache Sim Initialization Config:\n\n");
    printf("cache_size:         %llu KB\n", (_cache_size >> 10));
    printf("cacheline_size:     %llu B\n", _cacheline_size);
    printf("way_num:            %llu ways\n", _way_num);
    printf("replace_policy:     %d\n", _rp);
    printf("write_through?:     %llu\n", _write_through);
    printf("write_allocation?:  %llu\n\n", _write_allocation);
    printf("trace file:         %s\n",_trace_file.c_str());
    cache cache_sim;
    cache_sim.Init(_cache_size, _cacheline_size, _way_num, _rp, _write_through, _write_allocation);
    cache_sim.read_file(_trace_file.c_str());
    cache_sim.save_rate();
    return 0;
}