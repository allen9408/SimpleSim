##Simple Scalar Project
<center>Weiqing Ni</center>  
<center> Rutgers Comp Arch Project</center>  

### What We Have Done

1. hash predictor(strategy 6)  
2. G-share predictor   
3. Hodgepodge predictor (own choice)

### Installation 

1. Download source code:  
2. cd {DIR}   
3. make  

*origin code*: 
   > http://www.simplescalar.com/agreement.php3?simplesim-3v0e.tgz 

### Usage *(for branch prediction only)*

./sim-outorder -bpred {pred_type} -bpred:{bpred_type} {configs} target  

> example:  ./sim-outorder -bpred hash -bpred:hash 256 tests-alpha/bin/test-math  
> example:  ./sim-outorder -bpred gshare -bpred:gshare 12 benchmarks/anagram.alpha  
> example:  ./sim-outorder -bpred hodge -bpred:hodge 11 4 benchmarks/cc1.alpha -O benchmarks/1stmt.i  
> Predictors:
> -bpred           bimod 		 # branch predictor type   {nottaken|taken|bimod|2lev|comb|hash|gsahre|hodge}  
> Predictor configs:
> -bpred:bimod     2048			 # bimodal predictor config (\<table size\>)  
> -bpred:2lev      1 1024 8 0 	 # 2-level predictor config (\<l1size\> \<l2size\> \<hist_size\> \<xor\>)  
> -bpred:comb      1024 		 # combining predictor config (\<meta_table_size\>)  
> -bpred:ras       8		     # return address stack size (0 for no return stack)  
> -bpred:btb       512 4 		 # BTB config (\<num_sets\> \<associativity\>)  
> -bpred:hash      256  		 # hash config (hash table size)   
> -bpred:gshare    10 			 # gshare config (\<history fifo size\>)  
> -bpred:hodge	   10 2 		 # hodgepodge config(\<history fifo size\>\<associativity\>)

### Test *(for three strategies only)*  
./r.sh {type} {configs} {benchmark}  
> example: ./r.sh gshare 10 test-math
> example: ./r.sh hash   1024 test-llong
> example: ./r.sh hodge  12 2 test-anagram  
>
> type: 		hash/gshare/hodge  
> configs:
>     hash: {hash table size}  
>     gshare: {history FIFO bit-width}  
>     hodge: {history FIFO bit-width} {associativity}
> benchmark:	benchmarks in {DIR}/tests-alpha/bin/

### Hodgepodge Predictor Structure

![hodge_predicture](https://cl.ly/3Z1u1J1I063s/own%20predictor.png)


#### References:  
<font size="small">
[1] S. McFarling. Combining Branch Predictors. Technical report, Western Research Laboratory, 06 1993.  
[2] J. E. Smith. A study of branch prediction strategies. In Proceedings of the 8th Annual Symposium on Computer Architecture, ISCA ’81, pages 135–148, Los Alamitos, CA, USA, 1981. IEEE Computer Society Press.
</font>