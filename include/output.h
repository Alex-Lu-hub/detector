#ifndef DETECTOR_OUTPUT_H_
#define DETECTOR_OUTPUT_H_
#include <fstream>

#include <tensorflow/c/c_api.h>

class Output {
 public:
  Output() {}
  ~Output() {}
  
  void GetOutput(TF_Tensor *output_tensor, int output_1D, int output_2D, bool show_state);

 private:
  std::ofstream outfile_;
  int labelcount_[6] = {0};
};
#endif // DETECTOR_OUTPUT_H_