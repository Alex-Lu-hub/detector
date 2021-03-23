#include "detector.h"
#include "md_config.h"
#include "tf_frame.h"
#include "input.h"
#include "output.h"
    
#ifdef __cplusplus
extern "C" {
#endif 

struct detector_t{
    md_config* confinfo;
    tf_model* model;
    input* myinput;
    output* myoutput;
};

/*
@brief  检测结构初始化函数，用于初始化各个所需类
@param  config：配置文件的路径
@retval api：检测结构指针
*/
struct detector_t *detector_init(char *config) {
    struct detector_t * api = new struct detector_t;

    // 构建配置信息类
    std::string  config_path = config;
    md_config* confinfo = new md_config(config_path);

    // 构建模型类
    tf_model* model = new tf_model();

    // 如果模型加载失败，直接返回空指针
    if(!model->load(confinfo->model_path, confinfo->config, confinfo->config_len, confinfo->use_session_config)) {
        return nullptr;
    }

    // 构建数据输入类
    input* myinput = new input(model->graph, confinfo->input_op_name, confinfo->input_op_idx, confinfo->predict_flow_num, confinfo->packet_num_pre_flow, confinfo->packet_length);

    // 构建数据输出类
    output* myoutput = new output();
    
    api->confinfo = confinfo;
    api->model = model;
    api->myinput = myinput;
    api->myoutput = myoutput;
    return api;
}

/*
@brief  流传入函数，用于将一个流的数据传入tensor
@param  api：检测结构指针
@param  packets：传入数据指针
@param  packet_num：传入流包的个数
@retval None
*/
void detector_submit(struct detector_t *api, void *packets, int packet_num) {
    api->myinput->submit(packets, packet_num);
    // 如果已经需要预测，则进行预测
    if(api->myinput->isfull()) {
        const TF_Output input_op = TF_Output{TF_GraphOperationByName(api->model->graph, api->confinfo->input_op_name.c_str()), api->confinfo->input_op_idx};
        api->myoutput->get_output(api->model->predict(&input_op, &(api->myinput->input_tensor), api->confinfo->output_op_name, api->confinfo->output_op_idx), api->confinfo->output_1D, api->confinfo->output_2D);
    }
}

/*
@brief  洪范预测函数，直接进行预测
@param  api：检测结构指针
@retval None
*/
void detector_flush(struct detector_t *api) {
    api->myinput->flush();
    const TF_Output input_op = TF_Output{TF_GraphOperationByName(api->model->graph, api->confinfo->input_op_name.c_str()), api->confinfo->input_op_idx};
    api->myoutput->get_output(api->model->predict(&input_op, &(api->myinput->input_tensor), api->confinfo->output_op_name, api->confinfo->output_op_idx), api->confinfo->output_1D, api->confinfo->output_2D);
}

/*
@brief  检测结构销毁函数，销毁检测结构指针
@param  api：检测结构指针
@retval None
*/
void detector_fini(struct detector_t *api) {
    delete api->confinfo;
    delete api->model;
    delete api->myinput;
    delete api->myoutput;
    delete api;
}

#ifdef __cplusplus
};
#endif 