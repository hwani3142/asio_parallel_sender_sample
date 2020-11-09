#ifndef _CLIENT_H
#define _CLIENT_H

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <memory>
#include "util/constants.h"

class Client {
public:
    Client(const std::string &host, const std::string &port, const std::string &resource_id_)
            : resource_id(""), pid(22000), host_(host), port_(port), hs_cnt(1) {
        boost::asio::ip::tcp::resolver resolver(io_context);
        boost::asio::ip::tcp::resolver::query query(host_, port_);
        endpoint_iterator = resolver.resolve(query);

        SetResourceId(resource_id_.c_str());

        int8_t end_tag = END_TAG;
        memcpy(end_tag_buf, &end_tag, sizeof(int8_t));
    }

    ~Client() {
    }

    bool Connect();

    bool Disconnect();

    bool Handshake();

    int MakeHandshakeLength();

    std::shared_ptr<char> MakeHandshakeMsg(const int32_t len);

    int32_t AppendHeaderToBuf(char *buf, const int32_t len);

    bool Write(const char *msg_body, const int len);

    inline void SetResourceId(const char *resource_id) {
        strcpy(this->resource_id, resource_id);
    }

    inline char *GetResourceId() {
        return this->resource_id;
    }

    inline void WaitNextPushPeriod() {
        sleep(1);   // 1 sec
    }

    inline void SleepUntilSafeLoad() {
        const int sleep_sec = 1;  // 1 sec
        for (int i = 0; i < sleep_sec; i++) {
            std::cout << "[" << GetResourceId() << "] Sleep until safe load ["
                      << i + 1 << "/" << sleep_sec << "]" << std::endl;
            sleep(1);
        }
    }

    char end_tag_buf[1];
private:
    boost::asio::io_service io_context;
    std::shared_ptr<boost::asio::ip::tcp::socket> socket_;
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator;
    char resource_id[MAX_LEN_RESOURCE_ID];
    pid_t pid;
    std::string host_;
    std::string port_;
    const int32_t hs_cnt;
};

#endif // _CLIENT_H
