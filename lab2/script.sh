./main --trace_file trace/astar.trace --write_allocation --way_num 8 --cacheline_size 8 --replace_policy LRU --result_file astar_result
./main --trace_file trace/astar.trace --write_allocation --way_num 8 --cacheline_size 8 --replace_policy Random --result_file astar_result
./main --trace_file trace/astar.trace --write_allocation --way_num 8 --cacheline_size 8 --replace_policy BT --result_file astar_result

./main --trace_file trace/bzip2.trace --write_allocation --way_num 8 --cacheline_size 8 --replace_policy LRU --result_file bzip2_result
./main --trace_file trace/bzip2.trace --write_allocation --way_num 8 --cacheline_size 8 --replace_policy Random --result_file bzip2_result
./main --trace_file trace/bzip2.trace --write_allocation --way_num 8 --cacheline_size 8 --replace_policy BT --result_file bzip2_result

./main --trace_file trace/perlbench.trace --write_allocation --way_num 8 --cacheline_size 8 --replace_policy LRU --result_file perlbench_result
./main --trace_file trace/perlbench.trace --write_allocation --way_num 8 --cacheline_size 8 --replace_policy Random --result_file perlbench_result
./main --trace_file trace/perlbench.trace --write_allocation --way_num 8 --cacheline_size 8 --replace_policy BT --result_file perlbench_result

./main --trace_file trace/mcf.trace --write_allocation --way_num 8 --cacheline_size 8 --replace_policy LRU --result_file mcf_result
./main --trace_file trace/mcf.trace --write_allocation --way_num 8 --cacheline_size 8 --replace_policy Random --result_file mcf_result
./main --trace_file trace/mcf.trace --write_allocation --way_num 8 --cacheline_size 8 --replace_policy BT --result_file mcf_result