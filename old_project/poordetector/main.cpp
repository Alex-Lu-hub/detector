#include <stdio.h>
#include <stdlib.h>
#include <poordetector.h>
#include <sys/types.h>

int main() {
    struct detector_t *detector = detector_init("../config.yaml");

    __int64_t test[6][100];
    int i = 0, j = 0;
    for(i = 0; i < 6; ++i) {
        for(j = 0; j < 100; ++j) {
            test[i][j] = rand() % 256;
        }
    }

    for(i = 0; i < 1000; ++i) {
        detector_submit(detector, test, 6);
    }

    detector_flush(detector);

    detector_fini(detector);

    return 0;
}