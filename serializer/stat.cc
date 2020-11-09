#include "stat.h"
#include <ctime>
#include <iostream>
#include <cstring>
#include "util/network_utils.h"
#include "util/constants.h"

namespace Stat {

    void InitRandom() {
        std::srand((unsigned int) std::time(NULL));
    }

    int GenerateRandom() {
        return std::rand();
    }

    SampleData *GenData(const int perf_cnt) {
        SampleData *samples = new SampleData[perf_cnt];
        InitRandom();
        for (int i = 0; i < perf_cnt; i++) {
            samples->a = GenerateRandom() % 9 + 1;
            samples->b = GenerateRandom() % 9 + 1;
        }
        return samples;
    }

    std::shared_ptr<char> GenBuffer(SampleData *samples, const int perf_cnt, int *len) {
        int i = 0;
        int32_t tmp_len = 0;
        int32_t sum = 0;
        SampleData *data = NULL;

        for (i = 0; i < perf_cnt; ++i) {
            tmp_len = (sizeof(int) * 2);
            data = &samples[i];
            // NOTE: Aggregate data length
            sum += tmp_len;
        }
        *len = sum;
        *len += END_TAG_SIZE;
        std::shared_ptr<char> buf(new char[*len], std::default_delete<char[]>());
        return buf;
    }

    void SerializeBody(char *buf, SampleData *samples, const int perf_cnt, int *len) {
        char *p = buf;
        for (int cnt = 0; cnt < perf_cnt; cnt++) {
            SampleData *data = &samples[cnt];
            p += write_int32_to_buf(p, data->a);
            p += write_int32_to_buf(p, data->b);
        }
        *len = (p - buf);
    }


} // namespace trace
