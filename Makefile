main:main.o cache.o
	g++ -o main cache.o main.o
main.o:main.cpp
	g++ -c main.cpp
cache.o:cache.cpp
	g++ -c cache.cpp
.PHONY:clean lab1 lab2 lab3
lab1:
	./main --trace_file trace/astar.trace --cacheline_size 8 --way_num 1 --replace_policy LRU --write_allocation --result_file astar_result
	./main --trace_file trace/astar.trace --write_allocation --way_num 16384 --cacheline_size 8 --replace_policy LRU --result_file astar_result
	./main --trace_file trace/astar.trace --write_allocation --way_num 4 --cacheline_size 8 --replace_policy LRU --result_file astar_result
	./main --trace_file trace/astar.trace --write_allocation --way_num 8 --cacheline_size 8 --replace_policy LRU --result_file astar_result
	./main --trace_file trace/astar.trace --write_allocation --way_num 1 --cacheline_size 32 --replace_policy LRU --result_file astar_result
	./main --trace_file trace/astar.trace --write_allocation --way_num 4096 --cacheline_size 32 --replace_policy LRU --result_file astar_result
	./main --trace_file trace/astar.trace --write_allocation --way_num 4 --cacheline_size 32 --replace_policy LRU --result_file astar_result
	./main --trace_file trace/astar.trace --write_allocation --way_num 8 --cacheline_size 32 --replace_policy LRU --result_file astar_result
	./main --trace_file trace/astar.trace --write_allocation --way_num 1 --cacheline_size 64 --replace_policy LRU --result_file astar_result
	./main --trace_file trace/astar.trace --write_allocation --way_num 2048 --cacheline_size 64 --replace_policy LRU --result_file astar_result
	./main --trace_file trace/astar.trace --write_allocation --way_num 4 --cacheline_size 64 --replace_policy LRU --result_file astar_result
	./main --trace_file trace/astar.trace --write_allocation --way_num 8 --cacheline_size 64 --replace_policy LRU --result_file astar_result
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
	./main --trace_file trace/mcf.trace --cacheline_size 8 --way_num 1 --replace_policy LRU --write_allocation --result_file mcf_result
	./main --trace_file trace/mcf.trace --write_allocation --way_num 16384 --cacheline_size 8 --replace_policy LRU --result_file mcf_result
	./main --trace_file trace/mcf.trace --write_allocation --way_num 4 --cacheline_size 8 --replace_policy LRU --result_file mcf_result
	./main --trace_file trace/mcf.trace --write_allocation --way_num 8 --cacheline_size 8 --replace_policy LRU --result_file mcf_result
	./main --trace_file trace/mcf.trace --write_allocation --way_num 1 --cacheline_size 32 --replace_policy LRU --result_file mcf_result
	./main --trace_file trace/mcf.trace --write_allocation --way_num 4096 --cacheline_size 32 --replace_policy LRU --result_file mcf_result
	./main --trace_file trace/mcf.trace --write_allocation --way_num 4 --cacheline_size 32 --replace_policy LRU --result_file mcf_result
	./main --trace_file trace/mcf.trace --write_allocation --way_num 8 --cacheline_size 32 --replace_policy LRU --result_file mcf_result
	./main --trace_file trace/mcf.trace --write_allocation --way_num 1 --cacheline_size 64 --replace_policy LRU --result_file mcf_result
	./main --trace_file trace/mcf.trace --write_allocation --way_num 2048 --cacheline_size 64 --replace_policy LRU --result_file mcf_result
	./main --trace_file trace/mcf.trace --write_allocation --way_num 4 --cacheline_size 64 --replace_policy LRU --result_file mcf_result
	./main --trace_file trace/mcf.trace --write_allocation --way_num 8 --cacheline_size 64 --replace_policy LRU --result_file mcf_result
	./main --trace_file trace/perlbench.trace --cacheline_size 8 --way_num 1 --replace_policy LRU --write_allocation --result_file perlbench_result
	./main --trace_file trace/perlbench.trace --write_allocation --way_num 16384 --cacheline_size 8 --replace_policy LRU --result_file perlbench_result
	./main --trace_file trace/perlbench.trace --write_allocation --way_num 4 --cacheline_size 8 --replace_policy LRU --result_file perlbench_result
	./main --trace_file trace/perlbench.trace --write_allocation --way_num 8 --cacheline_size 8 --replace_policy LRU --result_file perlbench_result
	./main --trace_file trace/perlbench.trace --write_allocation --way_num 1 --cacheline_size 32 --replace_policy LRU --result_file perlbench_result
	./main --trace_file trace/perlbench.trace --write_allocation --way_num 4096 --cacheline_size 32 --replace_policy LRU --result_file perlbench_result
	./main --trace_file trace/perlbench.trace --write_allocation --way_num 4 --cacheline_size 32 --replace_policy LRU --result_file perlbench_result
	./main --trace_file trace/perlbench.trace --write_allocation --way_num 8 --cacheline_size 32 --replace_policy LRU --result_file perlbench_result
	./main --trace_file trace/perlbench.trace --write_allocation --way_num 1 --cacheline_size 64 --replace_policy LRU --result_file perlbench_result
	./main --trace_file trace/perlbench.trace --write_allocation --way_num 2048 --cacheline_size 64 --replace_policy LRU --result_file perlbench_result
	./main --trace_file trace/perlbench.trace --write_allocation --way_num 4 --cacheline_size 64 --replace_policy LRU --result_file perlbench_result
	./main --trace_file trace/perlbench.trace --write_allocation --way_num 8 --cacheline_size 64 --replace_policy LRU --result_file perlbench_result
lab2:
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
lab3:
	./main --trace_file trace/astar.trace --way_num 8 --cacheline_size 8 --replace_policy LRU --result_file astar_result
	./main --trace_file trace/astar.trace --write_allocation --write_through --way_num 8 --cacheline_size 8 --replace_policy Random --result_file astar_result
	./main --trace_file trace/astar.trace --write_through --way_num 8 --cacheline_size 8 --replace_policy BT --result_file astar_result
	./main --trace_file trace/astar.trace --write_allocation --write_through --way_num 8 --cacheline_size 8 --replace_policy BT --result_file astar_result
	./main --trace_file trace/mcf.trace --way_num 8 --cacheline_size 8 --replace_policy LRU --result_file mcf_result
	./main --trace_file trace/mcf.trace --write_allocation --write_through --way_num 8 --cacheline_size 8 --replace_policy Random --result_file mcf_result
	./main --trace_file trace/mcf.trace --write_through --way_num 8 --cacheline_size 8 --replace_policy BT --result_file mcf_result
	./main --trace_file trace/mcf.trace --write_allocation --write_through --way_num 8 --cacheline_size 8 --replace_policy BT --result_file mcf_result
	./main --trace_file trace/bzip2.trace --way_num 8 --cacheline_size 8 --replace_policy LRU --result_file bzip2_result
	./main --trace_file trace/bzip2.trace --write_allocation --write_through --way_num 8 --cacheline_size 8 --replace_policy Random --result_file bzip2_result
	./main --trace_file trace/bzip2.trace --write_through --way_num 8 --cacheline_size 8 --replace_policy BT --result_file bzip2_result
	./main --trace_file trace/bzip2.trace --write_allocation --write_through --way_num 8 --cacheline_size 8 --replace_policy BT --result_file bzip2_result
	./main --trace_file trace/perlbench.trace --way_num 8 --cacheline_size 8 --replace_policy LRU --result_file perlbench_result
	./main --trace_file trace/perlbench.trace --write_allocation --write_through --way_num 8 --cacheline_size 8 --replace_policy Random --result_file perlbench_result
	./main --trace_file trace/perlbench.trace --write_through --way_num 8 --cacheline_size 8 --replace_policy BT --result_file perlbench_result
	./main --trace_file trace/perlbench.trace --write_allocation --write_through --way_num 8 --cacheline_size 8 --replace_policy BT --result_file perlbench_result
clean:
	rm -f *.o
	rm main
