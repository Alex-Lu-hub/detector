#include "output.h"

#include <iostream>

void Output::GetOutput(TF_Tensor *output_tensor, int output_1D, int output_2D,
                       bool show_state) {
  auto data = static_cast<float *>(TF_TensorData(output_tensor));
  int label;
  float prob;
  for (int i = 0; i < output_1D; ++i) {
    prob = 0;
    label = 0;
    for (int j = 0; j < output_2D; ++j) {
      if (data[i * output_2D + j] > prob) {
        label = j;
        prob = data[i * output_2D + j];
      }
    }
    ++labelcount_[label];
  }
  if (show_state) {
    std::cout << "result:" << std::endl
              << "|-->Normal:" << labelcount_[0] << std::endl
              << "|-->Dewdrop:" << labelcount_[1] << std::endl
              << "|-->Nopen:" << labelcount_[2] << std::endl
              << "|-->Scaner:" << labelcount_[3] << std::endl
              << "|-->SSH:" << labelcount_[4] << std::endl
              << "|-->Telnet:" << labelcount_[5] << std::endl;
  }
  // outfile_.open("result.txt", std::ofstream::app);
  // outfile_ << "result:" << std::endl
  //          << "|-->Normal:" << labelcount_[0] << std::endl
  //          << "|-->Dewdrop:" << labelcount_[1] << std::endl
  //          << "|-->Nopen:" << labelcount_[2] << std::endl
  //          << "|-->Scaner:" << labelcount_[3] << std::endl
  //          << "|-->SSH:" << labelcount_[4] << std::endl
  //          << "|-->Telnet:" << labelcount_[5] << std::endl;
  // outfile_.close();
}