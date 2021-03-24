#ifndef POORDETECTOR_INPUT_H
#define POORDETECTOR_INPUT_H
#include <tensorflow/c/c_api.h>
#include <iostream>
#include <string>
#include <vector>

class input {
    public:
        int64_t* input_data;
        
        ~input();
        input(TF_Graph* graph,  std::string input_op_name, int input_op_idx, int predict_flow_num, int packet_num_pre_flow, int packet_length);
        bool isfull();
        void submit(void *packets, int packet_num);
        void flush();
        TF_Tensor* get_input_tensor();

    private:
        int len;
        int max;
        int num_per_flow;
        int p_len;
};
#endif