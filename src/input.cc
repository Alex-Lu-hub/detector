#include "input.h"

#include <vector>

void Input::Init(int predict_flow_num, int packet_num_pre_flow,
                 int packet_length) {
  input_data_ = (int64_t *)malloc(predict_flow_num * packet_num_pre_flow *
                                  packet_length * sizeof(int64_t));

  cur_flow_len_ = 0;
  max_flow_len_ = predict_flow_num;
  packet_num_per_flow_ = packet_num_pre_flow;
  packet_len_ = packet_length;
}

bool Input::IsFull() {
  if (cur_flow_len_ < max_flow_len_) {
    return false;
  } else {
    cur_flow_len_ = 0;
    return true;
  }
}

void Input::Submit(void *packets, int packet_num) {
  int cur_offset = cur_flow_len_ * packet_num_per_flow_ * packet_len_;
  int data_size = packet_num * packet_len_;
  memcpy(input_data_ + cur_offset, packets, data_size * sizeof(int64_t));

  if (packet_num < packet_num_per_flow_) {
    cur_offset += data_size;
    data_size = (packet_num_per_flow_ - packet_num) * packet_len_;
    memset(input_data_ + cur_offset, 0, data_size * sizeof(int64_t));
  }

  ++cur_flow_len_;
}

void Input::Flush() {
  if (cur_flow_len_ < max_flow_len_) {
    int cur_offset = cur_flow_len_ * packet_num_per_flow_ * packet_len_;
    int data_size =
        (max_flow_len_ - cur_flow_len_) * packet_num_per_flow_ * packet_len_;
    memset(input_data_ + cur_offset, 0, data_size * sizeof(int64_t));
  }
  cur_flow_len_ = 0;
}

TF_Tensor *Input::GetInputTensor() {
  int tensor_size = max_flow_len_ * packet_num_per_flow_ * packet_len_;
  const std::vector<int64_t> input_dims = {max_flow_len_, packet_num_per_flow_,
                                           packet_len_};
  TF_Tensor *input_tensor = TF_AllocateTensor(
      TF_INT64, input_dims.data(), static_cast<int>(input_dims.size()),
      tensor_size * sizeof(int64_t));
  if (input_tensor == nullptr) {
    return nullptr;
  }

  void *tensor_data = TF_TensorData(input_tensor);
  if (tensor_data == nullptr) {
    TF_DeleteTensor(input_tensor);
    return nullptr;
  }

  memcpy(tensor_data, input_data_, tensor_size * sizeof(int64_t));

  return input_tensor;
}