#!/bin/bash
for file in ./tests/bin/*; do
  echo ${file##*/}
  echo $file
  ./r.sh hash 1024 $file >./test_report/hash_10_${file##*/}
  ./r.sh gshare 10 $file >./test_report/gshare_10_${file##*/}
  ./r.sh hodge 10 2 $file >./test_report/hodge_10_1_${file##*/}
done
