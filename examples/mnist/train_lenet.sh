#!/usr/bin/env sh

# change the following numbers
# batch should also change lenet_train_test.prototxt
# gpu should change the two simutanously
# L should also change in lenet_solver.prototxt

batch_size="64"
gpuids="0,1"
ngpu="2"
L=$1

SYNC="200"

outnamebase="yang/batch"$batch_size"_L"$L"_GPU"$ngpu"_SYNC"$SYNC
echo $outnamebase

./build/tools/caffe train --sync_cost $SYNC \
    --solver=examples/mnist/lenet_solver_$L.prototxt --gpu $gpuids 2>&1 | tee $outnamebase".log"

cat $outnamebase".log" | grep "Test .* loss" > $outnamebase".testloss"
