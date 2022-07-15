Modern Cpp Demo
========

demos for learning modern cpp (since c++11)


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
* [google benchmark](https://github.com/google/benchmark)
* [google sanitizer](https://github.com/google/sanitizers/wiki/AddressSanitizer)