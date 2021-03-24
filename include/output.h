#ifndef DETECTOR_OUTPUT_H
#define DETECTOR_OUTPUT_H
#include <tensorflow/c/c_api.h>
#include <iostream>

class output {
    public:
        output(bool state);
        void get_output(TF_Tensor* output_tensor, int output_1D, int output_2D);

    private:
        bool show_state;
        int labelcount[6] = {0};
};
#endif