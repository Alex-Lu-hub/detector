#include "tf_frame.h"

#include <iostream>

TFModel::~TFModel() {
  if (sess_ != nullptr) {
    TF_Status *status = TF_NewStatus();
    TF_CloseSession(sess_, status);
    if (TF_GetCode(status) != TF_OK) {
      TF_CloseSession(sess_, status);
      TF_DeleteSession(sess_, status);
      if (show_state_) {
        std::cerr << TF_Message(status) << std::endl;
      }
      TF_DeleteStatus(status);
    }

    TF_DeleteSession(sess_, status);
    if (TF_GetCode(status) != TF_OK) {
      if (show_state_) {
        std::cerr << TF_Message(status) << std::endl;
      }
      TF_DeleteStatus(status);
    }

    TF_DeleteStatus(status);
  }

  if (graph_ != nullptr) {
    TF_DeleteGraph(graph_);
  }
}

bool TFModel::Load(std::string model_path, uint8_t *config, int config_len,
                   bool use_session_config, bool show_state) {
  show_state_ = show_state;
  
  TF_Buffer *buffer = ReadBufferFromFile(model_path.c_str());
  if (buffer == nullptr) {
    if (show_state_) {
      std::cerr << "Error creating the graph from the given model path "
                << model_path << " !" << std::endl;
    }
    return false;
  }

  TF_Status *status = TF_NewStatus();
  TF_ImportGraphDefOptions *opts = TF_NewImportGraphDefOptions();
  graph_ = TF_NewGraph();
  TF_GraphImportGraphDef(graph_, buffer, opts, status);
  TF_DeleteImportGraphDefOptions(opts);
  TF_DeleteBuffer(buffer);

  if (TF_GetCode(status) != TF_OK) {
    TF_DeleteGraph(graph_);
    graph_ = nullptr;
    if (show_state_) {
      std::cerr << TF_Message(status) << std::endl;
    }
    return false;
  }
  TF_DeleteStatus(status);

  status = TF_NewStatus();
  TF_SessionOptions *options = TF_NewSessionOptions();

  if (use_session_config) {
    TF_SetConfig(options, (void *)config, config_len, status);
  }

  sess_ = TF_NewSession(graph_, options, status);
  TF_DeleteSessionOptions(options);

  if (TF_GetCode(status) != TF_OK) {
    if (show_state_) {
      std::cerr << TF_Message(status) << std::endl;
    }
    TF_DeleteStatus(status);
    return false;
  }

  return true;
}

TF_Tensor *TFModel::Predict(const TF_Output *input_op,
                            TF_Tensor *const *input_tensor,
                            std::string output_op_name, int output_op_idx) {
  const TF_Output output_op = TF_Output{
      TF_GraphOperationByName(graph_, output_op_name.c_str()), output_op_idx};
  TF_Tensor *output_tensor = nullptr;
  TF_Status *status = TF_NewStatus();

  TF_SessionRun(sess_,
                nullptr,
                input_op, input_tensor, 1,
                &output_op, &output_tensor, 1,
                nullptr, 0,
                nullptr,
                status
  );

  if (show_state_) {
    std::cout << TF_Message(status) << std::endl;
  }

  if (TF_GetCode(status) != TF_OK) {
    return nullptr;
  }

  TF_DeleteStatus(status);

  return output_tensor;
}

TF_Buffer *ReadBufferFromFile(const char *file) {
  const auto f = std::fopen(file, "rb");
  if (f == nullptr) {
    return nullptr;
  }

  std::fseek(f, 0, SEEK_END);
  const auto fsize = ftell(f);
  std::fseek(f, 0, SEEK_SET);

  if (fsize < 1) {
    std::fclose(f);
    return nullptr;
  }

  const auto data = std::malloc(fsize);
  std::fread(data, fsize, 1, f);
  std::fclose(f);

  TF_Buffer *buf = TF_NewBuffer();
  buf->data = data;
  buf->length = fsize;
  buf->data_deallocator = DeallocateBuffer;

  return buf;
}

void DeallocateBuffer(void *data, size_t) { std::free(data); }