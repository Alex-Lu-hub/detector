#include "detector.h"

#include <unistd.h>

#include <mutex>
#include <thread>

#include "concurrentqueue.h"
#include "detector_config.h"
#include "input.h"
#include "output.h"
#include "tf_frame.h"

#ifdef __cplusplus
extern "C" {
#endif

std::mutex m;
static volatile bool inited = false;
static moodycamel::ConcurrentQueue<TF_Tensor *> *tensor_queue;
static std::vector<std::thread> predict_threads;

struct Detector {
  DetectorConfig confinfo;
  Input input;
};

void Threadwork(struct Detector *api) {
  printf("\n<---A new thread!--->\n");

  TFModel model;

  std::string model_path = api->confinfo.GetModelPath();
  uint8_t *config = api->confinfo.GetConfig();
  int config_len = api->confinfo.GetConfigLen();
  bool use_session_config = api->confinfo.IfUseSessionConfig();
  bool show_state = api->confinfo.IfShow();

  model.Load(model_path, config, config_len, use_session_config, show_state);

  TF_Graph *graph = model.GetGraph();
  std::string input_op_name = api->confinfo.GetInputOpName();
  int input_op_idx = api->confinfo.GetInputOpIdx();
  std::string output_op_name = api->confinfo.GetOutputOpName();
  int output_op_idx = api->confinfo.GetOutputOpIdx();
  int output_1D = api->confinfo.GetOutput1D();
  int output_2D = api->confinfo.GetOutput2D();
  const TF_Output input_op = TF_Output{
      TF_GraphOperationByName(graph, input_op_name.c_str()), input_op_idx};

  Output output;

  while (1) {
    // printf("\n<---The queue num is: %d--->\n", tensor_queue->size_approx());
    TF_Tensor *input_tensor;
    if (tensor_queue->try_dequeue(input_tensor)) {
      output.GetOutput(model.Predict(&input_op, &input_tensor, output_op_name,
                                     output_op_idx),
                       output_1D, output_2D, show_state);
      TF_DeleteTensor(input_tensor);
    }
  }
}

struct Detector *DetectorInit(char *config_path) {
  struct Detector *api = new (struct Detector);

  // 构建配置信息类
  std::string config_path_str = config_path;
  api->confinfo.Load(config_path_str);

  // 构建数据输入类
  int predict_flow_num = api->confinfo.GetPredictFlowNum();
  int packet_num_per_flow = api->confinfo.GetPacketNumPerFlow();
  int packet_len = api->confinfo.GetPacketLen();

  api->input.Init(predict_flow_num, packet_num_per_flow, packet_len);

  int tensor_queue_size = api->confinfo.GetTensorQueueSize();
  int predict_thread_num = api->confinfo.GetPredictThreadNum();

  if (!inited) {
    m.lock();
    if (!inited) {
      printf("<---init tensor_queue and predict_thread--->\n");
      tensor_queue =
          new moodycamel::ConcurrentQueue<TF_Tensor *>(tensor_queue_size);

      predict_threads.resize(predict_thread_num);
      for (int i = 0; i < predict_thread_num; ++i) {
        predict_threads[i] = std::thread(Threadwork, api);
      }
    } else {
      printf("<---tensor_queue and predict_thread are exsisted--->\n");
    }
    inited = true;
    m.unlock();
  } else {
    printf("<---tensor_queue and predict_thread are exsisted--->\n");
  }

  return api;
}

int DetectorSubmit(struct Detector *api, void *packets, int packet_num) {
  api->input.Submit(packets, packet_num);
  // 如果已经需要预测，则进行预测
  if (api->input.IsFull()) {
    TF_Tensor *input_tensor = api->input.GetInputTensor();
    if (tensor_queue->try_enqueue(input_tensor) == true) {
      return 1;
    } else {
      return -1;
    }
  }
  return 0;
}

int DetectorFlush(struct Detector *api) {
  api->input.Flush();

  TF_Tensor *input_tensor = api->input.GetInputTensor();
  if (tensor_queue->try_enqueue(input_tensor) == true) {
    return 1;
  } else {
    return -1;
  }
}

void DetectorFinish(struct Detector *api) {
  int predict_thread_num = api->confinfo.GetPredictThreadNum();
  for (int i = 0; i < predict_thread_num; ++i) {
    predict_threads[i].join();
  }
  delete api;
}

#ifdef __cplusplus
};
#endif