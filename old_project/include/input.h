#ifndef DETECTOR_INPUT_H
#define DETECTOR_INPUT_H
#include <tensorflow/c/c_api.h>
#include <iostream>
#include <string>
#include <vector>

class input {
    public:
        TF_Tensor* input_tensor;
        
        ~input();
        input(TF_Graph* graph,  std::string input_op_name, int input_op_idx, int predict_flow_num, int packet_num_pre_flow, int packet_length);
        bool isfull();
        void submit(void *packets, int packet_num);
        void flush();

    private:
        int len;
        int max;
        int num_per_flow;
        int p_len;
};
#endif