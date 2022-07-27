# folly
## folly dynamic

动态类型，可以存储int64、double、bool、string、array、object等多种类型
* [动态数据类型 folly::dynamic](https://zhiqiang.org/coding/folly-dynamic.html)
* [folly dynamic github doc](https://github.com/facebook/folly/blob/main/folly/docs/Dynamic.md)

## folly F14Map

哈希表实现，默认使用 F14FastMap，其实现使用了 SIMD 指令来加速
* [F14](https://github.com/facebook/folly/blob/main/folly/container/F14.md)
* [Open-sourcing F14 for faster, more memory-efficient hash tables](https://engineering.fb.com/2019/04/25/developer-tools/f14/)
* [hacker news](https://news.ycombinator.com/item?id=19760388)

## Conv

folly::to & folly::toAppend 这类的方法，可以方便的进行类型的转换
* [Conv](https://github.com/facebook/folly/blob/main/folly/docs/Conv.md)
