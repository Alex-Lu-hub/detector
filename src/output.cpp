#include "output.h"

using namespace std;

/*
@brief  输出预测信息
@param  output_tensor：tensorflow预测后输出的tensor
@retval None
*/
void output::get_output(TF_Tensor* output_tensor) {
    auto data = static_cast<float*>(TF_TensorData(output_tensor));
    for(int i = 0; i < 6; ++i) {
        cout << data[i] << endl;
    }
}