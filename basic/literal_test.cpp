#include "common_util.h"

#include <gtest/gtest.h>
#include <iostream>

TEST(LiteralTest, HexBinaryTest) {
    // since c++11
    double float_array[]{ 5.875, 1000, 0.177 };
    // 十六进制浮点数的表示方法，以0x1.f40000p+9为例：
    // 其中0x1.f4是一个十六进制的有效数，p+9是一个以2为底数，9为指数的幂。
    // 其中底数一定为2，指数使用的是十进制。也就是说0x1.f40000p+9可以表示为：0x1.f4 * 2^9。
    for (auto& elem : float_array) {
        std::cout << std::hexfloat << elem << " = "
            << std::defaultfloat << elem << std::endl;
    }

    auto x = 0b1100;
    ASSERT_EQ(12, x);

    auto y = 123'456;
    ASSERT_EQ(123456, y);
}

TEST(LiteralTest, StringTest) {
    char hello_world_html[] = R"(<!DOCTYPE html>
<html lang="en">
<body>
Hello, World!
</body>
</html>)";
    printf("%s\n", hello_world_html);
    // ) conflict
    char hello_world_html2[] = R"cpp(<!DOCTYPE html>
<html lang="en">
<body>
(Hello, World!)
</body>
</html>)cpp";
    printf("%s\n", hello_world_html2);

    char8_t utf8[] = u8R"(你好世界)"; // c++ 20
    char16_t utf16[] = uR"(你好世界)";
    char16_t utf32[] = uR"(你好世界)";
    wchar_t wstr[] = LR"(你好世界)";

    // disable warnings: https://www.fluentcpp.com/2019/08/30/how-to-disable-a-warning-in-cpp/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
    SIMPLE_PRINT(utf8, "%s");
    // 下面的打印都是不太对的，暂时不知道如何print
    SIMPLE_PRINT(utf16, "%s");
    SIMPLE_PRINT(utf32, "%s");
    SIMPLE_PRINT(wstr, "%ls");
#pragma GCC diagnostic pop
}