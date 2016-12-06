#!/bin/bash
for file in ./tests/bin/*; do
  target=${file##*/}
  echo $target
  ./r.sh hash 1024 $target >./test_report/hash_10_$target 2>&1
  ./r.sh gshare 10 $target >./test_report/gshare_10_$target 2>&1
  ./r.sh hodge 10 2 $target >./test_report/hodge_10_1_$target 2>&1
done
