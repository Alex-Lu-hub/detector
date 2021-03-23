import tensorflow as tf
gpu_options = tf.compat.v1.GPUOptions(per_process_gpu_memory_fraction = 0.1, visible_device_list = "1")
config = tf.compat.v1.ConfigProto(
    # device_num = 1,
    device_count = {"CPU":1, "GPU":0}, 
    gpu_options = gpu_options,
    # log_device_placement = True, 
    inter_op_parallelism_threads = 1,
    intra_op_parallelism_threads = 1,
    use_per_session_threads = False,
    operation_timeout_in_ms = 0)
ser = config.SerializeToString()
print()
for info in list(map(int,ser)):
    print(info)