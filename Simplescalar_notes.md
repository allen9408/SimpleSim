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
+ ​