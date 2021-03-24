#include "output.h"

using namespace std;

/*
@brief  输出信息类构造函数，打开文件准备写入
@param  state：显示信息标志
@retval None
*/
output::output(bool state) {
    show_state = state;
    outfile.open("result.txt");
}

/*
@brief  输出信息类析构函数，关闭文件
@param  None
@retval None
*/
output::~output() {
    outfile.close();
}

/*
@brief  输出预测信息
@param  output_tensor：tensorflow预测后输出的tensor
@param  output_1D：输出的矩阵第一个维度大小
@param  output_2D：输出的矩阵第二个维度大小
@retval None
*/
void output::get_output(TF_Tensor* output_tensor, int output_1D, int output_2D) {
    auto data = static_cast<float*>(TF_TensorData(output_tensor));
    int label;
    float prob;
    for(int i = 0; i < output_1D; ++i) {
        prob = 0;
        label = 0;
        for(int j = 0; j < output_2D; ++j) {
            if(data[i * output_2D + j] > prob) {
                label = j;
                prob = data[i * output_2D + j];
            }
        }
        ++labelcount[label];
    }
    if(show_state) {
        cout << "result:" << endl <<
        "|-->Normal:" << labelcount[0] << endl <<
        "|-->Dewdrop:" << labelcount[1] << endl <<
        "|-->Nopen:" << labelcount[2] << endl <<
        "|-->Scaner:" << labelcount[3] << endl <<
        "|-->SSH:" << labelcount[4] << endl <<
        "|-->Telnet:" << labelcount[5] << endl;
    }
    outfile << "result:" << endl <<
        "|-->Normal:" << labelcount[0] << endl <<
        "|-->Dewdrop:" << labelcount[1] << endl <<
        "|-->Nopen:" << labelcount[2] << endl <<
        "|-->Scaner:" << labelcount[3] << endl <<
        "|-->SSH:" << labelcount[4] << endl <<
        "|-->Telnet:" << labelcount[5] << endl;
}