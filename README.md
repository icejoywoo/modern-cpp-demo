Modern Cpp Demo
========

demos for learning modern cpp (since c++11)

tested on MacOS and Linux (Ubuntu)

## contrib

contrib folder is copied from ClickHouse, do some tiny changes according to my requirements.
* jemalloc
* grpc
* protobuf
* boringssl: google fork of openssl
* rapidjson
* poco c++ library
* etc.

## third-party submodules

``` bash
# add submodule
git submodule add https://github.com/google/googletest.git third_party/googletest
git submodule add https://github.com/xtensor-stack/xsimd.git third_party/xsimd
git submodule add https://github.com/google/benchmark.git third_party/benchmark
```

``` bash
# update submodule
git submodule sync --recursive
git submodule update --init --recursive
```

## Learning Materials

* [google test](http://google.github.io/googletest/primer.html)
* [google mock](http://google.github.io/googletest/gmock_for_dummies.html)
* gflags: [github](https://github.com/gflags/gflags) [docs](https://gflags.github.io/gflags/)
* [google glog](https://github.com/google/glog)
* [google benchmark](https://github.com/google/benchmark)
* [google sanitizer](https://github.com/google/sanitizers/wiki/AddressSanitizer)
* The Boost C++ Libraries: [1st cn](http://zh.highscore.de/cpp/boost/), [2nd en](https://theboostcpplibraries.com/), [boost-cmake](https://github.com/Orphis/boost-cmake)

### Articles from WeChat Official Accounts
* [现代 C++ 测试工具链(是时候抛弃 gtest/google bench 了)](https://mp.weixin.qq.com/s/HPs4RMKAoeSfG1qZkS8h3Q)
* [原创 | 如何系统学习C++？](https://mp.weixin.qq.com/s/WW_X12bTm94iaCgWBgYtJw)

# misc

## output size

ubuntu 20.04:
* gcc version 9.4.0: 4.7G
* Ubuntu clang version 15.0.0: 1.8G
* gcc version 11.1.0: 3.7G

install latest clang:
```bash
sudo bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"
```
