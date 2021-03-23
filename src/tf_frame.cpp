#include "tf_frame.h"

using namespace std;

/*
@brief  模型类的析构函数，删掉构建的graph和session
@param  None
@retval None
*/
tf_model::~tf_model() {
    if (sess) {
        TF_Status* status = TF_NewStatus();
        TF_CloseSession(sess, status);
        if (TF_GetCode(status) != TF_OK) {
            TF_CloseSession(sess, status);
            TF_DeleteSession(sess, status);
            cerr << TF_Message(status) << endl;
            TF_DeleteStatus(status);
        }
    
        TF_DeleteSession(sess, status);
        if (TF_GetCode(status) != TF_OK) {
            cerr << TF_Message(status) << endl;
            TF_DeleteStatus(status);
        }
    
        TF_DeleteStatus(status);
    }
 
    if (graph)
        TF_DeleteGraph(graph);
}

/*
@brief  读取模型，加载graph并构建session
@param  model_path：模型的路径
@param  config：配置信息的指针
@param  config_len：配置信息的长度
@param  use_session_config：是否使用配置信息的标志
@retval bool：是否成功读取模型
*/
bool tf_model::load(string model_path, uint8_t *config, int config_len, bool use_session_config) {
    // 读取模型
    TF_Buffer* buffer = ReadBufferFromFile(model_path.c_str());
    if (buffer == nullptr) {
        cerr << "Error creating the graph from the given model path " << model_path << " !" << endl;
        return false;
    }

    // 加载graph
    TF_Status* status = TF_NewStatus();
    TF_ImportGraphDefOptions* opts = TF_NewImportGraphDefOptions();
    graph = TF_NewGraph();
    TF_GraphImportGraphDef(graph, buffer, opts, status);
    TF_DeleteImportGraphDefOptions(opts);
    TF_DeleteBuffer(buffer);
    
    if (TF_GetCode(status) != TF_OK) {
        TF_DeleteGraph(graph);
        graph = nullptr;
        cerr << TF_Message(status) << endl;
        return false;
    }
    TF_DeleteStatus(status);

    // 构建session
    status = TF_NewStatus();
    TF_SessionOptions* options = TF_NewSessionOptions();
    
    // 模型运行时的显卡设置，根据标志选用
    if(use_session_config) {
        TF_SetConfig(options, (void*)config, config_len, status);   
    }

    sess = TF_NewSession(graph, options, status);
    TF_DeleteSessionOptions(options);

    if (TF_GetCode(status) != TF_OK) {
        cerr << TF_Message(status) << endl;
        TF_DeleteStatus(status);
        return false;
    }

    return true;
}

/*
@brief  tf模型类的预测函数，输入数据进行预测并输出预测结果
@param  input_op：模型输入的op信息
@param  input_tensor：模型输入的数据
@param  output_op_name：模型输出的op名
@param  output_op_idx：模型输出的op编号
@retval output_tensor：模型输出的预测数据
*/
TF_Tensor* tf_model::predict(const TF_Output* input_op, TF_Tensor* const* input_tensor, string output_op_name, int output_op_idx) {
    // Output Tensor Create
    const TF_Output output_op = TF_Output{TF_GraphOperationByName(graph, output_op_name.c_str()), output_op_idx};

    TF_Tensor* output_tensor = nullptr;

    TF_Status* status = TF_NewStatus();

    TF_SessionRun(sess,
                  nullptr, // Run options.
                  input_op, input_tensor, 1, // Input tensors, input tensor values, number of inputs.
                  &output_op, &output_tensor, 1, // Output tensors, output tensor values, number of outputs.
                  nullptr, 0, // Target operations, number of targets.
                  nullptr, // Run metadata.
                  status // Output status.
    );

    cout << TF_Message(status) << endl;
    
    if (TF_GetCode(status) != TF_OK) {
        return nullptr;
    }
    
    TF_DeleteStatus(status);

    return output_tensor;
}

/*
@brief  Buffer数据清除函数，配合TF_Buffer使用而构建
@param  data：Buffer数据
@retval None
*/
void DeallocateBuffer(void* data, size_t) {
    std::free(data);
}

/*
@brief  模型加载函数，需要从保存的模型文件中读取信息到TF_Buffer中，构建对应的TF_Buffer
@param  file：模型的路径(C风格字符串)
@retval buf：加载模型后的Buffer
*/
TF_Buffer* ReadBufferFromFile(const char* file) {
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

    TF_Buffer* buf = TF_NewBuffer();
    buf->data = data;
    buf->length = fsize;
    buf->data_deallocator = DeallocateBuffer;

    return buf;
}