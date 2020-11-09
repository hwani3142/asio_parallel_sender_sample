#include "gtest/gtest.h"
#include <cstdlib>
#include "client/Client.h"
#include <memory>

class StatSender : public ::testing::Test {
protected:
    virtual void SetUp() {
        SetEnvironment();
    }

    virtual void TearDown() {
    }

    void SetEnvironment() {
        perf_cnt = atoi(argv[3]);
        repeat_cnt = atoi(argv[4]);
        num_threads = atoi(argv[5]);
    }

public:
    int perf_cnt;
    int repeat_cnt;
    int num_threads;

    static int argc;
    static char *argv[];

    static void init(int argc, char *argv[]) {
        StatSender::argc = argc;
        for (int i = 0; i < argc; i++) {
            StatSender::argv[i] = argv[i];
        }
    }

    static std::shared_ptr<Client> GetClient(const std::string &resource_id) {
        return std::make_shared<Client>(StatSender::argv[1], StatSender::argv[2], resource_id);
    }
};
