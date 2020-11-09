#include "Client.h"
#include "util/network_utils.h"
#include <memory>

bool Client::Connect() {
    socket_ = std::make_shared<boost::asio::ip::tcp::socket>(io_context);
    io_context.run();

    boost::system::error_code ec;
    boost::asio::connect(*socket_.get(), endpoint_iterator, ec);
    if (ec) {
        std::cout << "[Error][Connect] " << ec.message() << std::endl;
        return false;
    }
    return true;
}

bool Client::Disconnect() {
    boost::system::error_code ec;
    socket_->close(ec);
    if (ec) {
        std::cout << "[Error][Disconnect] " << ec.message() << std::endl;
        return false;
    }
    return true;
}

bool Client::Handshake() {
    int len = MakeHandshakeLength();
    std::shared_ptr<char> msg(MakeHandshakeMsg(len));

    try {
        boost::asio::write(*socket_.get(), boost::asio::buffer(msg.get(), HEADER_SIZE));
        boost::asio::write(*socket_.get(), boost::asio::buffer(msg.get() + HEADER_SIZE, len + END_TAG_SIZE));
        return true;
    } catch (std::exception &e) {
        std::cout << "[Error][Handshake] " << e.what() << std::endl;
        return false;
    }
}

int Client::MakeHandshakeLength() {
    int len = sizeof(HANDSHAKE_TAG) + strlen(resource_id);
    return len;
}

std::shared_ptr<char> Client::MakeHandshakeMsg(const int32_t len) {
    int32_t buf_size = len + HEADER_SIZE + END_TAG_SIZE;
    std::shared_ptr<char> ret(new char[buf_size], std::default_delete<char[]>());
    char *p = ret.get();

    // Header
    p += AppendHeaderToBuf(p, len);

    // start tag                                                                 
    *p = HANDSHAKE_TAG;
    ++p;

    // resource id                                                               
    p += write_str_to_buf(p, resource_id);

    int8_t end_tag = END_TAG;
    memcpy(p, &end_tag, sizeof(int8_t));

    return ret;
}

int32_t Client::AppendHeaderToBuf(char *buf, const int32_t len) {
    char *p = buf;
    int nlen = htonl(len);
    memcpy(p, &nlen, sizeof(int32_t));
    p += sizeof(int32_t);
    return sizeof(int32_t) + sizeof(int32_t);
}

bool Client::Write(const char *msg_body, const int len) {
    try {
        std::shared_ptr<char> msg_header(new char[HEADER_SIZE], std::default_delete<char[]>());
        AppendHeaderToBuf(msg_header.get(), len);
        boost::asio::write(*socket_.get(), boost::asio::buffer(msg_header.get(), HEADER_SIZE));

        boost::asio::write(*socket_.get(), boost::asio::buffer(msg_body, len));

        boost::asio::write(*socket_.get(), boost::asio::buffer(end_tag_buf, END_TAG_SIZE));

        return true;
    } catch (std::exception &e) {
        std::cout << "[Error][Write] " << e.what() << std::endl;
        return false;
    }
}
