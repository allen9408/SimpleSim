#!/bin/bash

# For branch prediction
pred_type=$1
if [[ -z $pred_type ]]; then
	# default predictor: hash
	pred_type='hash'
fi
echo PREDICTORTYPE:$pred_type

if [[ $pred_type == 'hash' ]]; then
	# hash predictor
	hash_size=$2
	if [[ -z $hash_size ]]; then
		# default size 512
		hash_size=512
	fi
	./sim-outorder -bpred hash -bpred:hash $hash_size ./tests/bin/test-math
elif [[ $pred_type == 'gshare' ]]; then
	# gshare predictor
	# 1 parameter: history FIFO size
	fifo_size=$2
	if [[ -z $fifo_size ]]; then
		# default size 512
		fifo_size=10
	fi
	./sim-outorder -bpred gshare -bpred:gshare $fifo_size ./tests/bin/test-math
elif [[ $pred_type == '2lev' ]]; then
	# 2 level predictor
	# 4 parameters: l1size l2size hist_size xor
	./sim-outorder -bpred 2lev -bpred:2lev $2 $3 $4 $5 ./tests/bin/test-math
fi
