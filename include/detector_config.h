#ifndef DETECTOR_DETECTOR_CONFIG_H_
#define DETECTOR_DETECTOR_CONFIG_H_
#include <string>
#include <vector>

class DetectorConfig {
 public:
  DetectorConfig() {}
  ~DetectorConfig();

  void Load(std::string config_path);

  std::string GetModelPath() { return model_path_; }
  std::string GetModelFrame() { return frame_; }
  int GetPredictFlowNum() { return predict_flow_num_; }
  int GetPacketNumPerFlow() { return packet_num_pre_flow_; }
  int GetPacketLen() { return packet_length_; }
  int GetOutput1D() { return output_1d_; }
  int GetOutput2D() { return output_2d_; }
  std::string GetInputOpName() { return input_op_name_; }
  int GetInputOpIdx() { return input_op_idx_; }
  std::string GetOutputOpName() { return output_op_name_; }
  int GetOutputOpIdx() { return output_op_idx_; }
  uint8_t *GetConfig() { return config_; }
  int GetConfigLen() { return config_len_; }
  bool IfUseSessionConfig() { return use_session_config_; }
  bool IfShow() { return show_state_; }
  int GetTensorQueueSize() { return tensor_queue_size_; }
  int GetPredictThreadNum() { return predict_thread_num_; }

 private:
  std::vector<std::string> SplitString(std::string str, std::string separator);
  void GetSessionConfig(std::string py_commad);

  std::string model_path_;
  std::string frame_;
  std::string source_;
  int predict_flow_num_;
  int packet_num_pre_flow_;
  int packet_length_;
  int output_1d_;
  int output_2d_;
  std::string customsize_input_;
  std::string customsize_output_;
  std::string input_op_name_;
  int input_op_idx_;
  std::string output_op_name_;
  int output_op_idx_;
  uint8_t *config_;
  int config_len_;
  bool use_session_config_;
  bool show_state_;
  int tensor_queue_size_;
  int predict_thread_num_;
};
#endif // DETECTOR_DETECTOR_CONFIG_H_