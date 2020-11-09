#include "network_utils.h"
#include <netinet/in.h>
#include <cstring>
#include <cstdint>

#define htonll(x)   ntohll(x)
#define ntohll(x)   (((uint64_t)(ntohl((uint32_t)((x << 32) >> 32))) << 32) |\
                     (uint32_t)ntohl(((uint32_t)(x >> 32))))
#define htonf(x) ntohf(x)
#define htond(x) ntohd(x)

float ntohf(float value) {
    unsigned char *bp;
    unsigned char t;
    bp = (unsigned char *) &value;
    t = *bp;
    *bp = *(bp + 3);
    *(bp + 3) = t;
    t = *(bp + 1);
    *(bp + 1) = *(bp + 2);
    *(bp + 2) = t;
    return value;
}

double ntohd(double value) {
    unsigned char *bp;
    unsigned char t;
    bp = (unsigned char *) &value;
    t = *bp;
    *bp = *(bp + 7);
    *(bp + 7) = t;
    t = *(bp + 1);
    *(bp + 1) = *(bp + 6);
    *(bp + 6) = t;
    t = *(bp + 2);
    *(bp + 2) = *(bp + 5);
    *(bp + 5) = t;
    t = *(bp + 3);
    *(bp + 3) = *(bp + 4);
    *(bp + 4) = t;
    return value;
}


int32_t write_int32_to_buf(char *buf, int32_t val) {
    int32_t net_val;

    net_val = htonl(val);
    memcpy(buf, &net_val, sizeof(net_val));

    return sizeof(int32_t);
}

int32_t write_int64_to_buf(char *buf, int64_t val) {
    int64_t net_val;

    net_val = htonll(val);
    memcpy(buf, &net_val, sizeof(net_val));

    return sizeof(int64_t);
}

int32_t write_str_to_buf(char *buf, const char *str) {
    char *p = buf;
    int32_t len, wlen = 0;

    // [length] + [string]
    len = strlen(str);
    p += write_int32_to_buf(p, len);
    memcpy(p, str, len);
    wlen += len + sizeof(len);

    return wlen;
}

int32_t write_float_to_buf(char *buf, float val) {
    float net_val;

    net_val = htonf(val);
    memcpy(buf, &net_val, sizeof(net_val));

    return sizeof(float);
}

int32_t write_double_to_buf(char *buf, double val) {
    val = htond(val);
    memcpy(buf, &val, sizeof(val));
    return sizeof(double);
}


int32_t read_int32_from_msg(const char *&msg) {
    // NOTE: Big to little endian
    int value = (msg[0] & 0xff) << 24 | (msg[1] & 0xff) << 16 |
                (msg[2] & 0xff) << 8 | (msg[3] & 0xff);

    msg += sizeof(int32_t);
    return value;
}

int64_t read_int64_from_msg(const char *&msg) {
    int64_t value = (int64_t) read_int32_from_msg(msg) << 32 |
                    (read_int32_from_msg(msg) & 0xffffffffl);
    return value;
}

char *read_string_from_buf(const char *&msg) {
    unsigned char stag = *msg++;
    int length = read_int32_from_msg(msg);
    char *result = new char[length + 1];

    for (int i = 0; i < length; i++) {
        if (msg[i] == '\n') {
            result[i] = ' ';
            continue;
        }
        result[i] = msg[i];
    }
    result[length] = '\0';
    msg += length;
    return result;
}

float read_float_from_msg(const char *&msg) {
    float value;
    memcpy(&value, msg, sizeof(float));
    value = ntohf(value);

    msg += sizeof(float);
    return value;
}

double read_double_from_msg(const char *&msg) {
    double value;
    memcpy(&value, msg, sizeof(double));
    value = ntohd(value);

    msg += sizeof(double);
    return value;
}