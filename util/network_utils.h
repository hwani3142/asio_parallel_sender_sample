#ifndef _NETWORK_UTILS_H
#define _NETWORK_UTILS_H

#include <cstdint>

int32_t write_int32_to_buf(char *buf, int32_t val);

int32_t write_int64_to_buf(char *buf, int64_t val);

int32_t write_str_to_buf(char *buf, const char *str);

int32_t write_float_to_buf(char *buf, float val);

int32_t write_double_to_buf(char *buf, double val);

int32_t read_int32_from_msg(const char *&msg);

int64_t read_int64_from_msg(const char *&msg);

char *read_string_from_buf(const char *&msg);

float read_float_from_msg(const char *&msg);

double read_double_from_msg(const char *&msg);

#endif /* _NETWORK_UTILS_H */
