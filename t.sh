#!/bin/bash
hash_config="2^$1"|bc
gsahre_config=$1
hodge_config1=$1
hodge_config2=$2
for file in ./tests/bin/*; do
  echo ${file##*/}
  echo $file
  ./r.sh hash $hash_config $file >./test_report/hash_$1_$target 2>&1 
  ./r.sh gshare $gsahre_config $file >./test_report/gshare_$1_$target 2>&1
  ./r.sh hodge $hodge_config1 $hodge_config2 $file >./test_report/hodge_$1_$2_$target 2>&1
done
