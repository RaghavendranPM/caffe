// Copyright 2014 BVLC and contributors.
//
// Based on data_layer.cpp by Yangqing Jia.

#include <stdint.h>
#include <pthread.h>

#include <string>
#include <vector>

#include "caffe/layer.hpp"
#include "caffe/util/io.hpp"
#include "caffe/vision_layers.hpp"

using std::string;
using std::map;
using std::pair;

// caffe.proto > LayerParameter > WindowDataParameter
//   'source' field specifies the window_file
//   'crop_size' indicates the desired warped size

namespace caffe {

template <typename Dtype>
void SPPWindowDataLayer<Dtype>::Forward_gpu(const vector<Blob<Dtype>*>& bottom,
      vector<Blob<Dtype>*>* top) {
  // Fetch data in main thread
  SPPWindowDataLayerPrefetch<Dtype>(static_cast<void*>(this));

  // First, join the thread
  // JoinPrefetchThread();
  // Copy the data
  caffe_copy(prefetch_data_->count(), prefetch_data_->cpu_data(),
      (*top)[0]->mutable_gpu_data());
  caffe_copy(prefetch_label_->count(), prefetch_label_->cpu_data(),
      (*top)[1]->mutable_gpu_data());
  // Start a new prefetch thread
  // CreatePrefetchThread();
}

INSTANTIATE_CLASS(SPPWindowDataLayer);

}  // namespace caffe
