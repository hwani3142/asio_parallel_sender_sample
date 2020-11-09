#include "gtest/gtest.h"
#include "stat_sender_test.h"

int main(int argc, char *argv[]) {
    if (argc != 6) {
        std::cout << "[Usage] Insert {host}, {port}, {row_num}, {repeat_times} and {thread_num}, respectively"
                  << std::endl
                  << "Ex) ./parallel_sender 127.0.0.1 22007 50 600 2" << std::endl
                  << "Connect 127.0.0.1:22007, then send 50 rows 600 times for 2 threads" << std::endl;
        return 0;
    }
    /* Initialize Google Framework */
    ::testing::InitGoogleTest(&argc, argv);
    StatSender::init(argc, argv);

    return RUN_ALL_TESTS();
}
