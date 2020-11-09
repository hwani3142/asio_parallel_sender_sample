#include "stat_sender_test.h"
#include <sstream>
#include <string>
#include "boost/thread/thread.hpp"
#include "stat.h"

int StatSender::argc = 0;
char *StatSender::argv[] = {};
const static char RES_PRE[] = "STAT";

bool TestAll(const int thread_num, const int perf_cnt, const int repeat_cnt);

bool Connect(std::shared_ptr<Client> client);

bool Send(std::shared_ptr<Client> client, const int perf_cnt, const int repeat_cnt);

bool Disconnect(std::shared_ptr<Client> client);

TEST_F(StatSender, ConnectAndHandshake) {
    const char resource_id[] = "T_CON";
    std::shared_ptr<Client> client = GetClient(resource_id);

    bool connect = client->Connect();
    ASSERT_EQ(true, connect);

    bool handshake = client->Handshake();
    ASSERT_EQ(true, handshake);

    bool disconnect = client->Disconnect();
    ASSERT_EQ(true, disconnect);
}

TEST_F(StatSender, Single) {
    const int thread_num = 0;
    const int perf_cnt = 1;
    const int repeat_cnt = 1;
    bool test_all = TestAll(thread_num, perf_cnt, repeat_cnt);
    ASSERT_EQ(true, test_all);
}

TEST_F(StatSender, ParallelSend) {
    boost::thread *threads[num_threads];

    for (int i = 0; i < num_threads; i++) {
        threads[i] = new boost::thread(TestAll, i + 1,
                                       perf_cnt, repeat_cnt);
    }
    for (int i = 0; i < num_threads; i++) {
        threads[i]->join();
    }
    for (int i = 0; i < num_threads; i++) {
        delete threads[i];
    }
}


bool TestAll(const int thread_num, const int perf_cnt, const int repeat_cnt) {
    std::stringstream ss;
    ss << RES_PRE << thread_num;
    std::string resource_id = ss.str();

    std::shared_ptr<Client> client = StatSender::GetClient(resource_id);

    bool conn_result = Connect(client);
    if (!conn_result) return false;

    bool send_result = Send(client, perf_cnt, repeat_cnt);
    bool disconn_result = Disconnect(client);

    return send_result && disconn_result;
}

bool Connect(std::shared_ptr<Client> client) {
    bool result = client->Connect();
    if (!result) return false;

    bool handshake = client->Handshake();
    return handshake;
}

bool Send(std::shared_ptr<Client> client, const int perf_cnt, const int repeat_cnt) {
    const char *resource_id = client->GetResourceId();
    for (int i = 0; i < repeat_cnt; i++) {
        std::cout << "[" << resource_id << "] Step [" << i + 1 << "/" << repeat_cnt << "]" << std::endl;
        int len, msg_header_len;
        Stat::SampleData *samples = Stat::GenData(perf_cnt);
        std::shared_ptr<char> msg_body = Stat::GenBuffer(samples, perf_cnt, &len);
        char *pb = msg_body.get();

        int body_len;
        Stat::SerializeBody(pb, samples, perf_cnt, &body_len);

        bool write = client->Write(msg_body.get(), msg_header_len + body_len);
        if (!write) return false;

        delete[] samples;

        client->WaitNextPushPeriod();
    }

    client->SleepUntilSafeLoad();
    return true;
}

bool Disconnect(std::shared_ptr<Client> client) {
    return client->Disconnect();
}
