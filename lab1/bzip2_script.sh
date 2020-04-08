./main --trace_file trace/bzip2.trace --cacheline_size 8 --way_num 1 --replace_policy LRU --write_allocation --result_file bzip2_result
./main --trace_file trace/bzip2.trace --write_allocation --way_num 16384 --cacheline_size 8 --replace_policy LRU --result_file bzip2_result
./main --trace_file trace/bzip2.trace --write_allocation --way_num 4 --cacheline_size 8 --replace_policy LRU --result_file bzip2_result
./main --trace_file trace/bzip2.trace --write_allocation --way_num 8 --cacheline_size 8 --replace_policy LRU --result_file bzip2_result

./main --trace_file trace/bzip2.trace --write_allocation --way_num 1 --cacheline_size 32 --replace_policy LRU --result_file bzip2_result
./main --trace_file trace/bzip2.trace --write_allocation --way_num 4096 --cacheline_size 32 --replace_policy LRU --result_file bzip2_result
./main --trace_file trace/bzip2.trace --write_allocation --way_num 4 --cacheline_size 32 --replace_policy LRU --result_file bzip2_result
./main --trace_file trace/bzip2.trace --write_allocation --way_num 8 --cacheline_size 32 --replace_policy LRU --result_file bzip2_result

./main --trace_file trace/bzip2.trace --write_allocation --way_num 1 --cacheline_size 64 --replace_policy LRU --result_file bzip2_result
./main --trace_file trace/bzip2.trace --write_allocation --way_num 2048 --cacheline_size 64 --replace_policy LRU --result_file bzip2_result
./main --trace_file trace/bzip2.trace --write_allocation --way_num 4 --cacheline_size 64 --replace_policy LRU --result_file bzip2_result
./main --trace_file trace/bzip2.trace --write_allocation --way_num 8 --cacheline_size 64 --replace_policy LRU --result_file bzip2_result