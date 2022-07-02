#include "cc_string.h"
#include "cc_mem.h"
#include "gtest/gtest.h"

namespace
{
    class TestCcString : public testing::Test
    {
        void SetUp() override
        {};

        void TearDown() override
        {};
    };
}

TEST_F(TestCcString, TEST_CC_LOG)
{
    cc_char *dst = (cc_char*)cc_malloc(10);
    cc_char *src = (cc_char*)cc_malloc(10);

    sprintf(src, "srcstring\0");
    sprintf(dst, "dststring\0");

    (void)cc_cpystrn(dst, src, 10);
    ASSERT_STREQ("srcstring", dst);
}
