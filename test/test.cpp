#include <gtest/gtest.h>

int main(int argc, char** argv) {
    printf("Running main() from %s\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    ::testing::GTEST_FLAG(filter) = "ArTreeTest*";
    return RUN_ALL_TESTS();
}