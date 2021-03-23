#include <stdio.h>
#include <detector.h>

int main() {
    struct detector_t *detector = detector_init("../config.yaml");

    __int64_t test[6][100];
    int i = 0, j = 0;
    for(i = 0; i < 6; ++i) {
        for(j = 0; j < 100; ++j) {
            test[i][j] = j;
        }
    }

    for(i = 0; i < 1000; ++i) {
        detector_submit(detector, test, 6);
    }

    detector_fini(detector);

    return 0;
}