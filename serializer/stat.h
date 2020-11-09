#ifndef _STAT_SERIALIZER_H
#define _STAT_SERIALIZER_H

#include <cstdint>
#include <sys/types.h>
#include <memory>

namespace Stat {
/* NOTE: Design your structure */
    typedef struct sampleData {
        int a;
        int b;
    } SampleData;

    SampleData *GenData(const int perf_cnt);

    std::shared_ptr<char> GenBuffer(SampleData *sql_perfs, const int perf_cnt, int *len);

    void SerializeBody(char *buf, SampleData *sql_perfs, const int perf_cnt, int *len);

} // namespace Stat

#endif
