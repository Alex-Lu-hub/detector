#ifndef DETECTOR_TF_FRAME_H_
#define DETECTOR_TF_FRAME_H_
#include <string>

#include <tensorflow/c/c_api.h>

class TFModel {
 public:
  TFModel() {}
  ~TFModel();

  bool Load(std::string model_path, uint8_t *config, int config_len,
            bool use_session_config, bool show_state);
  TF_Tensor *Predict(const TF_Output *input_op, TF_Tensor *const *input_tensor,
                     std::string output_op_name, int output_op_idx);

  TF_Graph *GetGraph() { return graph_; }
  TF_Session *GetSession() { return sess_; }

 private:
  TF_Graph *graph_;
  TF_Session *sess_;
  bool show_state_;
};

TF_Buffer *ReadBufferFromFile(const char *file);
void DeallocateBuffer(void *data, size_t);
#endif // DETECTOR_TF_FRAME_H_