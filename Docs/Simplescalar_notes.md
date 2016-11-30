## Simple scalar

+ A simulator easy to add structure in hardware structure



### Taxomony

+ Scope 
+ Depth



### Simulator Suite

+ Sim-Fast
  + no cache
  + no instruction check
+ Sim-Safe
  + check instruction 
+ Sim-Profile
+ Sim-Cache
  + cache simulation
+ Sim-Bpred
  + <font color='red'>Branch prediction</font>
+ Sim-Profile
+ Sim-Outorder
  + most detailed simulator



>sim_main()
>
>RUU(Register Update Unit)
>
>LSQ(load/store Queue)
>
>Memory dependencies are resolved by LSQ
>
>Effective address calcelated in RUU
>
>Fetch -> Dispatch -> Scheduler(ruu_issue, lsq_refresh) -> Execute -> Writeback -> Commit 
>
>



## Install

+ big-endian or little endian

  >**endian** program in simplesim/


## Useful modules
+ regs.h 
  + regs_creste
  + regs_init
+ bpred.h
  + bpred_create()
  + bpred_lookup()
  + bpred_update()

## Create my own simulators

+ Bpred.[hc]
  + Add new case in bpred_create()
  + Add new element in bpred_t
  + Add new element in bored_class{} enum
  + Add new struct in bpred_dir_t{}
  + Add new case in bpred_dir_t()
  + **optional**: Add case in bpred_dir_config()/bpred_config()
  + Add case in bpred_reg_stats()
  + Add case in bpred_dir_lookup()
  + **Add case in bpred_lookup()**
  + **Add case in bpred_update()**


## My predictor choice

+ bimod table associative
+ bimod PC check
+ **optional:** combine static strategy
+ **optional**: multi-predictor