#include "input.h"

using namespace std;

/*
@brief  数据输入类的析构函数，删掉构建的Tensor
@param  None
@retval None
*/
input::~input() {
    TF_DeleteTensor(input_tensor);
}

/*
@brief  数据输入类的构造函数，构建输入模型的tensor
@param  graph：加载的graph
@param  input_op_name：输入op的名字
@param  input_op_idx：输入op的编号
@param  predict_flow_num：预测处理的流的个数
@param  packet_num_pre_flow：每个流的包个数
@param  packet_lengeh：每个包的长度
@retval None
*/
input::input(TF_Graph* graph,  string input_op_name, int input_op_idx, int predict_flow_num, int packet_num_pre_flow, int packet_length) {
    // 构建一个空的tensor
    const vector<int64_t> input_dims = {predict_flow_num, packet_num_pre_flow, packet_length};
    input_tensor = TF_AllocateTensor(TF_INT64, input_dims.data(), static_cast<int>(input_dims.size()), predict_flow_num * packet_num_pre_flow * packet_length * sizeof(int64_t));
    
    len = 0;
    max = predict_flow_num;
    num_per_flow = packet_num_pre_flow;
    p_len = packet_length;
}

/*
@brief  判断是否需要送去预测
@param  None
@retval bool：预测标记
*/
bool input::isfull() {
    if(len < max) {
        return false;
    }
    else {
        len = 0;
        return true;
    }
}

/*
@brief  一次向tensor送入一个流的数据
@param  packets：送入数据的指针
@param  packet_num：送入流的包个数
@retval None
*/
void input::submit(void *packets, int packet_num) {
    memcpy(TF_TensorData(input_tensor) + len * num_per_flow * p_len, packets, packet_num * p_len * sizeof(int64_t));
    
    // 包数目不满每个流设置的数目时，用0补全
    if(packet_num < num_per_flow) {
        memset(TF_TensorData(input_tensor) + (len * num_per_flow + packet_num) * p_len, 0, (num_per_flow - packet_num) * p_len * sizeof(int64_t));
    }
    ++len;
}

/*
@brief  无论情况如何，直接送去预测
@param  None
@retval None
*/
void input::flush() {
    // 流不足时，剩下的用0补全
    if(len < max) {
        memset(TF_TensorData(input_tensor) + len * num_per_flow * p_len, 0, (max - len) * num_per_flow * p_len * sizeof(int64_t));
    }
    len = 0;
}