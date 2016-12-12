#!/bin/bash

# Generate Predictor Test Report
# Created by: Weiqing Ni
gshare_config=$1
assoc=$2
if [[ -z $gshare_config ]]; then
	#statements
	gshare_config=10
fi
if [[ -z $assoc ]]; then
	#statements
	assoc=1
fi
hash_config=$[2**$gshare_config]
# GCC
./sim-outorder -bpred hash -bpred:hash $hash_config benchmarks/cc1.alpha -O benchmarks/1stmt.i > test_report/hash_$hash_config\_gcc.txt 2>&1
./sim-outorder -bpred gshare -bpred:gshare $gshare_config benchmarks/cc1.alpha -O benchmarks/1stmt.i > test_report/gshare_$hash_config\_gcc.txt 2>&1
./sim-outorder -bpred hodge -bpred:hodge $gshare_config $assoc benchmarks/cc1.alpha -O benchmarks/1stmt.i > test_report/hodge_$hash_config\_$assoc\_gcc.txt 2>&1
str_hash=$(ps -ef|grep bpred_hash.bpred_dir_rate test_report/hash_$hash_config\_gcc.txt)
str_gshare=$(ps -ef|grep bpred_gshare.bpred_dir_rate test_report/gshare_$hash_config\_gcc.txt)
str_hodge=$(ps -ef|grep bpred_hodge.bpred_dir_rate test_report/hodge_$hash_config\_$assoc\_gcc.txt)
echo -e "GCC	\c" >> test_report/final/hash_$hash_config.txt
echo $str_hash   >> test_report/final/hash_$hash_config.txt
echo -e "GCC	\c" >> test_report/final/gshare_$hash_config.txt
echo $str_gshare >> test_report/final/gshare_$hash_config.txt
echo -e "GCC	\c" >> test_report/final/hodge_$hash_config\_$assoc.txt
echo $str_hodge  >> test_report/final/hodge_$hash_config\_$assoc.txt


# Compress95
#./sim-outorder -bpred hash -bpred:hash $hash_config benchmarks/compress95.alpha benchmarks/compress95.i
./sim-outorder -bpred hash -bpred:hash $hash_config benchmarks/compress95.alpha benchmarks/compress95.i > test_report/hash_$hash_config\_compress95.txt 2>&1
./sim-outorder -bpred gshare -bpred:gshare $gshare_config benchmarks/compress95.alpha benchmarks/compress95.i > test_report/gshare_$hash_config\_compress95.txt 2>&1
./sim-outorder -bpred hodge -bpred:hodge $gshare_config $assoc benchmarks/compress95.alpha benchmarks/compress95.i > test_report/hodge_$hash_config\_$assoc\_compress95.txt 2>&1
str_hash=$(ps -ef|grep bpred_hash.bpred_dir_rate test_report/hash_$hash_config\_compress95.txt)
str_gshare=$(ps -ef|grep bpred_gshare.bpred_dir_rate test_report/gshare_$hash_config\_compress95.txt)
str_hodge=$(ps -ef|grep bpred_hodge.bpred_dir_rate test_report/hodge_$hash_config\_$assoc\_compress95.txt)
echo -e "CMP	\c" >> test_report/final/hash_$hash_config.txt
echo $str_hash   >> test_report/final/hash_$hash_config.txt
echo -e "CMP	\c" >> test_report/final/gshare_$hash_config.txt
echo $str_gshare >> test_report/final/gshare_$hash_config.txt
echo -e "CMP	\c" >> test_report/final/hodge_$hash_config\_$assoc.txt
echo $str_hodge  >> test_report/final/hodge_$hash_config\_$assoc.txt

# Anagram
#./sim-outorder -bpred hash -bpred:hash $hash_config benchmarks/anagram.alpha words benchmarks/anagram.in
./sim-outorder -bpred hash -bpred:hash $hash_config benchmarks/anagram.alpha words benchmarks/anagram.in > test_report/hash_$hash_config\_anagram.txt 2>&1
./sim-outorder -bpred gshare -bpred:gshare $gshare_config benchmarks/anagram.alpha words benchmarks/anagram.in > test_report/gshare_$hash_config\_anagram.txt 2>&1
./sim-outorder -bpred hodge -bpred:hodge $gshare_config $assoc benchmarks/anagram.alpha words benchmarks/anagram.in > test_report/hodge_$hash_config\_$assoc\_anagram.txt 2>&1
str_hash=$(ps -ef|grep bpred_hash.bpred_dir_rate test_report/hash_$hash_config\_anagram.txt)
str_gshare=$(ps -ef|grep bpred_gshare.bpred_dir_rate test_report/gshare_$hash_config\_anagram.txt)
str_hodge=$(ps -ef|grep bpred_hodge.bpred_dir_rate test_report/hodge_$hash_config\_$assoc\_anagram.txt)
echo -e "ANA	\c" >> test_report/final/hash_$hash_config.txt
echo $str_hash   >> test_report/final/hash_$hash_config.txt
echo -e "ANA	\c" >> test_report/final/gshare_$hash_config.txt
echo $str_gshare >> test_report/final/gshare_$hash_config.txt
echo -e "ANA	\c" >> test_report/final/hodge_$hash_config\_$assoc.txt
echo $str_hodge  >> test_report/final/hodge_$hash_config\_$assoc.txt

# Go
#./sim-outorder -bpred hash -bpred:hash $hash_config benchmarks/go.alpha 50 9  benchmarks/2stone9.in
./sim-outorder -bpred hash -bpred:hash $hash_config benchmarks/go.alpha 50 9  benchmarks/2stone9.in > test_report/hash_$hash_config\_go.txt 2>&1
./sim-outorder -bpred gshare -bpred:gshare $gshare_config benchmarks/go.alpha 50 9  benchmarks/2stone9.in > test_report/gshare_$hash_config\_go.txt 2>&1
./sim-outorder -bpred hodge -bpred:hodge $gshare_config $assoc benchmarks/go.alpha 50 9  benchmarks/2stone9.in > test_report/hodge_$hash_config\_$assoc\_go.txt 2>&1
str_hash=$(ps -ef|grep bpred_hash.bpred_dir_rate test_report/hash_$hash_config\_go.txt)
str_gshare=$(ps -ef|grep bpred_gshare.bpred_dir_rate test_report/gshare_$hash_config\_go.txt)
str_hodge=$(ps -ef|grep bpred_hodge.bpred_dir_rate test_report/hodge_$hash_config\_$assoc\_go.txt)
echo -e "GO 	\c" >> test_report/final/hash_$hash_config.txt
echo $str_hash   >> test_report/final/hash_$hash_config.txt
echo -e "GO 	\c" >> test_report/final/gshare_$hash_config.txt
echo $str_gshare >> test_report/final/gshare_$hash_config.txt
echo -e "GO 	\c" >> test_report/final/hodge_$hash_config\_$assoc.txt
echo $str_hodge  >> test_report/final/hodge_$hash_config\_$assoc.txt

# Test_math
./sim-outorder -bpred hash -bpred:hash $hash_config tests-alpha/bin/test-math > test_report/hash_$hash_config\_math.txt 2>&1
./sim-outorder -bpred gshare -bpred:gshare $gshare_config tests-alpha/bin/test-math > test_report/gshare_$hash_config\_math.txt 2>&1
./sim-outorder -bpred hodge -bpred:hodge $gshare_config $assoc tests-alpha/bin/test-math > test_report/hodge_$hash_config\_$assoc\_math.txt 2>&1
str_hash=$(ps -ef|grep bpred_hash.bpred_dir_rate test_report/hash_$hash_config\_math.txt)
str_gshare=$(ps -ef|grep bpred_gshare.bpred_dir_rate test_report/gshare_$hash_config\_math.txt)
str_hodge=$(ps -ef|grep bpred_hodge.bpred_dir_rate test_report/hodge_$hash_config\_$assoc\_math.txt)
echo -e "MAT	\c" >> test_report/final/hash_$hash_config.txt
echo $str_hash   >> test_report/final/hash_$hash_config.txt
echo -e "MAT	\c" >> test_report/final/gshare_$hash_config.txt
echo $str_gshare >> test_report/final/gshare_$hash_config.txt
echo -e "MAT	\c" >> test_report/final/hodge_$hash_config\_$assoc.txt
echo $str_hodge  >> test_report/final/hodge_$hash_config\_$assoc.txt

# Test_dirent
./sim-outorder -bpred hash -bpred:hash $hash_config tests-alpha/bin/test-dirent > test_report/hash_$hash_config\_dirent.txt 2>&1
./sim-outorder -bpred gshare -bpred:gshare $gshare_config tests-alpha/bin/test-dirent > test_report/gshare_$hash_config\_dirent.txt 2>&1
./sim-outorder -bpred hodge -bpred:hodge $gshare_config $assoc tests-alpha/bin/test-dirent > test_report/hodge_$hash_config\_$assoc\_dirent.txt 2>&1
str_hash=$(ps -ef|grep bpred_hash.bpred_dir_rate test_report/hash_$hash_config\_dirent.txt)
str_gshare=$(ps -ef|grep bpred_gshare.bpred_dir_rate test_report/gshare_$hash_config\_dirent.txt)
str_hodge=$(ps -ef|grep bpred_hodge.bpred_dir_rate test_report/hodge_$hash_config\_$assoc\_dirent.txt)
echo -e "DIR	\c" >> test_report/final/hash_$hash_config.txt
echo $str_hash   >> test_report/final/hash_$hash_config.txt
echo -e "DIR	\c" >> test_report/final/gshare_$hash_config.txt
echo $str_gshare >> test_report/final/gshare_$hash_config.txt
echo -e "DIR	\c" >> test_report/final/hodge_$hash_config\_$assoc.txt
echo $str_hodge  >> test_report/final/hodge_$hash_config\_$assoc.txt

# Test_llong
./sim-outorder -bpred hash -bpred:hash $hash_config tests-alpha/bin/test-llong > test_report/hash_$hash_config\_llong.txt 2>&1
./sim-outorder -bpred gshare -bpred:gshare $gshare_config tests-alpha/bin/test-llong > test_report/gshare_$hash_config\_llong.txt 2>&1
./sim-outorder -bpred hodge -bpred:hodge $gshare_config $assoc tests-alpha/bin/test-llong > test_report/hodge_$hash_config\_$assoc\_llong.txt 2>&1
str_hash=$(ps -ef|grep bpred_hash.bpred_dir_rate test_report/hash_$hash_config\_llong.txt)
str_gshare=$(ps -ef|grep bpred_gshare.bpred_dir_rate test_report/gshare_$hash_config\_llong.txt)
str_hodge=$(ps -ef|grep bpred_hodge.bpred_dir_rate test_report/hodge_$hash_config\_$assoc\_llong.txt)
echo -e "LLG	\c" >> test_report/final/hash_$hash_config.txt
echo $str_hash   >> test_report/final/hash_$hash_config.txt
echo -e "LLG	\c" >> test_report/final/gshare_$hash_config.txt
echo $str_gshare >> test_report/final/gshare_$hash_config.txt
echo -e "LLG	\c" >> test_report/final/hodge_$hash_config\_$assoc.txt
echo $str_hodge  >> test_report/final/hodge_$hash_config\_$assoc.txt

# Test_lswlr
./sim-outorder -bpred hash -bpred:hash $hash_config tests-alpha/bin/test-lswlr > test_report/hash_$hash_config\_lswlr.txt 2>&1
./sim-outorder -bpred gshare -bpred:gshare $gshare_config tests-alpha/bin/test-lswlr > test_report/gshare_$hash_config\_lswlr.txt 2>&1
./sim-outorder -bpred hodge -bpred:hodge $gshare_config $assoc tests-alpha/bin/test-lswlr > test_report/hodge_$hash_config\_$assoc\_lswlr.txt 2>&1
str_hash=$(ps -ef|grep bpred_hash.bpred_dir_rate test_report/hash_$hash_config\_lswlr.txt)
str_gshare=$(ps -ef|grep bpred_gshare.bpred_dir_rate test_report/gshare_$hash_config\_lswlr.txt)
str_hodge=$(ps -ef|grep bpred_hodge.bpred_dir_rate test_report/hodge_$hash_config\_$assoc\_lswlr.txt)
echo -e "LSW	\c" >> test_report/final/hash_$hash_config.txt
echo $str_hash   >> test_report/final/hash_$hash_config.txt
echo -e "LSW	\c" >> test_report/final/gshare_$hash_config.txt
echo $str_gshare >> test_report/final/gshare_$hash_config.txt
echo -e "LSW	\c" >> test_report/final/hodge_$hash_config\_$assoc.txt
echo $str_hodge  >> test_report/final/hodge_$hash_config\_$assoc.txt

# Format
for file in ./test_report/final/*; do
	target=${file##*/}
	sed -e 's/\# branch direction-prediction rate (i.e., all-hits\/updates)/ /g' -e 's/bpred_hash.bpred_dir_rate/ /g' -e 's/bpred_gshare.bpred_dir_rate/ /g' -e 's/bpred_hodge.bpred_dir_rate/ /g' test_report/final/$target > test_report/table/$target
done

# sed -e 's/\# branch direction-prediction rate (i.e., all-hits\/updates)/ /g' -e 's/bpred_hash.bpred_dir_rate/ /g' -e 's/bpred_hodge.bpred_dir_rate/ /g' -e 's/bpred_hodge.bpred_dir_rate/ /g' test_report/final/hash_$hash_config.txt > test_report/table/hash_$hash_config.txt