#ifndef DETECTOR_OUTPUT_H
#define DETECTOR_OUTPUT_H
#include <tensorflow/c/c_api.h>
#include <iostream>
#include <fstream>

class output {
    public:
        output(bool state);
        ~output();
        void get_output(TF_Tensor* output_tensor, int output_1D, int output_2D);

    private:
        std::ofstream outfile;
        bool show_state;
        int labelcount[6] = {0};
};
#endif