#ifndef DETECTOR_DETECTOR_H_
#define DETECTOR_DETECTOR_H_
#ifdef __cplusplus
extern "C" {
#endif

struct Detector;
void Threadwork(struct Detector *api);
struct Detector *DetectorInit(char *config_path);
int DetectorSubmit(struct Detector *api, void *packets, int packet_num);
int DetectorFlush(struct Detector *api);
void DetectorFinish(struct Detector *api);

#ifdef __cplusplus
}
#endif
#endif // DETECTOR_DETECTOR_H_