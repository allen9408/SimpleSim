##Simple Scalar Project
<center>Weiqing Ni</center>  
<center> Rutgers Comp Arch Project</center>  



### Installation 

1. Download source code from:

   > http://www.simplescalar.com/agreement.php3?simplesim-3v0e.tgz 

2. cd {DIR} and make



### Usage *(for branch prediction only)*

./sim-outorder -bpred {pred_type} -bpred:{bpred_type} {configs} target

> *Shit-like Interface!*   
>
> example:  ./sim-outorder -bpred hash -bpred:hash 256 tests/bin/test-math   
>
> -bpred                  bimod # branch predictor type   {nottaken|taken|bimod|2lev|comb|hash}  
> -bpred:bimod     2048 # bimodal predictor config (<table size>)  
> -bpred:2lev      1 1024 8 0 # 2-level predictor config (<l1size> <l2size> <hist_size> <xor>)  
> -bpred:comb      1024 # combining predictor config (<meta_table_size>)  
> -bpred:ras                  8 # return address stack size (0 for no return stack)  
> -bpred:btb       512 4 # BTB config (<num_sets> <associativity>)  
> -bpred:hash      256 # hash config (hash table size)   

