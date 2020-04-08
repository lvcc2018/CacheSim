# Cache系统组织与设计

2017011441 计76 吕传承

## 一、实验概况

1. 通过 C++ 实现 Cache 替换策略模拟器，在尽可能使用软件模拟硬件的情况下保留较好的可扩展性，能够进行不同替换策略（Random、LRU、BT等）、不同组织方式（不同块大小、不同块的组织方法）、不同写策略的硬件模拟并计算相关访存次数与读写命中率计算等数据。同时在实现模拟器时更加清楚 Cache 工作流程以及不同替换策略的本质。
2. 通过实现的 Cache 替换策略模拟器进行对照试验，测试在不同情况下不同文件的缺失率对比，比较不同的替换策略、组织方式与写方式对于命中率的影响。

## 二、代码架构及实现细节

### 1. 核心架构

Cache 替换策略模拟器核心代码为 cache 类，内部封装了结构 `cache_line` 以及 `cache_line_set` ，分别代表一个 cacheline 和一组 cacheline。另外类内封装了 cache 内部进行的各类操作，核心操作如下：

```c++
void Init() 				: 模拟器初始化，根据组织架构初始化内部数据结构并选择替换策略等
  
int check_hit() 		: 根据地址并检查在当前组中是否命中 cacheline
  
int get_victim()		: 根据选择的替换策略在当前组中选择替换的 cacheline 下标
  
void cache_update()	: 根据命中结果更新 cacheline 以及 cacheline set 中的辅助数据
```

### 2. 实现细节

1. **不同替换策略的实现**：主要区别在于获取要替换或者填入的 cacheline 位置。

   1. 首先根据输入地址解析得到 Tag、Index 以及 offset 信息，根据 Index 找到对应的 cacheline set。
   2. 检查 cacheline set 中是否命中，命中则返回命中结果，同时更新辅助数据。
   3. 如果没有命中则选择当前 set 中需要填入的位置，若有空则填入空位置并更新辅助数据，如果当前 set 中没有空则根据替换策略在 set 中寻找替换的 cache 块并填入同时更新辅助数据。

2. **Cache 数据分析**：在实现过程中 cache 模拟器主要包括 cache 元数据以及辅助数据结构的开销，分析如下：

   1. **元数据分析**：对于8路组关联、支持写回策略、块大小为 8B 的128KB大小的 cache ，Tag 位数为50位，Index 位数为11位，Offset 位数为3位，同时需要 Valid 位以及 Dirty 位，因此在 cacheline 数据结构中封装**64位的 unsigned long long** 来存储 Tag 结构，并且在更新 cacheline 时根据各位数对 Tag 进行位运算进行修改。
   2. **辅助数据结构分析**：
      1. LRU 替换策略的实现使用**32位的 unsigned int** 进行存储每一个 cacheline 在当前 set 中的次序，尽管有较大的资源浪费，但提供了更高的可扩展性，方便对更多路进行模拟。
      2. 二叉树替换策略在每一路使用**8位的 char** 进行存储当前 set 的二叉树的7位数据以及1位有效位。

3. **运行方法**：本框架提供了单独进行模拟以及快速复现结果的程序运行方法。

   1. 首先使用 `make` 命令进行编译，得到可执行文件 `main` 。

   2. 可以通过命令行参数指定相关模拟参数并运行 `main` ，命令行参数如下；

      ```shell
      --cache_size 		   :  指定 cache 系统总大小，单位为KB，默认为128
      
      --cacheline_size   :  指定每一个 cache 块的大小，单位为B，默认为8
      
      --way_num				   :  组相联路数，默认为8
        									  当设置为1时为直接映射，当设置为 cache 块个数时为全相联
      
      --replace_policy   :  指定替换策略，如LRU、Random、BT等
      
      --write_through  	 :  指定是否为写通，默认为写回
      
      --write_allocation :  指定是否为写分配，默认为不写分配
      
      --trace_file			 :  指定读入的 trace 文件，默认为“trace/astar.trace”
      
      --result_file			 :  指定命中率等数据的写入文件，默认为“result”
      
      --log_file 				 :  指定 log 的写入文件，默认为“log”
      ```

      比如8路组关联、支持写回策略、块大小为 8B 的128KB大小的 cache 读入 `astar.trace` 如下：

      ```shell
      ./main --cache_size 128 --cacheline_size 8 --trace_file trace/astar.trace
      ```

   3. 可以通过 Makefile 命令快速复现以下三个实验的结果（注意运行时间较长）：

      ```makefile
      make lab1 : 固定替换策略（LRU），固定写策略（写分配+写回），尝试不同组织方式
      make lab2 : 固定Cache布局（块大小8B，8-way组关联），固定写策略（写分配+写回），尝试不同替换策略
      make lab3 : 固定Cache布局（块大小8B，8-way组关联），固定替换策略（LRU），尝试不同写策略
      ```

      输出文件将输出不同条件下对于不同文件的模拟数据以及命中与否的 log ，举例如下：

      ```c++
      --------------------------------------------------------------
      Cache Sim Config:
      Cache size: 										 	128 KB
      Cache line size: 				 					8 B
      Cache ways num: 									1 ways
      Cache line num: 									16384 cachelines
      Cache write policy: 							Write back + Write allocation
      Address Analysis:Tag (47 bits) | Index (14 bits) | offset (3 bits) 
      
      Order count: 											501468
      Read order count: 								289766
      Write order count: 								211702
      Access memory count: 							117324
      Read memory count: 								117324
      Write memory count: 							197075
      Hit count: 												384144
      Miss count: 											117324
      Read hit count: 									259050
      Read miss count: 									30716
      Write hit count: 									125094
      Write miss count: 								86608
      Miss rate: 												0.233961
      Read miss rate: 									0.106003
      Write miss rate: 									0.409103
      --------------------------------------------------------------
      ```

      可以记录 cache 情况、分析命令情况访存情况、读写缺失率等信息。（**每次运行信息将添加到文件尾**）

## 三、实验结果

### 1. 尝试在不同情况下的缺失率对比

1. **不同的组织方式**

   在固定替换策略（LRU），固定写策略（写分配+写回）的前提下，尝试不同的Cache布局获得结果如下：

   <style type="text/css">
   .tg  {border-collapse:collapse;border-spacing:0;}
   .tg td{font-family:Arial, sans-serif;font-size:14px;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:black;}
   .tg th{font-family:Arial, sans-serif;font-size:14px;font-weight:normal;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:black;}
   .tg .tg-1wig{font-weight:bold;text-align:left;vertical-align:top}
   .tg .tg-baqh{text-align:center;vertical-align:top}
   .tg .tg-wa1i{font-weight:bold;text-align:center;vertical-align:middle}
   .tg .tg-amwm{font-weight:bold;text-align:center;vertical-align:top}
   .tg .tg-nrix{text-align:center;vertical-align:middle}
   .tg .tg-0lax{text-align:left;vertical-align:top}
   </style>
   <table class="tg">
     <tr>
       <th class="tg-wa1i" rowspan="2">块大小</th>
       <th class="tg-wa1i" rowspan="2">组织方法</th>
       <th class="tg-amwm" colspan="4">文件名</th>
     </tr>
     <tr>
       <td class="tg-1wig">Astar.trace</td>
       <td class="tg-1wig">Bzip2.trace</td>
       <td class="tg-1wig">Mcf.trace</td>
       <td class="tg-1wig">Perlbench.trace</td>
     </tr>
     <tr>
       <td class="tg-nrix">8B</td>
       <td class="tg-nrix">直接映射</td>
       <td class="tg-0lax">0.233961</td>
       <td class="tg-0lax">0.0206147</td>
       <td class="tg-0lax">0.0494465</td>
       <td class="tg-0lax">0.0366683</td>
     </tr>
     <tr>
       <td class="tg-nrix">8B</td>
       <td class="tg-nrix">全关联</td>
       <td class="tg-0lax">0.186933</td>
       <td class="tg-0lax">0.00609902</td>
       <td class="tg-0lax">0.0330313</td>
       <td class="tg-0lax">0.0113353</td>
     </tr>
     <tr>
       <td class="tg-nrix">8B</td>
       <td class="tg-nrix">4路组关联</td>
       <td class="tg-0lax">0.232791</td>
       <td class="tg-0lax">0.0121705</td>
       <td class="tg-0lax">0.0457593</td>
       <td class="tg-0lax">0.0207118</td>
     </tr>
     <tr>
       <td class="tg-nrix">8B</td>
       <td class="tg-nrix">8路组关联</td>
       <td class="tg-0lax">0.232848</td>
       <td class="tg-0lax">0.0121705</td>
       <td class="tg-0lax">0.0457593</td>
       <td class="tg-0lax">0.0179016</td>
     </tr>
     <tr>
       <td class="tg-nrix">32B</td>
       <td class="tg-nrix">直接映射</td>
       <td class="tg-0lax">0.0983772</td>
       <td class="tg-0lax">0.013311</td>
       <td class="tg-0lax">0.0219677</td>
       <td class="tg-0lax">0.0231377</td>
     </tr>
     <tr>
       <td class="tg-nrix">32B</td>
       <td class="tg-nrix">全关联</td>
       <td class="tg-0lax">0.0492913</td>
       <td class="tg-0lax">0.0015445</td>
       <td class="tg-0lax">0.0108371</td>
       <td class="tg-0lax">0.00387434</td>
     </tr>
     <tr>
       <td class="tg-nrix">32B</td>
       <td class="tg-nrix">4路组关联</td>
       <td class="tg-0lax">0.0962993</td>
       <td class="tg-0lax">0.00306328</td>
       <td class="tg-0lax">0.018245</td>
       <td class="tg-0lax">0.011357</td>
     </tr>
     <tr>
       <td class="tg-nrix">32B</td>
       <td class="tg-nrix">8路组关联</td>
       <td class="tg-0lax">0.0962753</td>
       <td class="tg-0lax">0.00306328</td>
       <td class="tg-0lax">0.018245</td>
       <td class="tg-0lax">0.00822165</td>
     </tr>
     <tr>
       <td class="tg-nrix">64B</td>
       <td class="tg-nrix">直接映射</td>
       <td class="tg-0lax">0.0526813</td>
       <td class="tg-0lax">0.0158967</td>
       <td class="tg-0lax">0.0145952</td>
       <td class="tg-0lax">0.0189401</td>
     </tr>
     <tr>
       <td class="tg-nrix">64B</td>
       <td class="tg-nrix">全关联</td>
       <td class="tg-0lax">0.0255769</td>
       <td class="tg-0lax">0.000780513</td>
       <td class="tg-0lax">0.00544219</td>
       <td class="tg-0lax">0.00232933</td>
     </tr>
     <tr>
       <td class="tg-baqh">64B</td>
       <td class="tg-baqh">4路组关联</td>
       <td class="tg-0lax">0.0500989</td>
       <td class="tg-0lax">0.0015445</td>
       <td class="tg-0lax">0.0108371</td>
       <td class="tg-0lax">0.00853104</td>
     </tr>
     <tr>
       <td class="tg-baqh">64B</td>
       <td class="tg-baqh">8路组关联</td>
       <td class="tg-0lax">0.0500012</td>
       <td class="tg-0lax">0.0015445</td>
       <td class="tg-0lax">0.0108371</td>
       <td class="tg-0lax">0.00624506</td>
     </tr>
   </table>

2. **不同的替换策略**

   在固定Cache布局（块大小8B，8-way组关联），固定写策略（写分配+写回）的前提下，尝试不同的Cache替换策略获得结果如下：

   <style type="text/css">
   .tg  {border-collapse:collapse;border-spacing:0;}
   .tg td{font-family:Arial, sans-serif;font-size:14px;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:black;}
   .tg th{font-family:Arial, sans-serif;font-size:14px;font-weight:normal;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:black;}
   .tg .tg-1wig{font-weight:bold;text-align:left;vertical-align:top}
   .tg .tg-wa1i{font-weight:bold;text-align:center;vertical-align:middle}
   .tg .tg-amwm{font-weight:bold;text-align:center;vertical-align:top}
   .tg .tg-nrix{text-align:center;vertical-align:middle}
   .tg .tg-0lax{text-align:left;vertical-align:top}
   </style>
   <table class="tg">
     <tr>
       <th class="tg-wa1i" rowspan="2">替换策略</th>
       <th class="tg-amwm" colspan="4">文件名</th>
     </tr>
     <tr>
       <td class="tg-1wig">Astar.trace</td>
       <td class="tg-1wig">Bzip2.trace</td>
       <td class="tg-1wig">Mcf.trace</td>
       <td class="tg-1wig">Perlbench.trace</td>
     </tr>
     <tr>
       <td class="tg-nrix">LRU</td>
       <td class="tg-0lax">0.232848</td>
       <td class="tg-0lax">0.0121705</td>
       <td class="tg-0lax">0.0457593</td>
       <td class="tg-0lax">0.0179016</td>
     </tr>
     <tr>
       <td class="tg-nrix">随机替换</td>
       <td class="tg-0lax">0.232338</td>
       <td class="tg-0lax">0.0121705</td>
       <td class="tg-0lax">0.0459701</td>
       <td class="tg-0lax">0.0179351</td>
     </tr>
     <tr>
       <td class="tg-0lax">二叉树随机替换</td>
       <td class="tg-0lax">0.232252</td>
       <td class="tg-0lax">0.0121705</td>
       <td class="tg-0lax">0.0459445</td>
       <td class="tg-0lax">0.0178839</td>
     </tr>
   </table>

3. **不同的写策略**

   在固定Cache布局（块大小8B，8-way组关联），固定替换策略（LRU）的前提下，尝试不同的写策略获得结果如下：

   <style type="text/css">
   .tg  {border-collapse:collapse;border-spacing:0;}
   .tg td{font-family:Arial, sans-serif;font-size:14px;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:black;}
   .tg th{font-family:Arial, sans-serif;font-size:14px;font-weight:normal;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:black;}
   .tg .tg-1wig{font-weight:bold;text-align:left;vertical-align:top}
   .tg .tg-baqh{text-align:center;vertical-align:top}
   .tg .tg-wa1i{font-weight:bold;text-align:center;vertical-align:middle}
   .tg .tg-amwm{font-weight:bold;text-align:center;vertical-align:top}
   .tg .tg-nrix{text-align:center;vertical-align:middle}
   .tg .tg-0lax{text-align:left;vertical-align:top}
   </style>
   <table class="tg">
     <tr>
       <th class="tg-wa1i" rowspan="2">写策略</th>
       <th class="tg-amwm" colspan="4">文件名</th>
     </tr>
     <tr>
       <td class="tg-1wig">Astar.trace</td>
       <td class="tg-1wig">Bzip2.trace</td>
       <td class="tg-1wig">Mcf.trace</td>
       <td class="tg-1wig">Perlbench.trace</td>
     </tr>
     <tr>
       <td class="tg-nrix">写不分配+写直达</td>
       <td class="tg-0lax">0.344989</td>
       <td class="tg-0lax">0.0866993</td>
       <td class="tg-0lax">0.111469</td>
       <td class="tg-0lax">0.046634</td>
     </tr>
     <tr>
       <td class="tg-nrix">写分配+写回</td>
       <td class="tg-0lax">0.232338</td>
       <td class="tg-0lax">0.0121705</td>
       <td class="tg-0lax">0.0459701</td>
       <td class="tg-0lax">0.0179351</td>
     </tr>
     <tr>
       <td class="tg-baqh">写不分配+写回</td>
       <td class="tg-0lax">0.345621</td>
       <td class="tg-0lax">0.0866993</td>
       <td class="tg-0lax">0.111469</td>
       <td class="tg-0lax">0.0467049</td>
     </tr>
     <tr>
       <td class="tg-baqh">写分配+写直达</td>
       <td class="tg-0lax">0.232252</td>
       <td class="tg-0lax">0.0121705</td>
       <td class="tg-0lax">0.0459445</td>
       <td class="tg-0lax">0.0178839</td>
     </tr>
   </table>

### 2. 重点trace详细数据

固定 Cache 布局（块大 8B ，采用 8-way  组关联），固定LRU替换策略，固定写策略（写分配+写回）时运行四个重点 trace 文件得到详细数据结果如下（对应 log 文件记录了命中与否）：

#### Astar.trace：

```c++
--------------------------------------------------------------
Cache Sim Config:
Cache size: 											128 KB
Cache line size: 									8 B
Cache ways num: 									8 ways
Cache line num: 									16384 cachelines
Cache write policy: 							Write back + Write allocation
Address Analysis:Tag (50 bits) | Index (11 bits) | offset (3 bits) 

Order count: 											501468
Read order count: 								289766
Write order count: 								211702
Access memory count: 							116766
Read memory count: 								116766
Write memory count: 							74762
Hit count: 												384702
Miss count: 											116766
Read hit count: 									259687
Read miss count: 									30079
Write hit count: 									125015
Write miss count: 								86687
Miss rate: 												0.232848
Read miss rate: 									0.103804
Write miss rate: 									0.409477
--------------------------------------------------------------
```

#### Bzip2.trace：

```c++
--------------------------------------------------------------
Cache Sim Config:
Cache size: 											128 KB
Cache line size: 									8 B
Cache ways num: 									8 ways
Cache line num: 									16384 cachelines
Cache write policy: 							Write back + Write allocation
Address Analysis:Tag (50 bits) | Index (11 bits) | offset (3 bits) 

Order count: 											544514
Read order count: 								369344
Write order count: 								175170
Access memory count: 							6627
Read memory count: 								6627
Write memory count: 							0
Hit count: 												537887
Miss count: 											6627
Read hit count: 									365973
Read miss count: 									3371
Write hit count: 									171914
Write miss count: 								3256
Miss rate: 												0.0121705
Read miss rate: 									0.00912699
Write miss rate: 									0.0185877
--------------------------------------------------------------
```

#### Mcf.trace

```c++
--------------------------------------------------------------
Cache Sim Config:
Cache size: 											128 KB
Cache line size: 									8 B
Cache ways num: 									8 ways
Cache line num: 									16384 cachelines
Cache write policy: 							Write back + Write allocation
Address Analysis:Tag (50 bits) | Index (11 bits) | offset (3 bits) 

Order count: 											507700
Read order count: 								280182
Write order count: 								227518
Access memory count: 							23232
Read memory count: 								23232
Write memory count: 							9067
Hit count: 												484468
Miss count: 											23232
Read hit count: 									275835
Read miss count: 									4347
Write hit count: 									208633
Write miss count: 								18885
Miss rate: 												0.0457593
Read miss rate: 									0.0155149
Write miss rate: 									0.0830044
--------------------------------------------------------------
```

#### Perlbench.trace

```c++
--------------------------------------------------------------
Cache Sim Config:
Cache size: 											128 KB
Cache line size: 									8 B
Cache ways num: 									8 ways
Cache line num: 									16384 cachelines
Cache write policy: 							Write back + Write allocation
Address Analysis:Tag (50 bits) | Index (11 bits) | offset (3 bits) 

Order count: 											507441
Read order count: 								302052
Write order count: 								205389
Access memory count: 							9084
Read memory count: 								9084
Write memory count: 							753
Hit count: 												498357
Miss count: 											9084
Read hit count: 									297048
Read miss count: 									5004
Write hit count: 									201309
Write miss count: 								4080
Miss rate: 												0.0179016
Read miss rate: 									0.0165667
Write miss rate: 									0.0198647
--------------------------------------------------------------
```

## 四、实验分析

1. **程序实现分析**：实现的框架提供了较好的可扩展性，可以实现较多 cache 模拟条件，并且提供了较多接口可以实现更多 cache 替换策略。缺点在于在实现 cache 元数据时浪费了较多位数，最好的实现方式为根据 cache 条件来确定各数据的位数，动态分配并使用 bitset 直接对位进行操作可以达到最简，但由于时间原因未进行更多实现，这是一个较好的优化方向。
2. **实验结果分析**：
   1. 对于不同条件下的命中率，影响最大的因素为 trace 文件，不同的 trace 文件对应的时间局部性和空间局部性差异较大，因此得到的命中率差异主要来源为 trace 文件的不同。
   2. 使用不同的替换策略情况下，使用 LRU 和 BT 进行模拟得到的结果与使用随机替换得到的结果相差较小，分析可能是 trace 文件本身体现的局部性特征一般，并且与 8B 的块大小相比较之下 trace 文件仍然较少，因此难以区分出各种不同替换策略之间较大的差异性，但不同替换策略之间的差异性可以通过自己建立专门测试文件的到较好的区分，在此不做过多赘述。
   3. 使用不同的块大小的情况下，使用的块大小越大，缺失率较低，可见块大小是影响缺失率的较大的因素之一，这是比较明显的结论，块大小较大的情况下被替换出的概率较低，可以发现空间局部性较好的文件对于块大小的敏感性较强。
   4. 使用不同组织方式时可以发现，使用全相联得到的缺失率最低，但全相联运行时间最长，因为需要一一对 Tag 进行比对，其他三种方式中直接映射缺失率较高，而四路组相联和八路组相联的缺失率不相上下，可能的原因同样是对于小规模数据可能难以体现两种组织方式的较大差异。
   5. 使用不同的写策略时，可以看到写分配对于降低缺失率有较大影响，阅读详细数据之后发现该策略可以显著减少写缺失率从而降低全局缺失率。而写回与否则对降低缺失率影响较小，难以在当前文件中体现。阅读两者的详细数据之后观察访存次数可见写策略对于访存次数影响较大，以 astar.trace 为例，使用写不分配+写直达策略时极大减少了读内存次数，可见使用特定的写策略可以对访存次数有极大的改变。

## 五、拓展实现

此外对其他文件进行实验，固定 Cache 布局（块大 8B ，采用 8-way  组关联），固定LRU替换策略，固定写策略（写分配+写回）时运行四个重点 trace 文件得到详细数据结果如下，结果分析与以上类似，在此不再赘述。

#### Gcc.trace

```c++
--------------------------------------------------------------
Cache Sim Config:
Cache size: 											128 KB
Cache line size: 									8 B
Cache ways num: 									8 ways
Cache line num: 									16384 cachelines
Cache write policy: 							Write back + Write allocation
Address Analysis:Tag (50 bits) | Index (11 bits) | offset (3 bits) 

Order count: 											515683
Read order count: 								318197
Write order count: 								197486
Access memory count: 							21132
Read memory count: 								21132
Write memory count: 							4728
Hit count: 												494551
Miss count: 											21132
Read hit count: 									314455
Read miss count: 									3742
Write hit count: 									180096
Write miss count: 								17390
Miss rate: 												0.0409787
Read miss rate: 									0.01176
Write miss rate: 									0.0880569
--------------------------------------------------------------
```

#### Swim.trace

```c++
--------------------------------------------------------------
Cache Sim Config:
Cache size: 											128 KB
Cache line size: 									8 B
Cache ways num: 									8 ways
Cache line num: 									16384 cachelines
Cache write policy: 							Write back + Write allocation
Address Analysis:Tag (50 bits) | Index (11 bits) | offset (3 bits) 

Order count: 											303193
Read order count: 								220668
Write order count: 								82525
Access memory count: 							19819
Read memory count: 								19819
Write memory count: 							4304
Hit count: 												283374
Miss count: 											19819
Read hit count: 									219352
Read miss count: 									1316
Write hit count: 									64022
Write miss count: 								18503
Miss rate: 												0.0653676
Read miss rate: 									0.00596371
Write miss rate: 									0.224211
--------------------------------------------------------------
```

#### Twolf.trace

```c++
--------------------------------------------------------------
Cache Sim Config:
Cache size: 											128 KB
Cache line size: 									8 B
Cache ways num: 									8 ways
Cache line num: 									16384 cachelines
Cache write policy: 							Write back + Write allocation
Address Analysis:Tag (50 bits) | Index (11 bits) | offset (3 bits) 

Order count: 											482824
Read order count: 								351403
Write order count: 								131421
Access memory count: 							5505
Read memory count: 								5505
Write memory count: 0
Hit count: 												477319
Miss count: 											5505
Read hit count: 									350369
Read miss count: 									1034
Write hit count: 									126950
Write miss count: 								4471
Miss rate: 												0.0114017
Read miss rate: 									0.00294249
Write miss rate: 									0.0340204
--------------------------------------------------------------
```

## 六、实验感想

经过本次试验对 Cache 系统有了更加深入的理解，对不同替换策略的实现细节也有了更下细节的认识。因为代码能力的缘故没有对保存位数有太过严格的局限，但本身不影响程序的功能与可扩展性。本身这也是对工程能力与代码能力的一次考验与练习，在本次试验中受益匪浅。

感谢老师与各位助教的帮助和指导！