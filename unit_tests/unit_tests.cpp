/**
 * Testes unitários da biblioteca t2fs.
 */

#ifdef __cplusplus
extern "C"
{
#endif

#include "../include/t2fs.h"

#ifdef __cplusplus
}
#endif

#include "gtest/gtest.h"


TEST(identify2, asserts_size) {
    int big_size = 500;
    char big_string[big_size];

    EXPECT_GE(identify2(big_string, big_size), 0);

    int small_size = 10;
    char small_string[small_size];

    EXPECT_LT(identify2(small_string, small_size), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
