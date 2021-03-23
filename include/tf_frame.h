#ifndef DETECTOR_TF_FRAME_H
#define DETECTOR_TF_FRAME_H
#include <tensorflow/c/c_api.h>
#include <iostream>
#include <string>

void DeallocateBuffer(void* data, size_t);
TF_Buffer* ReadBufferFromFile(const char* file);

class tf_model {
    public:
        TF_Graph* graph;
        TF_Session* sess;

        ~tf_model();
        bool load(std::string model_path, uint8_t *config, int config_len, bool use_session_config);
        TF_Tensor* predict(const TF_Output* input_op, TF_Tensor* const* input_tensor, std::string output_op_name, int output_op_idx);

    private:
    
};
#endif