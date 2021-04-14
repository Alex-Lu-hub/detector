#ifndef DETECTOR_INPUT_H_
#define DETECTOR_INPUT_H_
#include <tensorflow/c/c_api.h>

class Input {
public:
  Input() {}
  ~Input() { free(input_data_); }

  void Init(int predict_flow_num, int packet_num_pre_flow, int packet_length);
  bool IsFull();
  void Submit(void *packets, int packet_num);
  void Flush();

  TF_Tensor *GetInputTensor();

private:
  int64_t *input_data_;
  int cur_flow_len_;
  int max_flow_len_;
  int packet_num_per_flow_;
  int packet_len_;
};
#endif // DETECTOR_INPUT_H_