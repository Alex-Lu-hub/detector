#ifndef POORDETECTOR_MD_CONFIG_H
#define POORDETECTOR_MD_CONFIG_H
#include <vector>
#include <string>
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <stdio.h>
#include <string.h>

class md_config {
    public:
        std::string model_path;
        std::string frame;
        std::string source;
        int predict_flow_num;
        int packet_num_pre_flow;
        int packet_length;
        int output_1D;
        int output_2D;
        std::string customsize_input;
        std::string customsize_output;
        std::string input_op_name;
        int input_op_idx;
        std::string output_op_name;
        int output_op_idx;
        uint8_t *config;
        int config_len;
        bool use_session_config;
        bool show_state;

        ~md_config();
        md_config(std::string config_path);
    
    private:
        std::vector<std::string> split(std::string str, std::string separator);
        void get_session_config(std::string py_commad);
};
#endif