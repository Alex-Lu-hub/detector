#include "md_config.h"

using namespace std;

/*
@brief  配置信息类的析构函数，free掉配置信息的指针
@param  None
@retval None
*/
md_config::~md_config() {
    free(config);
}

/*
@brief  配置信息类的构造函数，在构造时同时根据配置文件内容获得所有信息
@param  config_path：配置文件的路径
@retval None
*/
md_config::md_config(string config_path) {
    YAML::Node config = YAML::LoadFile(config_path);

    model_path = config["model_path"].as<string>();
    source = config["source"].as<string>();
    input_data_form = config["input_data_form"].as<string>();
    customsize_input = config["customsize_input"].as<string>();
    customsize_output = config["customsize_output"].as<string>();
    input_op = config["input_op"].as<string>();
    output_op = config["output_op"].as<string>();
    string py_path = config["tf_session_config"].as<string>();
    string py_bin = config["python_config"].as<string>();
    
    if(py_path == "default") {
        use_session_config = false;
    }
    else {
        use_session_config = true;
        string py_command = py_bin + " " + py_path;

        get_session_config(py_command);
    }
    frame = split(model_path, ".")[1];  //获得模型后缀名确定框架

    predict_flow_num = stoi(split(input_data_form, "*")[0]);  //获得送入模型的矩阵维度1
    packet_num_pre_flow = stoi(split(input_data_form, "*")[1]);  //获得送入模型的矩阵维度2
    packet_length = stoi(split(input_data_form, "*")[2]);  //获得送入模型的矩阵维度3

    input_op_name = split(input_op, ":")[0];  //获得模型输入层op名
    input_op_idx = stoi(split(input_op, ":")[1]);  //获得模型输入层op编号

    output_op_name = split(output_op, ":")[0];  //获得模型输出层op名
    output_op_idx = stoi(split(output_op, ":")[1]);  //获得模型输出层op编号
}

/*
@brief  按照给定的字符分割输入的字符串
@param  str：需要被分割的字符串
@param  separator：作为分割的字符
@retval result：分割后的字符串列表
*/
vector<string> md_config::split(string str, string separator) {
    vector<string> result;
    int cutAt;
    while ((cutAt = str.find_first_of(separator)) != str.npos) {
        // 有可能存在string str = "a,,b,c"，求split(str, ",")情况，所以每次都要判断cutAt是否为零
        if (cutAt > 0)
        {
            result.push_back(str.substr(0, cutAt));
        }
        str = str.substr(cutAt + 1);
    }
    if (str.length() > 0)
    {
        result.push_back(str);
    }
    return result;
}

/*
@brief  按照配置文件中的python脚本修改session的设置
@param  py_command：执行ptthon脚本的指令
@retval None
*/
void md_config::get_session_config(string py_command) {
    FILE *fp = NULL;
    char buf[1024];
    char result[4096];
    uint8_t num = 0;
    if( (fp = popen(py_command.c_str(), "r")) != NULL)
    {
        while(fgets(buf, 1024, fp) != NULL)
        {
            strcat(result, buf);
        }
        pclose(fp);
        fp = NULL;
    }
    string tmp = result;
    string swap;
    config_len = 0;
    int count = 0;
    for(string info: split(tmp, "\n")) {
        if(count < 1) {
            ++count;
            continue;
        }
        swap[config_len] = stoi(info);
        ++config_len;
    }

    config = (uint8_t *)malloc(config_len * sizeof(uint8_t));
    memcpy(config, swap.c_str(), config_len * sizeof(uint8_t));
}