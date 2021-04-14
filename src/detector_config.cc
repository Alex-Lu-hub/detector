#include "detector_config.h"

#include <stdio.h>
#include <string.h>

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

DetectorConfig::~DetectorConfig() {
  if (config_ != nullptr) {
    free(config_);
  }
}

void DetectorConfig::Load(std::string config_path) {
  YAML::Node config = YAML::LoadFile(config_path);

  model_path_ = config["model_path"].as<std::string>();
  source_ = config["source"].as<std::string>();
  customsize_input_ = config["customsize_input"].as<std::string>();
  customsize_output_ = config["customsize_output"].as<std::string>();
  show_state_ = config["show_state"].as<bool>();
  tensor_queue_size_ = config["tensor_queue_size"].as<int>();
  predict_thread_num_ = config["predict_thread_num"].as<int>();

  std::string input_data_form = config["input_data_form"].as<std::string>();
  std::string output_data_form = config["output_data_form"].as<std::string>();
  std::string input_op = config["input_op"].as<std::string>();
  std::string output_op = config["output_op"].as<std::string>();
  std::string py_path = config["tf_session_config"].as<std::string>();
  std::string py_bin = config["python_config"].as<std::string>();

  if (py_path == "default") {
    use_session_config_ = false;
  } else {
    use_session_config_ = true;
    std::string py_command = py_bin + " " + py_path;
    GetSessionConfig(py_command);
  }

  frame_ = SplitString(model_path_, ".")[1];

  std::vector<std::string> inputinfo = SplitString(input_data_form, "*");
  predict_flow_num_ = stoi(inputinfo[0]);
  packet_num_pre_flow_ = stoi(inputinfo[1]);
  packet_length_ = stoi(inputinfo[2]);

  std::vector<std::string> outputinfo = SplitString(output_data_form, "*");
  output_1d_ = stoi(outputinfo[0]);
  output_2d_ = stoi(outputinfo[1]);

  std::vector<std::string> inputopinfo = SplitString(input_op, ":");
  input_op_name_ = inputopinfo[0];
  input_op_idx_ = stoi(inputopinfo[1]);

  std::vector<std::string> outputopinfo = SplitString(output_op, ":");
  output_op_name_ = outputopinfo[0];
  output_op_idx_ = stoi(outputopinfo[1]);
}

std::vector<std::string> DetectorConfig::SplitString(std::string str,
                                                     std::string separator) {
  std::vector<std::string> result;
  int curat;
  while ((curat = str.find_first_of(separator)) != str.npos) {
    // 有可能存在string str = "a,,b,c"，求split(str, ",")情况
    // 所以每次都要判断curat是否为零
    if (curat > 0) {
      result.push_back(str.substr(0, curat));
    }
    str = str.substr(curat + 1);
  }
  if (str.length() > 0) {
    result.push_back(str);
  }
  return result;
}

void DetectorConfig::GetSessionConfig(std::string py_command) {
  FILE *fp = NULL;
  char buf[1024];
  char result[4096];
  if ((fp = popen(py_command.c_str(), "r")) != NULL) {
    while (fgets(buf, 1024, fp) != NULL) {
      strcat(result, buf);
    }
    pclose(fp);
    fp = NULL;
  }
  std::string tmp = result;
  std::string swap;
  config_len_ = 0;
  int count = 0;
  for (std::string info : SplitString(tmp, "\n")) {
    if (count < 1) {
      ++count;
      continue;
    }
    swap[config_len_] = stoi(info);
    ++config_len_;
  }

  config_ = (uint8_t *)malloc(config_len_ * sizeof(uint8_t));
  memcpy(config_, swap.c_str(), config_len_ * sizeof(uint8_t));
}