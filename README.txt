# ASIO_PARALLEL_SENDER_SAMPLE

## Purpose
* Strength test using boost-asio multi-client

## Requirements (included)
* Boost
* pthread
* gtest

## Customization
* Design protocol
  * Handshake
    * Sample:
  * Message
    * Sample:
* Serializer (serializer/**)
  * Design data structure, then serialize them
  * Calculate size of buffer


## Build
```sh
$ ./build.sh
```

## Run
### Script
```sh
$ ./run.sh
```
### Manual
```sh
# Connect 127.0.0.1:22007, then send 50 rows 10 times for 2 threads
$ ./parallel_sender 127.0.0.1 22007 50 10 2
```

