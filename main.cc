#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include <iostream>
#include <thread>
#include <vector>

#include <tensorflow/c/c_api.h>

#include <detector.h>

#define kThreadCount 2

void ThreadFunc(int index) {
  struct Detector *detector = DetectorInit("/home/lw/detector/config.yaml");

  __int64_t test[6][100];
  int i = 0, j = 0, flag = 0;
  for (i = 0; i < 6; ++i) {
    for (j = 0; j < 100; ++j) {
      test[i][j] = rand() % 256;
    }
  }

  for (i = 0; i < 100000; ++i) {
    DetectorSubmit(detector, test, 6);
  }

  DetectorFlush(detector);

  DetectorFinish(detector);
}

int main() {
  std::thread t[kThreadCount];

  for (int n = 0; n < kThreadCount; ++n) {
    t[n] = std::thread(ThreadFunc, n);
  }
  for (int n = 0; n < kThreadCount; ++n) {
    t[n].join();
  }
  return 0;
}