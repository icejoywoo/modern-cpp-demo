// OBOE short for off-by-one-error
// 在计数时由于边界条件判断失误导致结果多了一或少了一的错误
#include <cstdio>

int main() {
    // 这个目前实测没有提示出来，编译会报warning
    int a[] = {0, 1};
    int b = a[3];
    printf("b: %d\n", b);
    return 0;
}