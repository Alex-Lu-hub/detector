#ifndef DETECTOR_API_H
#define DETECTOR_API_H
#ifdef __cplusplus
extern "C" {
#endif 

struct detector_t;
struct detector_t *detector_init(char *config);
void detector_submit(struct detector_t *api, void *packets, int packet_num);
void detector_flush(struct detector_t *api);
void detector_fini(struct detector_t *api);

#ifdef __cplusplus
}
#endif 
#endif